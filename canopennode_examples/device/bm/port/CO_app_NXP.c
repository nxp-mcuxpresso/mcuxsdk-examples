/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "CO_app_NXP.h"
#include "OD.h"

/* default values for CO_CANopenInit() */
#define NMT_CONTROL          (CO_NMT_ERR_ON_ERR_REG | CO_ERR_REG_GENERIC_ERR | \
                              CO_ERR_REG_COMMUNICATION)
#define FIRST_HB_TIME        500
#define SDO_SRV_TIMEOUT_TIME 1000
#define SDO_CLI_TIMEOUT_TIME 500
#define SDO_CLI_BLOCK        false
#define OD_STATUS_BITS       NULL

/* It will be set by canopen_app_init and will be used across app to get access to CANOpen objects */
CANopenNodeNXP *canopenNodeNXP;

/* Global variables and objects */
CO_t *CO = NULL; /* CANopen object */

/* Global variables */
uint64_t time_old, time_current;
CO_ReturnError_t err;
volatile uint64_t g_systickCounter = 0;
volatile uint8_t g_reqFlag = 0;

void SysTick_Handler(void)
{
    g_systickCounter++;
}

static uint64_t GetTickCounter(void)
{
  return g_systickCounter;
}

void gpt_timerInit(GPT_Type *base, gpt_output_compare_channel_t ch,uint32_t gptFreq)
{
    gpt_config_t gptConfig;

    GPT_GetDefaultConfig(&gptConfig);

    /* Initialize GPT module */
    GPT_Init(base, &gptConfig);

    /* Sets the time period of the GPT module */
    GPT_SetOutputCompareValue(base, ch, gptFreq);
}

static void gptStart_IT(gpt_handle *timerHandle)
{
    /* Enable GPT interrupt */
    GPT_EnableInterrupts(timerHandle->base, timerHandle->gptChMask);
    /* Enable at the Interrupt */
    EnableIRQ(timerHandle->gptIRQid);
    /* Start Timer */
    GPT_StartTimer(timerHandle->base);
}

static void gptStop_IT(gpt_handle *timerHandle)
{
    /* Disable GPT interrupt */
    GPT_DisableInterrupts(timerHandle->base, timerHandle->gptChMask);
    /* Disable at the Interrupt */
    DisableIRQ(timerHandle->gptIRQid);
    /* Stop Timer */
    GPT_StopTimer(timerHandle->base);
}

char *getNMT_string(CO_NMT_internalState_t state)
{
    switch (state)
    {
    case CO_NMT_INITIALIZING:
        return "Initializing";
    case CO_NMT_PRE_OPERATIONAL:
        return "PreOperational";
    case CO_NMT_OPERATIONAL:
        return "Operational";
    case CO_NMT_STOPPED:
        return "Stopped";
    default:
        return "Unknown";
    }
}

#if (CO_CONFIG_NMT) & CO_CONFIG_NMT_CALLBACK_CHANGE
void changedNMT_callback(CO_NMT_internalState_t state)
{
    PRINTF("Current state of the device is <%s>!\r\n", getNMT_string(state));
}
#endif

void RPDO_calback(void *object)
{
    PRINTF("The request message is received from the CANopen manager!\r\n");
    g_reqFlag = 1;
}

