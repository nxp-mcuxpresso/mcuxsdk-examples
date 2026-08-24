/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "clock_config.h"
#include "flexio_endat2.h"
#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_flexio.h"
#include "fsl_reset.h"
#include "pin_mux.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
static FLEXIO_ENDAT2_Type s_endat2Handle = {
    .flexio          = DEMO_FLEXIO_INSTANCE,
    .txdPinIdx       = DEMO_FLEXIO_TXD_CHANNEL,
    .rxdPinIdx       = DEMO_FLEXIO_RXD_CHANNEL,
    .clkPinIdx       = DEMO_FLEXIO_CLK_CHANNEL,
    .dirPinIdx       = DEMO_FLEXIO_DIR_CHANNEL,
    .shifterStartIdx = 0U,
    .timerStartIdx   = 0U,
    .hwTrigger       = false,
    .baudRateBps     = DEMO_ENCODER_BIT_RATE,
    .mtLen           = DEMO_ENCODER_MT_LEN,
    .stLen           = DEMO_ENCODER_ST_LEN,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void DEMO_InitFlexioClock(void)
{
    CLOCK_AttachClk(DEMO_FLEXIO_CLOCK_SOURCE);
    CLOCK_SetClockDiv(kCLOCK_DivFLEXIO0, DEMO_FLEXIO_CLOCK_DIV);
    CLOCK_EnableClock(kCLOCK_GateFLEXIO0);
    RESET_ReleasePeripheralReset(kFLEXIO0_RST_SHIFT_RSTn);
}

void FLEXIO_IRQHandler(void)
{
    FLEXIO_ENDAT2_IRQHandler();
}

int main(void)
{
    uint32_t sampleCount = 0U;
    int      status;

    BOARD_InitHardware();
    DEMO_InitFlexioClock();

    PRINTF("\r\nMCXA366 FlexIO %s demo\r\n", DEMO_ENCODER_PROTOCOL);
    PRINTF("Encoder: %s, MT=%u, ST=%u, bitrate=%u bps\r\n", DEMO_ENCODER_NAME, DEMO_ENCODER_MT_LEN,
           DEMO_ENCODER_ST_LEN, DEMO_ENCODER_BIT_RATE);
    PRINTF("FlexIO clock: %u Hz\r\n", DEMO_FLEXIO_CLOCK_FREQ);
    PRINTF("Pins: TXD=D%u, RXD=D%u, CLK=D%u, DIR=D%u\r\n", DEMO_FLEXIO_TXD_CHANNEL, DEMO_FLEXIO_RXD_CHANNEL,
           DEMO_FLEXIO_CLK_CHANNEL, DEMO_FLEXIO_DIR_CHANNEL);

    FLEXIO_Reset(s_endat2Handle.flexio);

    status = FLEXIO_ENDAT2_Init(&s_endat2Handle, DEMO_FLEXIO_CLOCK_FREQ);
    if (status != 0)
    {
        PRINTF("FLEXIO_ENDAT2_Init failed: %d\r\n", status);
        while (1)
        {
        }
    }

    FLEXIO_ENDAT2_CmdPreset(&s_endat2Handle, kFlexIO_ENDAT2_cmd_idx_EncSendPosVal);

#if (DEMO_WAIT_RESULTS == DEMO_WAIT_RESULTS_INTERRUPT)
    FLEXIO_ENDAT2_EnableRxInterrupt(&s_endat2Handle, true);
    EnableIRQ(DEMO_FLEXIO_IRQ_NUMBER);
#endif

    while (1)
    {
        FLEXIO_ENDAT2_SwTrigger(&s_endat2Handle);

#if (DEMO_WAIT_RESULTS == DEMO_WAIT_RESULTS_INTERRUPT)
        while (!s_endat2Handle.rxdBufferReady)
        {
        }
        s_endat2Handle.rxdBufferReady = false;
#else
        status = FLEXIO_ENDAT2_ReadBlocking(&s_endat2Handle);
        if (status != 0)
        {
            PRINTF("RX timeout\r\n");
            while (1)
            {
            }
        }
#endif

        status = FLEXIO_ENDAT2_DataParser(&s_endat2Handle);

        if (status == 0)
        {
            if ((sampleCount++ % DEMO_PRINT_DECIMATION) == 0U)
            {
                if (s_endat2Handle.crcMatch)
                {
                    PRINTF("MT=%u ST=%u ERR1=%u CRC=OK\r\n", (unsigned int)s_endat2Handle.mt,
                           (unsigned int)s_endat2Handle.st, s_endat2Handle.error1Bit);
                }
                else
                {
                    PRINTF("MT=%u ST=%u ERR1=%u CRC=FAIL\r\n", (unsigned int)s_endat2Handle.mt,
                           (unsigned int)s_endat2Handle.st, s_endat2Handle.error1Bit);
                }
            }
        }
        else if ((sampleCount++ % DEMO_PRINT_DECIMATION) == 0U)
        {
            PRINTF("No EnDat2 start bit detected\r\n");
        }

        SDK_DelayAtLeastUs(DEMO_SAMPLE_PERIOD_US, SystemCoreClock);
    }
}
