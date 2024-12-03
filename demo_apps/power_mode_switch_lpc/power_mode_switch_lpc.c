/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_common.h"
#include "fsl_power.h"
#include "fsl_usart.h"
#include "fsl_syscon.h"
#include "fsl_wkt.h"
#include "fsl_iocon.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

static power_mode_cfg_t s_CurrentPowerMode;
static uint32_t s_CurrentWakeupSource;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static power_mode_cfg_t DEMO_GetUserSelection(void);
static uint32_t DEMO_GetWakeUpSource(power_mode_cfg_t targetPowerMode);

/*******************************************************************************
 * Code
 ******************************************************************************/
void WKT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    WKT_ClearStatusFlags(WKT, kWKT_AlarmFlag);
    WKT_StartTimer(WKT, USEC_TO_COUNT(DEMO_WKT_TIMEOUT_VALUE, DEMO_WKT_CLK_FREQ));
}

static uint32_t DEMO_GetUserInput(uint32_t maxChoice)
{
    uint32_t ch = 0U;

    while (1)
    {
        ch = GETCHAR();
        if ((ch < '1') || (ch > (maxChoice + '0')))
        {
            continue;
        }
        else
        {
            ch = ch - '1';
            break;
        }
    }

    return ch;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("Power mode switch Demo for LPC8xx.\r\n");

    while (1)
    {
        s_CurrentPowerMode    = DEMO_GetUserSelection();
        s_CurrentWakeupSource = DEMO_GetWakeUpSource(s_CurrentPowerMode);

        /* prepare to enter low power mode */
        DEMO_PreEnterLowPower();

        /* Enter the low power mode. */
        switch (s_CurrentPowerMode)
        {
            case kPmu_Sleep: /* Enter sleep mode. */
                POWER_EnterSleep();
                break;
            case kPmu_Deep_Sleep: /* Enter deep sleep mode. */
                POWER_EnterDeepSleep(DEMO_ACTIVE_IN_DEEPSLEEP);
                break;
            case kPmu_PowerDown: /* Enter deep sleep mode. */
                POWER_EnterPowerDown(DEMO_ACTIVE_IN_DEEPSLEEP);
                break;
            case kPmu_Deep_PowerDown: /* Enter deep power down mode. */
                POWER_EnterDeepPowerDownMode();
                break;
            default:
                break;
        }

        /* restore the active mode configurations */
        DEMO_LowPowerWakeup();
        PRINTF("Wakeup.\r\n");

        if (s_CurrentWakeupSource == DEMO_WAKEUP_CASE_WKT)
        {
            WKT_StopTimer(WKT);
        }
    }
}

static uint32_t DEMO_GetWakeUpSource(power_mode_cfg_t targetPowerMode)
{
    uint32_t ch = 0U;

    switch (targetPowerMode)
    {
        case kPmu_Sleep:
            PRINTF(DEMO_SLEEP_WAKEUP_SOURCE);
            ch = DEMO_GetUserInput(DEMO_SLEEP_WAKEUP_SOURCE_SIZE);
            break;
        case kPmu_Deep_Sleep:
            PRINTF(DEMO_DEEP_SLEEP_WAKEUP_SOURCE);
            ch = DEMO_GetUserInput(DEMO_DEEP_SLEEP_WAKEUP_SOURCE_SIZE);
            break;
        case kPmu_PowerDown:
            PRINTF(DEMO_POWER_DOWN_WAKEUP_SOURCE);
            ch = DEMO_GetUserInput(DEMO_POWER_DOWN_WAKEUP_SOURCE_SIZE);
            break;
        case kPmu_Deep_PowerDown:
            PRINTF(DEMO_DEEP_POWERDOWN_WAKEUP_SOURCE);
            ch = DEMO_GetUserInput(DEMO_DEEP_POWERDOWN_WAKEUP_SOURCE_SIZE);
            break;
    }

    switch (ch)
    {
        case DEMO_WAKEUP_CASE_WKT:
            /* Enable wakeup for wkt. */
            EnableDeepSleepIRQ(WKT_IRQn);
            DisableDeepSleepIRQ(PIN_INT0_IRQn);
            WKT_StartTimer(WKT, USEC_TO_COUNT(DEMO_WKT_TIMEOUT_VALUE, DEMO_WKT_CLK_FREQ));
            break;
        case DEMO_WAKEUP_CASE_WAKEUP:
            /* Enable wakeup for PinInt0. */
            EnableDeepSleepIRQ(PIN_INT0_IRQn);
            DisableDeepSleepIRQ(WKT_IRQn);
            if (targetPowerMode == kPmu_Deep_PowerDown)
            {
                POWER_DPD_ENABLE_WAKEUP_PIN
            }
            break;
        case DEMO_WAKEUP_CASE_RESET:
            if (targetPowerMode == kPmu_Deep_PowerDown)
            {
                POWER_DPD_ENABLE_RESET_PIN
            }
            break;
    }

    return ch;
}

/*
 * Get user selection from UART.
 */
static power_mode_cfg_t DEMO_GetUserSelection(void)
{
    uint32_t ch = 0U;

    PRINTF(
        "\r\nSelect an option\r\n"
        "\t1. Sleep mode\r\n"
        "\t2. Deep Sleep mode\r\n"
        "\t3. Power Down mode\r\n"
        "\t4. Deep power down mode\r\n");

    ch = DEMO_GetUserInput(4);

    switch (ch)
    {
        case 0:
            ch = kPmu_Sleep;
            break;
        case 1:
            ch = kPmu_Deep_Sleep;
            break;
        case 2:
            ch = kPmu_PowerDown;
            break;
        case 3:
            ch = kPmu_Deep_PowerDown;
        default:
            break;
    }
    return (power_mode_cfg_t)ch;
}
