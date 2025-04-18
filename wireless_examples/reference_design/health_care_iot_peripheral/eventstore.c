/*
 * Copyright 2022-2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "eventstore.h"
#include "fsl_debug_console.h"
#include "fsl_iap.h"
#include "fsl_os_abstraction.h"
#include "fwk_platform.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/** The number of entries in the event store */
#define ENTRY_COUNT (512 / sizeof(eventstore_entry_t))
/** The number of copies */
#define COPY_COUNT 2
/** Extracts the event type from the entry */
#define GET_TYPE(entry) ((entry).event & 0x0f)
/** Extract the metadata from the entry */
#define GET_META(entry)     ((entry).event >> 4)
#define GET_ALL_META(entry) ((entry).event)

/** Creates an event from type and metadata */
#define CREATE_EVENT(type, meta) ((type & 0x0f) | (meta << 4))
/** Transforms the radio state to string */
#define RADIOSTATE_TO_STR(x) \
    (x == kSAFETY_RadioState_Advertising ? "advertising" : x == kSAFETY_RadioState_Connected ? "connected" : "unknown")
/** Transforms the reset cause to string */
#define RESETCAUSE_TO_STR(x) (x == kSAFETY_ResetCause_Por ? "POR" : x == kSAFETY_ResetCause_Bod ? "BOD" : "unknown")
#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
#define EPOCH2020 1577833200 /* Epoch time for 01/01/2020 12AM:00:00 */
#endif                       /* (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U)) */

/**
 * The different supported event types
 */
typedef enum
{
    kEVENTSTORE_EntryType_Reset       = 1,
    kEVENTSTORE_EntryType_DeviceState = 2,
    kEVENTSTORE_EntryType_RadioState  = 3,
    kEVENTSTORE_EntryType_CurrentTime = 4,
    kEVENTSTORE_EntryType_Empty       = 0xf
} eventstore_entry_type_t;

/**
 * The entry structure
 */
typedef struct
{
    /** The time when the event happened */
    uint32_t timestamp;
    /** The event itself */
    uint32_t event;
} eventstore_entry_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static eventstore_entry_t *NextEntry(eventstore_entry_t *previousEntry);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/** The location is flash where the eventstore is located */
static const uint8_t *s_flashLocation;
/** Buffer containing the event store entries */
static eventstore_entry_t s_entries[ENTRY_COUNT];
/** The active copy */
static uint32_t s_activeCopy;

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * Prints the events in the event store
 */
static void PrintEvents()
{
    eventstore_entry_t *entry = NULL;
    PRINTF("Eventstore content:\n");
    while ((entry = NextEntry(entry)) != NULL)
    {
        PRINTF("  %lds: ", entry->timestamp);
        switch (GET_TYPE(*entry))
        {
            case kEVENTSTORE_EntryType_Empty:
                PRINTF("Empty \n");
                break;
            case kEVENTSTORE_EntryType_Reset:
                PRINTF("Device reset %s\n", RESETCAUSE_TO_STR(GET_META(*entry)));
                break;
            case kEVENTSTORE_EntryType_DeviceState:
                PRINTF("Device state changed to %ld\n", GET_META(*entry));
                break;
            case kEVENTSTORE_EntryType_RadioState:
                PRINTF("Radio state changed to %s\n", RADIOSTATE_TO_STR(GET_META(*entry)));
                break;
#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
            case kEVENTSTORE_EntryType_CurrentTime:
                PRINTF("User Time Offset = %ld\n", (((uint32_t)GET_META(*entry) + EPOCH2020)));
                break;
#endif /* (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U)) */
            default:
                PRINTF("Unknown\n");
                break;
        }
    }
}

/**
 * Searches the next entry in the event store
 *
 * @param previousEntry The previous entry or NULL when looking for the first entry
 * @return The next entry of NULL when the end of the store has been reached
 */
