/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "app.h"
#include "fsl_lpuart.h"
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

#define TOTAL_LINE_NUMBER     4
#define SAVE_POS              "\033[s"
#define REVERT_POS            "\033[u"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void can_HWInit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
CANopenNodeNXP canOpenNodeNXP = {
    .desiredNodeID = 1,
    .baudrate      = 1000,
    .timerHandle   = {
        .base      = EXAMPLE_GPT,
        .gptChMask = kGPT_OutputCompare1InterruptEnable,
        .gptIRQid  = GPT_IRQ_ID
    },
    .CANHandle      = EXAMPLE_CAN,
    .HWInitFunction = can_HWInit,
};

static char canopenList[] = "\r\nPlease select a test case by pressing the \033[1;33mup or down key\033[0m on your keyboard:\r\n"
                            "****************************************************\r\n"
                            "*          |  CANopen Manager  |  CANopen Device   *\r\n"
                            "****************************************************\r\n"
                            "* Node-Id  |  0x01             |  0x02             *\r\n"
                            "* BaudRate |  1000K            |  1000K            *\r\n"
                            "* State    |  Unknown          |  Unknown          *\r\n"
                            "****************************************************\r\n"
                            "* Sync Message Counter (1 time/s) | 0              *\r\n"
                            "****************************************************\r\n"
                            "* SYNC PDO Data     (Per 5 SYNCs) | -              *\r\n"
                            "****************************************************\r\n"
                            "* Event PDO Data    (Test case 4) | -              *\r\n"
                            "****************************************************\r\n"
                            "*                    Test cases                    *\r\n"
                            "****************************************************\r\n"
                            "* [\033[1;33m>\033[0m] 1. Start CANopen device           (NMT Test) *\r\n"
                            "* [ ] 2. Set device to pre-operational  (NMT Test) *\r\n"
                            "* [ ] 3. Read 'Heartbeat producer time' (SDO Test) *\r\n"
                            "* [ ] 4. Manually obtain the PDO data   (PDO Test) *\r\n"
                            "****************************************************\r\n"
                            "*                       Log                        *\r\n"
                            "****************************************************\r\n";

volatile uint16_t rxIndex;
volatile uint8_t keyFlag = 0;
volatile uint8_t line = 0;
volatile uint8_t runCMD = 0;

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

void EXAMPLE_LPUART_IRQHandler(void)
{
    uint8_t data;

    /* If new data arrived. */
    if ((kLPUART_RxDataRegFullFlag) & LPUART_GetStatusFlags(EXAMPLE_LPUART))
    {
        data = LPUART_ReadByte(EXAMPLE_LPUART);

        switch (data)
        {
        case 0x1B:
            if (rxIndex == 0)
                rxIndex++;
            else
                rxIndex = 0;
            break;

        case 0x5B:
            if (rxIndex == 1)
                rxIndex++;
            else
                rxIndex = 0;
            break;

        case 'A':
            if (rxIndex == 2)
                keyFlag = 1;
            rxIndex = 0;
            break;

        case 'B':
            if (rxIndex == 2)
                keyFlag = 2;
            rxIndex = 0;
            break;

        case '\r':
        case '\n':
            runCMD++;
            break;

        default:
            rxIndex = 0;
            keyFlag = 0;
            break;
        }
    }
    SDK_ISR_EXIT_BARRIER;
}

void checkKeyboard(void)
{
    if ((keyFlag == 1) || (keyFlag == 2))
    {
        PRINTF(SAVE_POS"\033[%dA\033[3C ", (TOTAL_LINE_NUMBER - line) + 3);
        if (keyFlag == 1)
        {
            if (line > 0)
            {
                line--;
                PRINTF("\033[1A\033[1D\033[1;33m>\033[0m");
            }
            else
            {
                line = TOTAL_LINE_NUMBER - 1;
                PRINTF("\033[%dB\033[1D\033[1;33m>\033[0m", line);
            }
        }
        else if (keyFlag == 2)
        {
            if (line == (TOTAL_LINE_NUMBER - 1))
            {
                PRINTF("\033[%dA\033[1D\033[1;33m>\033[0m", line);
                line = 0;
            }
            else
            {
                line++;
                PRINTF("\033[1B\033[1D\033[1;33m>\033[0m");
            }
        }
        keyFlag = 0;
        PRINTF(REVERT_POS);
    }
    else if (runCMD > 0)
    {
        switch (line)
        {
        case 0:
            CO_GTWA_write(canOpenNodeNXP.canOpenStack->gtwa, "[1] 2 start\r\n", 13);
            PRINTF(SAVE_POS"The command [1] is executed!           "REVERT_POS);
            break;

        case 1:
            CO_GTWA_write(canOpenNodeNXP.canOpenStack->gtwa, "[1] 2 preop\r\n", 13);
            PRINTF(SAVE_POS"The command [2] is executed!           "REVERT_POS);
            break;

        case 2:
            CO_GTWA_write(canOpenNodeNXP.canOpenStack->gtwa, "[1] 2 read 0x1017 0 i16\r\n", 25);
            PRINTF(SAVE_POS"The command [3] is executed!           "REVERT_POS);
            break;

        case 3:
            CO_TPDOsendRequest(&canOpenNodeNXP.canOpenStack->TPDO[0]);
            PRINTF(SAVE_POS"The command [4] is executed!           "REVERT_POS);
            break;

        default:
            break;
        }
        runCMD = 0;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Enable RX interrupt. */
    LPUART_EnableInterrupts(EXAMPLE_LPUART, kLPUART_RxDataRegFullInterruptEnable);
    EnableIRQ(EXAMPLE_LPUART_IRQn);

    gpt_timerInit(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, EXAMPLE_GPT_CLK_FREQ / 1000);

    LOG_INFO("\r\n==\033[1;33mCANopenNode manager\033[0m example -- Start.==\r\n\r\n");

    canopen_app_init(&canOpenNodeNXP);

    PRINTF("%s", canopenList);

    while (true)
    {
        checkKeyboard();
        canopen_app_process();
    }
}

