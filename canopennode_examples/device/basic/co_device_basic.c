/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "app.h"
#include "CO_app_NXP.h"
#include "OD.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF
#define DLC (8)
#define CANopenDeviceID     0x04

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DEMO_CANInit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t g_rxBuffer[1] = {0};
CANopenNodeNXP canOpenNodeNXP;
extern CO_t* CO;
extern volatile bool rxBufferEmpty;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_CANInit(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_rx_fifo_config_t rxFifoConfig;
    uint8_t index;

    /* Init FlexCAN module. */
    /*
     * flexcanConfig.clkSrc                 = kFLEXCAN_ClkSrc0;
     * flexcanConfig.bitRate                = 1000000U;
     * flexcanConfig.bitRateFD              = 2000000U;
     * flexcanConfig.maxMbNum               = 16;
     * flexcanConfig.enableLoopBack         = false;
     * flexcanConfig.enableSelfWakeup       = false;
     * flexcanConfig.enableIndividMask      = false;
     * flexcanConfig.disableSelfReception   = false;
     * flexcanConfig.enableListenOnlyMode   = false;
     * flexcanConfig.enableDoze             = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

    flexcanConfig.bitRate = 500000U;
#if defined(EXAMPLE_CAN_CLK_SOURCE)
    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    flexcan_timing_config_t timing_config;
    memset(&timing_config, 0, sizeof(flexcan_timing_config_t));
    if (FLEXCAN_CalculateImprovedTimingValues(EXAMPLE_CAN, flexcanConfig.bitRate, EXAMPLE_CAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        LOG_INFO("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#endif

    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

    /* Accept non-matching standard/extended ID messages */
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, 0);

    /* Setup Tx Message Buffer. */
    for (index = TX_MB_FIRST_INDEX; index <= TX_MB_LAST_INDEX; index++)
    {
        FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, index, true);
    }

    /* Setup Legacy Rx FIFO. */
    rxFifoConfig.idFilterTable = NULL;
    rxFifoConfig.idFilterNum   = 0U;
    rxFifoConfig.idFilterType  = kFLEXCAN_RxFifoFilterTypeA;
    rxFifoConfig.priority      = kFLEXCAN_RxFifoPrioHigh;
    FLEXCAN_SetRxFifoConfig(EXAMPLE_CAN, &rxFifoConfig, true);

    FLEXCAN_SetRxFifoGlobalMask(EXAMPLE_CAN, FLEXCAN_RX_FIFO_STD_MASK_TYPE_A(0, 0, 0));
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    EXAMPLE_InitTimer();

    LOG_INFO("\r\n== CANopenNode device basic EXAMPLE ==\r\n");
    LOG_INFO(" - Press A Update Manager RPDO (0x1401) mapping parameter (0x2002)\r\n");

    LOG_INFO("\r\nPress any key to start CANopen\r\n");
    GETCHAR();

    canOpenNodeNXP.CANBasePtr     = EXAMPLE_CAN;
    canOpenNodeNXP.HWInitFunction = DEMO_CANInit;
    canOpenNodeNXP.baudrate       = 500;
    canOpenNodeNXP.desiredNodeID  = CANopenDeviceID;
    canOpenNodeNXP.startTimer     = EXAMPLE_StartTimer;
    canOpenNodeNXP.stopTimer      = EXAMPLE_StopTimer;
    canopen_app_init(&canOpenNodeNXP);

    OD_RAM.x2001_device_tx_var0 = 255;

    while (true)
    {
        canopen_app_process();

        EXAMPLE_UARTProcess();

        if (!rxBufferEmpty)
        {
            rxBufferEmpty = true;
            switch (g_rxBuffer[0])
            {
                case 'A':
                    LOG_INFO("Press A\r\n");
                    LOG_INFO("Trigger PDO Synchronous Acyclic communication\r\n");
                    OD_RAM.x2002_device_tx_var1++;
                    CO_TPDOsendRequest(&CO->TPDO[1]);
                    break;

                default:
                    LOG_INFO("Invalid Input \r\n");
                    break;
            }
        }
    }
}
