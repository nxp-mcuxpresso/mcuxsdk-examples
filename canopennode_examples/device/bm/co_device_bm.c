/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "app.h"
#include "CO_app_NXP.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF

#if (defined(USE_CANFD) && USE_CANFD)
/*
 *    DWORD_IN_MB    DLC    BYTES_IN_MB             Maximum MBs
 *    2              8      kFLEXCAN_8BperMB        64
 *    4              10     kFLEXCAN_16BperMB       42
 *    8              13     kFLEXCAN_32BperMB       25
 *    16             15     kFLEXCAN_64BperMB       14
 *
 * Dword in each message buffer, Length of data in bytes, Payload size must align,
 * and the Message Buffers are limited corresponding to each payload configuration:
 */
#define DWORD_IN_MB (16)
#define DLC         (15)
#define BYTES_IN_MB kFLEXCAN_64BperMB
#else
#define DLC (8)
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_GPT_IRQHandler(void)
{
    if (GPT_GetStatusFlags(EXAMPLE_GPT, kGPT_OutputCompare1Flag))
    {
        /* Clear interrupt flag.*/
        GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_OutputCompare1Flag);
        canopen_app_interrupt();
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
     * exception return operation might vector to incorrect interrupt
     */
    SDK_ISR_EXIT_BARRIER;
}

void can_HWInit(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;
    int index;

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

#if defined(EXAMPLE_CAN_CLK_SOURCE)
    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    flexcan_timing_config_t timing_config;
    memset(&timing_config, 0, sizeof(flexcan_timing_config_t));
#if (defined(USE_CANFD) && USE_CANFD)
    if (FLEXCAN_FDCalculateImprovedTimingValues(EXAMPLE_CAN, flexcanConfig.bitRate, flexcanConfig.bitRateFD,
                                                EXAMPLE_CAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        LOG_INFO("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#else
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
#endif

#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_FDInit(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ, BYTES_IN_MB, true);
#else
    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);
#endif

    /*
     * Configure global filter that is used as last check if message did not pass any of other filters:
     *
     * We do not rely on hardware filters in this example
     * and are performing software filters instead
     *
     * Accept non-matching standard/extended ID messages
     */

    /* Set Rx Masking mechanism. */
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, 0);

    /* Setup Tx Message Buffer. */
    for (index = TX_MB_FIRST_INDEX; index <= TX_MB_LAST_INDEX; index++)
    {
        FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, index, true);
    }

    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard;
    mbConfig.type   = kFLEXCAN_FrameTypeData;
    mbConfig.id     = FLEXCAN_ID_STD(0);
    for (index = RX_MB_FIRST_INDEX; index <= RX_MB_LAST_INDEX; index++)
    {
        FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, index, &mbConfig, true);
    }
}

/*!
 * @brief Main function
 */
extern flexcan_handle_t flexcanHandle;
int main(void)
{
    CANopenNodeNXP canOpenNodeNXP = {
        .desiredNodeID = 2,
        .baudrate      = 1000,
        .timerHandle   = {
            .base      = EXAMPLE_GPT,
            .gptChMask = kGPT_OutputCompare1InterruptEnable,
            .gptIRQid  = GPT_IRQ_ID
        },
        .CANHandle      = EXAMPLE_CAN,
        .HWInitFunction = can_HWInit,
    };

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    gpt_timerInit(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, EXAMPLE_GPT_CLK_FREQ / 1000);

    LOG_INFO("\r\n==\033[1;33mCANopenNode device\033[0m example -- Start.==\r\n\r\n");

    canopen_app_init(&canOpenNodeNXP);

    while (true)
    {
        canopen_app_process();
    }
}