/* This function will basically setup the CANopen node */
int canopen_app_init(CANopenNodeNXP *_canopenNodeNXP)
{
    /* Keep a copy global reference of canOpenNXP Object */
    canopenNodeNXP = _canopenNodeNXP;

#if (CO_CONFIG_STORAGE) & CO_CONFIG_STORAGE_ENABLE
    static CO_storage_t storage;
    static CO_storage_entry_t storageEntries[] = {{.addr = &OD_PERSIST_COMM,
                                                   .len = sizeof(OD_PERSIST_COMM),
                                                   .subIndexOD = 2,
                                                   .attr = CO_storage_cmd | CO_storage_restore,
                                                   .addrNV = NULL}};
    uint8_t storageEntriesCount = sizeof(storageEntries) / sizeof(storageEntries[0]);
    uint32_t storageInitError = 0;
#endif

    /* Allocate memory */
    CO_config_t *config_ptr = NULL;
#ifdef CO_MULTIPLE_OD
    /* example usage of CO_MULTIPLE_OD (but still single OD here) */
    CO_config_t co_config = {0};
    OD_INIT_CONFIG(co_config);
    co_config.CNT_LEDS = 1;
    co_config.CNT_LSS_SLV = 1;
    config_ptr = &co_config;
#endif /* CO_MULTIPLE_OD */

    uint32_t heapMemoryUsed;
    CO = CO_new(config_ptr, &heapMemoryUsed);
    if (CO == NULL)
    {
        PRINTF("Error: Can't allocate memory\r\n");
        return 1;
    }
    else
    {
        PRINTF("Allocated %u bytes for CANopen objects\r\n", heapMemoryUsed);
    }

    canopenNodeNXP->canOpenStack = CO;

#if (CO_CONFIG_STORAGE) & CO_CONFIG_STORAGE_ENABLE
    err = CO_storageBlank_init(&storage, CO->CANmodule, OD_ENTRY_H1010_storeParameters,
                               OD_ENTRY_H1011_restoreDefaultParameters, storageEntries, storageEntriesCount,
                               &storageInitError);

    if (err != CO_ERROR_NO && err != CO_ERROR_DATA_CORRUPT)
    {
        PRINTF("Error: Storage %d\r\n", storageInitError);
        return 2;
    }
#endif

    canopen_app_resetCommunication();
    return 0;
}

int canopen_app_resetCommunication(void)
{
    /* CANopen communication reset - initialize CANopen objects */
    PRINTF("CANopenNode - Reset communication...\r\n");

    /* Wait rt_thread. */
    CO->CANmodule->CANnormal = false;

    /* Enter CAN configuration. */
    CO_CANsetConfigurationMode((void*)canopenNodeNXP);
    CO_CANmodule_disable(CO->CANmodule);

    /* initialize CANopen */
    err = CO_CANinit(CO, canopenNodeNXP, 0); // Bitrate for NXP microcontroller is being set in FlexCAN Settings
    if (err != CO_ERROR_NO)
    {
        PRINTF("Error: CAN initialization failed: %d\r\n", err);
        return 1;
    }

    CO_LSS_address_t lssAddress = {.identity = {.vendorID = OD_PERSIST_COMM.x1018_identity.vendor_ID,
                                                .productCode = OD_PERSIST_COMM.x1018_identity.productCode,
                                                .revisionNumber = OD_PERSIST_COMM.x1018_identity.revisionNumber,
                                                .serialNumber = OD_PERSIST_COMM.x1018_identity.serialNumber}};
    err = CO_LSSinit(CO, &lssAddress, &canopenNodeNXP->desiredNodeID, &canopenNodeNXP->baudrate);
    if (err != CO_ERROR_NO)
    {
        PRINTF("Error: LSS slave initialization failed: %d\r\n", err);
        return 2;
    }

    canopenNodeNXP->activeNodeID = canopenNodeNXP->desiredNodeID;
    uint32_t errInfo = 0;

    err = CO_CANopenInit(CO,                   /* CANopen object */
                         NULL,                 /* alternate NMT */
                         NULL,                 /* alternate em */
                         OD,                   /* Object dictionary */
                         OD_STATUS_BITS,       /* Optional OD_statusBits */
                         NMT_CONTROL,          /* CO_NMT_control_t */
                         FIRST_HB_TIME,        /* firstHBTime_ms */
                         SDO_SRV_TIMEOUT_TIME, /* SDOserverTimeoutTime_ms */
                         SDO_CLI_TIMEOUT_TIME, /* SDOclientTimeoutTime_ms */
                         SDO_CLI_BLOCK,        /* SDOclientBlockTransfer */
                         canopenNodeNXP->activeNodeID, &errInfo);
    if (err != CO_ERROR_NO && err != CO_ERROR_NODE_ID_UNCONFIGURED_LSS)
    {
        if (err == CO_ERROR_OD_PARAMETERS)
        {
            PRINTF("Error: Object Dictionary entry 0x%X\r\n", errInfo);
        }
        else
        {
            PRINTF("Error: CANopen initialization failed: %d\r\n", err);
        }
        return 3;
    }

#if (CO_CONFIG_NMT) & CO_CONFIG_NMT_CALLBACK_CHANGE
    CO_NMT_initCallbackChanged(CO->NMT, changedNMT_callback);
#endif

    err = CO_CANopenInitPDO(CO, CO->em, OD, canopenNodeNXP->activeNodeID, &errInfo);
    if (err != CO_ERROR_NO)
    {
        if (err == CO_ERROR_OD_PARAMETERS)
        {
            PRINTF("Error: Object Dictionary entry 0x%X\r\n", errInfo);
        }
        else
        {
            PRINTF("Error: PDO initialization failed: %d\r\n", err);
        }
        return 4;
    }

#if (CO_CONFIG_PDO) & CO_CONFIG_FLAG_CALLBACK_PRE
    CO_RPDO_initCallbackPre(&CO->RPDO[0], NULL, RPDO_calback);
#endif

    /* Set systick reload value to generate 1ms interrupt */
    SysTick_Config(SystemCoreClock / 1000U);
    /* Enable GPT interrupt and start GPT */
    gptStart_IT(&canopenNodeNXP->timerHandle);

    /* Configure CAN transmit and receive interrupt */

    /* Configure CANopen callbacks, etc */
    if (!CO->nodeIdUnconfigured)
    {
#if (CO_CONFIG_STORAGE) & CO_CONFIG_STORAGE_ENABLE
        if (storageInitError != 0)
        {
            CO_errorReport(CO->em, CO_EM_NON_VOLATILE_MEMORY, CO_EMC_HARDWARE, storageInitError);
        }
#endif
    }
    else
    {
        PRINTF("CANopenNode - Node-id not initialized\r\n");
    }

    /* start CAN */
    CO_CANsetNormalMode(CO->CANmodule);

    PRINTF("CANopenNode - Running...\r\n");
    time_old = time_current = GetTickCounter();
    return 0;
}