static eventstore_entry_t *NextEntry(eventstore_entry_t *previousEntry)
{
    /* Do we need to look for the first entry? */
    if (previousEntry == NULL)
    {
        /* Yes, point to the first entry */
        previousEntry = s_entries;
    }
    else
    {
        /* No, jump to the next entry */
        previousEntry++;
    }

    /* Have we reached the end of the store or is the entry empty? */
    if ((previousEntry - s_entries >= ENTRY_COUNT) || (GET_TYPE(*previousEntry) == kEVENTSTORE_EntryType_Empty))
    {
        /* Yes, return NULL to indicate end of entries */
        previousEntry = NULL;
    }
    return previousEntry;
}

/**
 * Adds a new entry to the store. The current time is automatically included
 *
 * @param event The entry to add
 * @return kStatus_Success in case of success
 */
static status_t AddEntry(uint32_t event)
{
    uint32_t idx;
    /* Look for an empty entry and store the event */
    for (idx = 0; idx < ENTRY_COUNT; idx++)
    {
        if (GET_TYPE(s_entries[idx]) == kEVENTSTORE_EntryType_Empty)
        {
            s_entries[idx].timestamp = PLATFORM_GetTimeStamp();
            s_entries[idx].event     = event;
            break;
        }
    }
    /* Was there still an empty entry that could be used to store the event? */
    return idx == ENTRY_COUNT ? kStatus_Fail : kStatus_Success;
}

/**
 * Stores the event store to flash.
 * An alternating location is chosen to improve robustness against faulty writes
 *
 * @param cb Optional cb function that is called when the data is effectively written to flash
 */
static void WriteEventStore(safety_written_cb_t cb, void *context)
{
    /* Switch to the next copy */
    s_activeCopy = (s_activeCopy + 1) % 2;
    /* Request the safety process to write the data */
    SAFETY_WriteFlash((uint32_t)s_flashLocation + s_activeCopy * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES,
                      (uint8_t *)s_entries, sizeof(s_entries), true, cb, context);
}

/**
 * Initializes the event store
 * The flashLocation must be sector aligned
 * The flashSize must be exactly 2 * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES
 *
 * @param flashLocation The location in flash
 * @param flashSize The reserved space in flash
 * @return kStatus_Success on success
 */
status_t EVENTSTORE_Init(const uint8_t *flashLocation, uint32_t flashSize)
{
    status_t status = kStatus_Fail;
    /* Is the flash location sector aligned and does it consist of COPY_COUNT sectors? */
    if ((((uint32_t)flashLocation & (FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES - 1)) == 0) &&
        (flashSize == COPY_COUNT * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES))
    {
        uint32_t idx;
        uint32_t maxCount = 0;
        status            = kStatus_Success;
        s_flashLocation   = flashLocation;
        flash_config_t flashInstance;

        status = FLASH_Init(&flashInstance);
        assert_equal(status, kStatus_Success);

        /* Search for the copy with most entries. This is considered to be the most recent copy */
        for (idx = 0; idx < COPY_COUNT; idx++)
        {
            uint32_t count            = 0;
            eventstore_entry_t *entry = NULL;
            uint32_t sr;

            /* If a flash phrase contains ECC errors, reading it will cause a HardFault. We check the flash sector
             * first. If a sector contains an ECC error, we erase the sector to get it back in a consistent state.
             */
            /* FLASH_CheckECC requires the kCLOCK_Sysctl, which is enabled in BOARD_InitClk */
            OSA_EnterCritical(&sr);
            status = FLASH_CheckECC(&flashInstance,
                                    (uint32_t)s_flashLocation + idx * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES,
                                    FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES);
            assert((status == kStatus_Success) || (status == kStatus_FLASH_AccessError));
            if (kStatus_FLASH_AccessError == status)
            {
                status = FLASH_Erase(&flashInstance,
                                     (uint32_t)s_flashLocation + idx * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES,
                                     FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES, (uint32_t)kFLASH_ApiEraseKey);
                assert_equal(status, kStatus_Success);
                PRINTF("ECC error detected, erasing EVENTSTORE at 0x%x\n",
                       (uint32_t)s_flashLocation + idx * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES);
            }
            OSA_ExitCritical(sr);

            memcpy(s_entries, s_flashLocation + idx * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES, sizeof(s_entries));
            /* Count the number of entries in this copy */
            while ((entry = NextEntry(entry)) != NULL)
            {
                count++;
            }
            if (count >= maxCount)
            {
                maxCount     = count;
                s_activeCopy = idx;
            }
        }
        /* Now copy the most recent copy into the buffer */
        memcpy(s_entries, s_flashLocation + s_activeCopy * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES,
               sizeof(s_entries));
        PrintEvents();
    }
    return status;
}

