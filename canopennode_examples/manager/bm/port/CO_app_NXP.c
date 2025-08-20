/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "CO_app_NXP.h"
#include "OD.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* default values for CO_CANopenInit() */
#define NMT_CONTROL          (CO_NMT_STARTUP_TO_OPERATIONAL | CO_NMT_ERR_ON_ERR_REG | \
                              CO_ERR_REG_GENERIC_ERR | CO_ERR_REG_COMMUNICATION)
#define FIRST_HB_TIME        500
#define SDO_SRV_TIMEOUT_TIME 1000
#define SDO_CLI_TIMEOUT_TIME 500
#define SDO_CLI_BLOCK        false
#define OD_STATUS_BITS       NULL

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* It will be set by canopen_app_init and will be used across app to get access to CANOpen objects */
CANopenNodeNXP *canopenNodeNXP;

/* Global variables and objects */
CO_t *CO = NULL; /* CANopen object */

/* Global variables */
uint64_t time_old, time_current;
CO_ReturnError_t err;
volatile uint64_t g_systickCounter = 0;

struct {
    uint8_t changedNMT: 1;
    uint8_t sdoClient: 1;
    uint8_t bootup: 1;
    uint8_t timeoutHB: 1;
    uint8_t changedNMT_HB: 1;
    uint8_t rpdo: 1;
    uint8_t rpdoEvent: 1;
} callbackFlags_g = {0};

struct {
    uint8_t cmd;
    uint16_t index;
    uint8_t subIndex;
    uint16_t value;
} sdoValues_g = {0};

CO_NMT_internalState_t localState, remoteState;
uint32_t testData_g, eventData_g;
int counterSYNC = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
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
        return "Initializing  ";
    case CO_NMT_PRE_OPERATIONAL:
        return "PreOperational";
    case CO_NMT_OPERATIONAL:
        return "Operational   ";
    case CO_NMT_STOPPED:
        return "Stopped       ";
    default:
        return "Unknown       ";
    }
}

#if (CO_CONFIG_NMT) & CO_CONFIG_NMT_CALLBACK_CHANGE
void changedNMT_callback(CO_NMT_internalState_t state)
{
    callbackFlags_g.changedNMT = 1;
    localState = state;
}
#endif

#if (CO_CONFIG_SDO_CLI) & CO_CONFIG_FLAG_CALLBACK_PRE
void signalSDOclient_callback(void *object)
{
    callbackFlags_g.sdoClient = 1;
    sdoValues_g.cmd      = CO->SDOclient->CANrxData[0];
    sdoValues_g.index    = *(uint16_t *)&CO->SDOclient->CANrxData[1];
    sdoValues_g.subIndex = CO->SDOclient->CANrxData[3];
    sdoValues_g.value    = *(uint16_t *)&CO->SDOclient->CANrxData[4];
}
#endif

void bootup_callback(uint8_t nodeId, uint8_t idx, void *object)
{
    callbackFlags_g.bootup = 1;
}

void heartbeat_callback(uint8_t nodeId, uint8_t idx, void *object)
{
}

void HBtimeout_callback(uint8_t nodeId, uint8_t idx, void *object)
{
    callbackFlags_g.timeoutHB = 1;
}

void HBchangedNMT_callback(uint8_t nodeId, uint8_t idx,
                           CO_NMT_internalState_t NMTstate,
                           void *object)
{
    callbackFlags_g.changedNMT_HB = 1;
    remoteState = NMTstate;
}

void RPDO_calback(void *object)
{
    uint8_t bufNo = 0;
    uint32_t rpdoIndex = (uint32_t)object;

    switch (rpdoIndex)
    {
    case 0:
        callbackFlags_g.rpdo = 1;
        if (CO->RPDO[0].SYNC->CANrxToggle) bufNo = 1;
        testData_g = *(uint32_t *)CO->RPDO[0].CANrxData[bufNo];
        break;

    case 1:
        callbackFlags_g.rpdoEvent = 1;
        if (CO->RPDO[1].SYNC->CANrxToggle) bufNo = 1;
        eventData_g = *(uint32_t *)CO->RPDO[1].CANrxData[bufNo];
        break;

    default:
        break;
    }
}

