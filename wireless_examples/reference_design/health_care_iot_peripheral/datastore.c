/*
 * Copyright 2022-2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "safety.h"
#include "fwk_platform.h"
#include "fsl_debug_console.h"
#include "fsl_iap.h"
#include "fsl_os_abstraction.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CACHE_SIZE FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool s_busyWriting;
static uint8_t s_cache[CACHE_SIZE];
static const uint8_t *s_flashLocation;
static uint32_t s_flashSize;
static uint32_t s_entrySize;
static uint32_t s_cacheOffset;
static uint32_t s_flashOffset;

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * Function called when cache is effectively written to flash
 *
 * @param data Pointer to the data that has been written
 * @param length Number of bytes that have been written
 */
static void CacheWritten(uint8_t *data, uint32_t length, void *context)
{
    /* It's safe to modify these variables in the caller's context (other thread) */
    memset(s_cache, 0, sizeof(s_cache));
    s_cacheOffset = 0;
    s_flashOffset += sizeof(s_cache);
    s_busyWriting = false;
}

/**
 * Initializes the datastore
 *
 * @param flashLocation The location in flash where the data is stored persistently
 * @param flashSize The reserved size in bytes for the datastore in flash
 * @param entrySize The size of a single entry in bytes
 */
void DATASTORE_Init(const uint8_t *flashLocation, uint32_t flashSize, uint32_t entrySize)
{
    /* Round down the flash size to a multiple of cache size */
    s_flashSize     = ((uint32_t)(flashSize / sizeof(s_cache))) * sizeof(s_cache);
    s_flashLocation = flashLocation;
    s_entrySize     = entrySize;
    s_cacheOffset   = 0;
    s_busyWriting   = false;
    s_flashOffset   = 0;
    flash_config_t flashInstance;
    status_t status;
    uint32_t sr;

    status = FLASH_Init(&flashInstance);
    assert_equal(status, kStatus_Success);

    /* If a flash phrase contains ECC errors, reading it will cause a HardFault. We check the flash sector
     * first. If a sector contains an ECC error, we erase the sector to get it back in a consistent state.
     */
    /* FLASH_CheckECC requires the kCLOCK_Sysctl, which is enabled in BOARD_InitClk */
    OSA_EnterCritical(&sr);
    status = FLASH_CheckECC(&flashInstance, (uint32_t)s_flashLocation, flashSize);
    assert((status == kStatus_Success) || (status == kStatus_FLASH_AccessError));
    if (kStatus_FLASH_AccessError == status)
    {
        status = FLASH_Erase(&flashInstance, (uint32_t)s_flashLocation, flashSize, (uint32_t)kFLASH_ApiEraseKey);
        assert_equal(status, kStatus_Success);
        PRINTF("ECC error detected, erasing DATASTORE at 0x%x\n", (uint32_t)flashLocation);
    }
    OSA_ExitCritical(sr);

    /* Skip the already stored data */
    while (s_flashOffset < s_flashSize)
    {
        uint32_t i;
        uint8_t zerocheck = 0;
        uint8_t onecheck  = 0xff;
        /* Search the first page that is completely empty. Other pages are
         * assumed to be in use. Note that an empty page may contains all zeros
        (compiler will initialize the array with zeros) or all ones (after erase) */
        for (i = 0; i < sizeof(s_cache) && (zerocheck == 0 || onecheck == 0xff); i++)
        {
            zerocheck |= s_flashLocation[s_flashOffset + i];
            onecheck &= s_flashLocation[s_flashOffset + i];
        }
        if ((zerocheck == 0) || (onecheck == 0xff))
        {
            break;
        }
        s_flashOffset += sizeof(s_cache);
    }
    memset(s_cache, 0, sizeof(s_cache));
}

/**
 * Adds an entry to the data store.
 *
 * @param entry Pointer to the entry.
 * @post The cache might be written to flash if the cache is full
 * @return kStatus_Success in case of success
 */
status_t DATASTORE_Add(const void *entry)
{
    status_t status = kStatus_Success;
    /* Is a previous flash write still ongoing ? */
    if (s_busyWriting)
    {
        status = kStatus_Fail;
    }
    /* No, is the flash full? */
    else if (s_flashOffset >= s_flashSize)
    {
        status = kStatus_Fail;
    }
    /* No, then add the entry to the cache */
    else
    {
        /* Prefix the entry with the current time */
        uint32_t rtc = PLATFORM_GetTimeStamp();
        memcpy(s_cache + s_cacheOffset, &rtc, sizeof(rtc));
        s_cacheOffset += sizeof(rtc);
        memcpy(s_cache + s_cacheOffset, entry, s_entrySize);
        s_cacheOffset += s_entrySize;
        /* Is there less than 1 entry space left in the cache ? */
        if (sizeof(s_cache) - s_cacheOffset < s_entrySize + sizeof(rtc))
        {
            /* Yes, then write the data to flash */
            s_busyWriting = true;
            /* Erase sector first if this is the first data of this sector */
            bool eraseFirst =
                (((uint32_t)(s_flashLocation + s_flashOffset)) & (FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES - 1)) == 0;
            SAFETY_WriteFlash((uint32_t)s_flashLocation + s_flashOffset, s_cache, sizeof(s_cache), eraseFirst,
                              CacheWritten, NULL);
        }
    }
    return status;
}