void canopen_app_process(void)
{
    static uint32_t testData = 0;

    testData++;
    OD_set_u32(OD_ENTRY_H6000, 1, testData, true);
    OD_set_u32(OD_ENTRY_H6002, 0, testData, true);

    if (g_reqFlag)
    {
        CO_TPDOsendRequest(&CO->TPDO[1]);
        g_reqFlag = 0;
    }
    /* loop for normal program execution */
    /* get time difference since last function call */
    time_current = GetTickCounter();

    if (time_current > time_old)
    {
        /* CANopen process */
        CO_NMT_reset_cmd_t reset_status;
        uint64_t timeDifference_us = (time_current - time_old) * 1000;
        time_old = time_current;
        reset_status = CO_process(CO, false, timeDifference_us, NULL);
        canopenNodeNXP->outStatusLEDRed = CO_LED_RED(CO->LEDs, CO_LED_CANopen);
        canopenNodeNXP->outStatusLEDGreen = CO_LED_GREEN(CO->LEDs, CO_LED_CANopen);

        if (reset_status == CO_RESET_COMM)
        {
            /* delete objects from memory */
            gptStop_IT(&canopenNodeNXP->timerHandle);
            CO_CANsetConfigurationMode((void*)canopenNodeNXP);
            CO_delete(CO);
            PRINTF("CANopenNode Reset Communication request\r\n");
            canopen_app_init(canopenNodeNXP);
        }
        else if (reset_status == CO_RESET_APP)
        {
            PRINTF("CANopenNode Device Reset\r\n");
            NVIC_SystemReset();
        }
    }
}

/* Thread function executes in constant intervals, this function can be called from FreeRTOS tasks or Timers */
void canopen_app_interrupt(void)
{
    CO_LOCK_OD(CO->CANmodule);
    if (!CO->nodeIdUnconfigured && CO->CANmodule->CANnormal)
    {
        bool_t syncWas = false;
        /* get time difference since last function call */
        uint32_t timeDifference_us = 1000;

#if (CO_CONFIG_SYNC) & CO_CONFIG_SYNC_ENABLE
        syncWas = CO_process_SYNC(CO, timeDifference_us, NULL);
#endif
#if (CO_CONFIG_PDO) & CO_CONFIG_RPDO_ENABLE
        CO_process_RPDO(CO, syncWas, timeDifference_us, NULL);
#endif
#if (CO_CONFIG_PDO) & CO_CONFIG_TPDO_ENABLE
        CO_process_TPDO(CO, syncWas, timeDifference_us, NULL);
#endif
        /* Further I/O or nonblocking application code may go here. */
    }
    CO_UNLOCK_OD(CO->CANmodule);
}