void initAllCallbackFunctions(void)
{
#if (CO_CONFIG_NMT) & CO_CONFIG_NMT_CALLBACK_CHANGE
    CO_NMT_initCallbackChanged(CO->NMT, changedNMT_callback);
#endif

#if (CO_CONFIG_SDO_CLI) & CO_CONFIG_FLAG_CALLBACK_PRE
    CO_SDOclient_initCallbackPre(CO->SDOclient, NULL, signalSDOclient_callback);
#endif

#if (CO_CONFIG_HB_CONS) & CO_CONFIG_HB_CONS_CALLBACK_MULTI
    for (int index = 0; index < OD_CNT_ARR_1016; index++)
    {
        CO_HBconsumer_initCallbackRemoteReset(CO->HBcons, index, NULL, bootup_callback);
        CO_HBconsumer_initCallbackHeartbeatStarted(CO->HBcons, index, NULL, heartbeat_callback);
	CO_HBconsumer_initCallbackTimeout(CO->HBcons, index, NULL, HBtimeout_callback);
        CO_HBconsumer_initCallbackNmtChanged(CO->HBcons, index, NULL, HBchangedNMT_callback);
    }
#endif

#if (CO_CONFIG_PDO) & CO_CONFIG_FLAG_CALLBACK_PRE
    CO_RPDO_initCallbackPre(&CO->RPDO[0], (void *)0, RPDO_calback);
    CO_RPDO_initCallbackPre(&CO->RPDO[1], (void *)1, RPDO_calback);
#endif
}

void updateTable(void)
{
    if (CO->SYNC->counter != counterSYNC)
    {
        counterSYNC = CO->SYNC->counter;
        PRINTF("\033[s\033[15A\033[36C%03d\033[u", counterSYNC);
    }

    if (callbackFlags_g.changedNMT)
    {
        PRINTF("\033[s\033[17A\033[14C%s\033[u", getNMT_string(localState));
        callbackFlags_g.changedNMT = 0;
    }

    if (callbackFlags_g.sdoClient)
    {
        PRINTF("\033[s\033[1;33mSDO data: [%02Xh][%04Xh][%d][%04Xh]\033[0m       \033[u",
               sdoValues_g.cmd, sdoValues_g.index, sdoValues_g.subIndex, sdoValues_g.value);
        callbackFlags_g.sdoClient = 0;
    }

    if (callbackFlags_g.bootup)
    {
        PRINTF("\033[sThe device [2] has started!            \033[u");
        callbackFlags_g.bootup = 0;
    }

    if (callbackFlags_g.timeoutHB)
    {
        PRINTF("\033[sThe device [2] has timed out!          \033[u");
        callbackFlags_g.timeoutHB = 0;
    }

    if (callbackFlags_g.changedNMT_HB)
    {
        PRINTF("\033[s\033[17A\033[34C%s\033[u", getNMT_string(remoteState));
        PRINTF("\033[sThe state of the device [2] is changed!\033[u");
        callbackFlags_g.changedNMT_HB = 0;
    }

    if (callbackFlags_g.rpdo)
    {
        PRINTF("\033[s\033[13A\033[36C0x%08X\033[u", testData_g);
        callbackFlags_g.rpdo = 0;
    }

    if (callbackFlags_g.rpdoEvent)
    {
        PRINTF("\033[s\033[11A\033[36C0x%08X\033[u", eventData_g);
        callbackFlags_g.rpdoEvent = 0;
    }
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
    err = CO_CANinit(CO, canopenNodeNXP, 0);
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

    /* Initializing all callbacks */
    initAllCallbackFunctions();

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
    updateTable();

    /* loop for normal program execution */
    /* get time difference since last function call */
    time_current = GetTickCounter();

    if (time_current > time_old)
    {
        /* CANopen process */
        CO_NMT_reset_cmd_t reset_status;
        uint64_t timeDifference_us = (time_current - time_old) * 1000;
        time_old = time_current;
        reset_status = CO_process(CO, true, timeDifference_us, NULL);
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

