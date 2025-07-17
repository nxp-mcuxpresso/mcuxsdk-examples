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
#define NMT_ERROR            (CO_ERR_REG_GENERIC_ERR | CO_ERR_REG_COMMUNICATION)
#define NMT_CONTROL          (CO_NMT_ERR_ON_ERR_REG | NMT_ERROR)
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
uint32_t time_old, time_current;
CO_ReturnError_t err;
volatile uint32_t g_systickCounter = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void)
{
    g_systickCounter++;
}

static uint32_t GetTickCounter(void)
{
    return g_systickCounter;
}

/* This function will basically setup the CANopen node */
int canopen_app_init(CANopenNodeNXP *_canopenNodeNXP)
{
    uint32_t heapMemoryUsed;
    /* Allocate memory */
    CO_config_t *config_ptr = NULL;
    /* Keep a copy global reference of canOpenNXP Object */
    canopenNodeNXP = _canopenNodeNXP;

    CO = CO_new(config_ptr, &heapMemoryUsed);

    if (CO == NULL)
    {
        PRINTF("Error: Can't allocate memory\r\n\r\n");
        return 1;
    }
    else
    {
        PRINTF("\r\nAllocated %u bytes for CANopen objects\r\n", heapMemoryUsed);
    }

    canopenNodeNXP->canOpenStack = CO;

    canopen_app_resetCommunication();
    return 0;
}

int canopen_app_resetCommunication(void)
{
    uint32_t errInfo = 0;

    /* CANopen communication reset - initialize CANopen objects */
    PRINTF("CANopenNode - Reset communication...\r\n");

    /* Wait rt_thread. */
    CO->CANmodule->CANnormal = false;

    canopenNodeNXP->HWInitFunction();

    /* Initialize CANopen, bitrate for NXP microcontroller is set in FlexCAN settings. */
    err = CO_CANinit(CO, canopenNodeNXP, 0);
    if (err != CO_ERROR_NO)
    {
        PRINTF("Error: CAN initialization failed: %d\r\n\r\n", err);
        return 1;
    }

    CO_LSS_address_t lssAddress = {.identity = {.vendorID = OD_PERSIST_COMM.x1018_identity.vendor_ID,
                                                .productCode = OD_PERSIST_COMM.x1018_identity.productCode,
                                                .revisionNumber = OD_PERSIST_COMM.x1018_identity.revisionNumber,
                                                .serialNumber = OD_PERSIST_COMM.x1018_identity.serialNumber}};
    err = CO_LSSinit(CO, &lssAddress, &canopenNodeNXP->desiredNodeID, &canopenNodeNXP->baudrate);
    if (err != CO_ERROR_NO)
    {
        PRINTF("Error: LSS slave initialization failed: %d\r\n\r\n", err);
        return 2;
    }

    canopenNodeNXP->activeNodeID = canopenNodeNXP->desiredNodeID;

    err = CO_CANopenInit(CO,                                /* CANopen object */
                         NULL,                              /* alternate NMT */
                         NULL,                              /* alternate em */
                         OD,                                /* Object dictionary */
                         OD_STATUS_BITS,                    /* Optional OD_statusBits */
                         (CO_NMT_control_t)NMT_CONTROL,     /* CO_NMT_control_t */
                         FIRST_HB_TIME,                     /* firstHBTime_ms */
                         SDO_SRV_TIMEOUT_TIME,              /* SDOserverTimeoutTime_ms */
                         SDO_CLI_TIMEOUT_TIME,              /* SDOclientTimeoutTime_ms */
                         SDO_CLI_BLOCK,                     /* SDOclientBlockTransfer */
                         canopenNodeNXP->activeNodeID, 
                         &errInfo);
    if (err != CO_ERROR_NO && err != CO_ERROR_NODE_ID_UNCONFIGURED_LSS)
    {
        if (err == CO_ERROR_OD_PARAMETERS)
        {
            PRINTF("Error: Object Dictionary entry 0x%X\r\n\r\n", errInfo);
        }
        else
        {
            PRINTF("Error: CANopen initialization failed: %d\r\n\r\n", err);
        }
        return 3;
    }

    err = CO_CANopenInitPDO(CO, CO->em, OD, canopenNodeNXP->activeNodeID, &errInfo);
    if (err != CO_ERROR_NO)
    {
        if (err == CO_ERROR_OD_PARAMETERS)
        {
            PRINTF("Error: Object Dictionary entry 0x%X\r\n\r\n", errInfo);
        }
        else
        {
            PRINTF("Error: PDO initialization failed: %d\r\n\r\n", err);
        }
        return 4;
    }

    /* Set systick reload value to generate 1ms interrupt */
    SysTick_Config(SystemCoreClock / 1000U);
    /* Enable Timer interrupt and start Timer */
    canopenNodeNXP->startTimer();

    /* Configure CAN transmit and receive interrupt */

    /* Configure CANopen callbacks, etc */
    if (CO->nodeIdUnconfigured)
    {
        PRINTF("CANopenNode - Node-id not initialized\r\n");
    }

    /* start CAN */
    CO_CANsetNormalMode(CO->CANmodule);

    PRINTF("CANopenNode - Running...\r\n\r\n");

    time_old = time_current = GetTickCounter();
    return 0;
}

void canopen_app_process(void)
{
    /* 
     * Loop for normal program execution.
     * Get time difference since last function call.
     * User can use peripheral Timer to get time difference in us.
     * In this case, user should calculate time difference based on Timer counter frequency.
     */
    time_current = GetTickCounter();

    if ((time_current - time_old) > 0)
    {
        /* CANopen process */
        CO_NMT_reset_cmd_t reset_status;
        uint32_t timeDifference_us = (time_current - time_old) * 1000;
        time_old = time_current;
        reset_status = CO_process(CO, false, timeDifference_us, NULL);
        canopenNodeNXP->outStatusLEDRed = CO_LED_RED(CO->LEDs, CO_LED_CANopen);
        canopenNodeNXP->outStatusLEDGreen = CO_LED_GREEN(CO->LEDs, CO_LED_CANopen);

        if (reset_status == CO_RESET_COMM)
        {
            /* delete objects from memory */
            canopenNodeNXP->stopTimer();
            FLEXCAN_Deinit(canopenNodeNXP->CANBasePtr);
            PRINTF("\r\nCANopenNode Reset Communication request\r\n");
            canopen_app_resetCommunication();
        }
        else if (reset_status == CO_RESET_APP)
        {
            PRINTF("\r\nCANopenNode Device Reset\r\n");
            NVIC_SystemReset();
        }
        else
        {
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

