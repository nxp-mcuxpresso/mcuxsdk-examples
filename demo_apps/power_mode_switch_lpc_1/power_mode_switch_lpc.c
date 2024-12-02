/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_common.h"
#include "fsl_power.h"
#include "fsl_inputmux.h"
#include "fsl_pint.h"
#include "fsl_usart.h"
#include "fsl_gint.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
const char *g_wakeupInfoStr[] = {"Sleep [Press the user key to wakeup]", "Deep Sleep [Press the user key to wakeup]",
                                 "Powerdown [Reset to wakeup]", "Deep Powerdown [Reset to wakeup]"};
uint32_t g_currentPowerMode;

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static uint32_t APP_GetUserSelection(void);
static void APP_InitWakeupPin(void);
static void pint_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    /* Running 12 MHz to Core*/
    APP_RUNNING_INTERNAL_CLOCK;

    /* Attach Main Clock as CLKOUT */
    CLOCK_AttachClk(kMAIN_CLK_to_CLKOUT);

    APP_InitWakeupPin();

    PRINTF("Power Manager Demo for LPC device.\r\n");
    PRINTF("The \"user key\" is: %s\r\n", APP_USER_WAKEUP_KEY_NAME);

    while (1)
    {
        g_currentPowerMode = APP_GetUserSelection();
        PRINTF("Entering %s ...\r\n", g_wakeupInfoStr[g_currentPowerMode]);

        /* Enter the low power mode. */
        switch (g_currentPowerMode)
        {
            case kPmu_Sleep: /* Enter sleep mode. */
                POWER_EnterSleep();
                break;
            case kPmu_Deep_Sleep: /* Enter deep sleep mode. */
                POWER_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP, 0x0, APP_WAKEUP_FROM_DEEPSLEEP, 0x0);
                break;
            case kPmu_PowerDown: /* Enter power down mode. */
                PRINTF(
                    "Press any key to confirm to enter the power down mode and wakeup the device by press sw3 key on "
                    "board.\r\n\r\n");
                GETCHAR();
                DEMO_PreLowPower();
                POWER_EnterPowerDown(APP_EXCLUDE_FROM_POWERDOWN, kPOWER_SRAM_PDWN_MASK, APP_WAKEUP_FROM_POWERDOWN,
                                     0x20000000);
                DEMO_PowerDownWakeup();
                APP_InitWakeupPin();
                break;
            case kPmu_Deep_PowerDown: /* Enter deep power down mode. */
                PRINTF(
                    "Press any key to confirm to enter the deep power down mode and wakeup the device by "
                    "reset.\r\n\r\n");
                GETCHAR();
                DEMO_PreDeepPowerDown();
                POWER_EnterDeepPowerDown(APP_EXCLUDE_FROM_DEEPPOWERDOWN, kPOWER_SRAM_DPWD_MASK,
                                         APP_WAKEUP_FROM_DEEPPOWERDOWN, 0);
                break;
            default:
                break;
        }

        PRINTF("Wakeup.\r\n");
    }
}

/*!
 * @brief Call back for GINT0 event
 */
void gint0_callback(void)
{
    PRINTF("Gin event occurs\r\n");
}

/*
 * Setup a GPIO input pin as wakeup source.
 */
static void APP_InitWakeupPin(void)
{
    gpio_pin_config_t gpioPinConfigStruct;

    /* Set SW pin as GPIO input. */
    gpioPinConfigStruct.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(APP_USER_WAKEUP_KEY_GPIO, APP_USER_WAKEUP_KEY_PORT, APP_USER_WAKEUP_KEY_PIN, &gpioPinConfigStruct);

    /* Configure the Input Mux block and connect the trigger source to PinInt channle. */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, APP_USER_WAKEUP_KEY_INPUTMUX_SEL); /* Using channel 0. */
    INPUTMUX_Deinit(INPUTMUX); /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */

    /* Configure the interrupt for SW pin. */
    PINT_Init(PINT);
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge, pint_intr_callback);
    PINT_EnableCallback(PINT); /* Enable callbacks for PINT */

    /* Enable wakeup for PinInt0. */
    EnableDeepSleepIRQ(PIN_INT0_IRQn); /* GPIO pin interrupt 0 wake-up. */

    /* Initialize GINT0*/
    GINT_Init(GINT0);

    /* Setup GINT0 for edge trigger, "OR" mode */
    GINT_SetCtrl(GINT0, kGINT_CombineOr, kGINT_TrigEdge, gint0_callback);

    /* Select pins & polarity for GINT0 */
    GINT_ConfigPins(GINT0, DEMO_GINT0_PORT, DEMO_GINT0_POL_MASK, DEMO_GINT0_ENA_MASK);

    /* Enable callbacks for GINT0 & GINT1 */
    GINT_EnableCallback(GINT0);
}

/*
 * Callback function when wakeup key is pressed.
 */
static void pint_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    PRINTF("Pin event occurs\r\n");
}

/*
 * Get user selection from UART.
 */
static uint32_t APP_GetUserSelection(void)
{
    uint32_t ch;

    /* Clear rx overflow error once it happens during low power mode. */
    if (APP_USART_RX_ERROR == (APP_USART_RX_ERROR & USART_GetStatusFlags((USART_Type *)BOARD_DEBUG_UART_BASEADDR)))
    {
        USART_ClearStatusFlags((USART_Type *)BOARD_DEBUG_UART_BASEADDR, APP_USART_RX_ERROR);
    }

    PRINTF(
        "Select an option\r\n"
        "\t1. Sleep mode\r\n"
        "\t2. Deep Sleep mode\r\n"
        "\t3. power down mode\r\n"
        "\t4. Deep power down mode\r\n");
    while (1)
    {
        ch = GETCHAR();
        if ((ch < '1') || (ch > '4')) /* Only '1', '2', '3' , '4'. */
        {
            continue;
        }
        else
        {
            ch = ch - '1'; /* Only 0, 1, 2 (,3). */
            break;
        }
    }
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
            break;
    }
    return ch;
}