/**
 * Returns the last recorded device state
 *
 * @return The last recorded device state
 */
safety_device_state_t EVENTSTORE_GetLatestDeviceState(void)
{
    eventstore_entry_t *entry   = NULL;
    safety_device_state_t state = kSAFETY_DeviceState_Uninitialized;
    while ((entry = NextEntry(entry)) != NULL)
    {
        if (GET_TYPE(*entry) == kEVENTSTORE_EntryType_DeviceState)
        {
            state = (safety_device_state_t)GET_META(*entry);
        }
    }
    return state;
}

/**
 * Records the given device state to the event store
 *
 * @param state The device state to record
 * @param cb Function called when new state is stored in flash
 * @return kStatus_Success upon success
 */
status_t EVENTSTORE_RecordDeviceState(safety_device_state_t state, safety_written_cb_t cb, void *context)
{
    status_t status = AddEntry(CREATE_EVENT(kEVENTSTORE_EntryType_DeviceState, state));
    if (status == kStatus_Success)
    {
        WriteEventStore(cb, context);
    }
    return status;
}

/**
 * Records the given radio state to the event store
 *
 * @param radioState The radio state
 */
status_t EVENTSTORE_RecordRadioState(safety_radio_state_t radioState)
{
    status_t status = AddEntry(CREATE_EVENT(kEVENTSTORE_EntryType_RadioState, radioState));
    if (status == kStatus_Success)
    {
        WriteEventStore(NULL, NULL);
    }
    return status;
}

/**
 * Records the reset cause
 *
 * @param radioState The radio state
 */
status_t EVENTSTORE_RecordResetCause(safety_reset_cause_t resetCause)
{
    status_t status = AddEntry(CREATE_EVENT(kEVENTSTORE_EntryType_Reset, resetCause));
    if (status == kStatus_Success)
    {
        WriteEventStore(NULL, NULL);
    }
    return status;
}

#if (defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U))
/**
 * Returns the offset of current time after synchronization with the client
 *
 * @return  offset of current time after synchronization
 */
uint32_t EVENTSTORE_GetCurrentTimeOffset(void)
{
    eventstore_entry_t *entry  = NULL;
    uint32_t currentTimeOffset = 0;
    while ((entry = NextEntry(entry)) != NULL)
    {
        if (GET_TYPE(*entry) == kEVENTSTORE_EntryType_CurrentTime)
        {
            /* add the Epoch Time for 01/01/2020 12AM:00:00 to the recorded time */
            currentTimeOffset = ((uint32_t)GET_META(*entry) + EPOCH2020) - entry->timestamp;
            break;
        }
    }
    return currentTimeOffset;
}

/**
 * Records the user current time
 *
 * @param currentTime User current time to record
 */
status_t EVENTSTORE_RecordCurrentTime(uint32_t currentTime)
{
    currentTime -= EPOCH2020;
    status_t status = AddEntry(CREATE_EVENT(kEVENTSTORE_EntryType_CurrentTime, (currentTime - PLATFORM_GetTimeStamp())));
    if (status == kStatus_Success)
    {
        WriteEventStore(NULL, NULL);
    }

    return status;
}
#endif /*(defined(gAppUseTimeService_d) && (gAppUseTimeService_d == 1U)) */
