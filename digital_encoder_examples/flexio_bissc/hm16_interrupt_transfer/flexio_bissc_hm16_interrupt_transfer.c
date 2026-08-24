/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "clock_config.h"
#include "flexio_bissc.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_ENCODER_NAME           "HM16"
#define DEMO_BISSC_MA_CLOCK_COUNT   45U
#define DEMO_BISSC_RX_WINDOW_LENGTH 39U

/*******************************************************************************
 * Variables
 ******************************************************************************/

static FLEXIO_BISSC_Type g_bisscHandle = {
    .flexio                  = DEMO_BISSC_FLEXIO_BASE,
    .slPinIdx                = DEMO_BISSC_SL_PIN,
    .maPinIdx                = DEMO_BISSC_MA_PIN,
    .shifterStartIdx         = DEMO_BISSC_SHIFTER_START,
    .timerStartIdx          = DEMO_BISSC_TIMER_START,
    .hwTrigger              = (DEMO_BISSC_USE_HW_TRIGGER != 0U),
    .rxStartOnSlRisingEdge  = true,
    .baudRateBps             = DEMO_ENCODER_BIT_RATE,
    .mtLen                   = DEMO_ENCODER_MT_LEN,
    .stLen                   = DEMO_ENCODER_ST_LEN,
    .ackLen                  = DEMO_ENCODER_ACK_LEN,
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static bool DEMO_WaitBisscFrame(FLEXIO_BISSC_Type *base);
static void DEMO_PrintHeader(FLEXIO_BISSC_Type *base);
static void DEMO_PrintFrame(FLEXIO_BISSC_Type *base);

/*******************************************************************************
 * Code
 ******************************************************************************/

static bool DEMO_WaitBisscFrame(FLEXIO_BISSC_Type *base)
{
    uint32_t timeout = DEMO_BISSC_WAIT_TIMEOUT;

    while (timeout-- != 0U)
    {
        if (base->rxdBufferReady)
        {
            return true;
        }
    }

    return false;
}

static void DEMO_PrintHeader(FLEXIO_BISSC_Type *base)
{
    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    PRINTF("\r\nFlexIO %s %s demo on MCXA366\r\n", DEMO_ENCODER_PROTOCOL, DEMO_ENCODER_NAME);
    PRINTF("FlexIO clock: %u Hz\r\n", DEMO_BISSC_CLOCK_FREQUENCY);
    PRINTF("Bit rate: %u bps, MT: %u bits, ST: %u bits, ACK: %u bits\r\n", base->baudRateBps, base->mtLen,
           base->stLen, base->ackLen);
    PRINTF("MA: FLEXIO0_D%u, SL: FLEXIO0_D%u\r\n", base->maPinIdx, base->slPinIdx);
    PRINTF("RX start: SL rising edge, RX edge: rising, SL pull: off\r\n");
    PRINTF("MA clocks: %u, RX window: %u bits\r\n\r\n", DEMO_BISSC_MA_CLOCK_COUNT,
           DEMO_BISSC_RX_WINDOW_LENGTH);
}

static void DEMO_PrintFrame(FLEXIO_BISSC_Type *base)
{
    PRINTF("MT=%u, ST=%u, ERR=%u, WARN=%u, CRC=%s\r\n", base->mt, base->st, base->errorBit, base->warnBit,
           base->crcMatch ? "OK" : "FAIL");
}

int main(void)
{
    FLEXIO_BISSC_Type *base = &g_bisscHandle;

    BOARD_InitHardware();
    DEMO_PrintHeader(base);

    if (FLEXIO_BISSC_Init(base, DEMO_BISSC_CLOCK_FREQUENCY) != 0)
    {
        PRINTF("FlexIO BiSS-C init failed. Check FlexIO clock and encoder bit rate.\r\n");
        while (1)
        {
        }
    }

#if (DEMO_BISSC_USE_INTERRUPT != 0U)
    NVIC_ClearPendingIRQ(DEMO_BISSC_FLEXIO_IRQ);
    (void)EnableIRQ(DEMO_BISSC_FLEXIO_IRQ);
    FLEXIO_BISSC_EnableRxInterrupt(base, true);
#endif

    while (1)
    {
        base->rxdBufferReady = false;
        FLEXIO_BISSC_SwTrigger(base);

#if (DEMO_BISSC_USE_INTERRUPT != 0U)
        if (!DEMO_WaitBisscFrame(base))
        {
            PRINTF("BiSS-C frame timeout\r\n");
            SDK_DelayAtLeastUs(DEMO_BISSC_FRAME_INTERVAL_US, SystemCoreClock);
            continue;
        }
        base->rxdBufferReady = false;
#else
        FLEXIO_BISSC_ReadBlocking(base);
#endif

        FLEXIO_BISSC_DataParser(base);
        DEMO_PrintFrame(base);

        SDK_DelayAtLeastUs(DEMO_BISSC_FRAME_INTERVAL_US, SystemCoreClock);
    }
}

void FLEXIO_IRQHandler(void)
{
    FLEXIO_BISSC_IRQHandler();
}
