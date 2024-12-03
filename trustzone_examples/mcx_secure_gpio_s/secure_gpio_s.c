/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if (__ARM_FEATURE_CMSE & 1) == 0
#error "Need ARMv8-M security extensions"
#elif (__ARM_FEATURE_CMSE & 2) == 0
#error "Compile with --cmse"
#endif

#include "fsl_device_registers.h"
#include "arm_cmse.h"
#include "board.h"
#include "veneer_table.h"
#include "tzm_config.h"
#include "tzm_api.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NON_SECURE_START DEMO_CODE_START_NS

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* The TrustZone should be configured as early as possible after RESET.
     * Therefore it is called from SystemInit() during startup. The SystemInitHook() weak function
     * overloading is used for this purpose.
     */
    BOARD_InitTrustZone();
}

/*!
 * @brief SysTick Handler
 */
void SysTick_Handler(void)
{
    /* Enable usage of PIN DEMO_SW2 and DEMO_LED_RED in Nonsecure world if BUTTON 1 is pressed*/

    /* Control GPIO MASK based on SW1 button press */
    if (GPIO_PinRead(DEMO_SW1_GPIO, DEMO_SW1_GPIO_PIN))
    {
        GPIO_DisablePinControlNonSecure(DEMO_SW2_GPIO, DEMO_SW2_GPIO_NSE_MASK);
        GPIO_DisablePinControlNonSecure(DEMO_LED_RED_GPIO, DEMO_LED_RED_GPIO_NSE_MASK);
        LED_BLUE_OFF();

        /* If LED RED is turned ON in Normal world and later is disabled access to pin for LED RED from Secure world,
         * LED will remain ON without this because code in Normal world cannot read state of pin */
        LED_RED_OFF();
    }
    else
    {
        /* Enable usage of SW2 button from non secure world*/
        GPIO_EnablePinControlNonSecure(DEMO_SW2_GPIO, DEMO_SW2_GPIO_NSE_MASK);
        GPIO_EnablePinControlNonSecure(DEMO_LED_RED_GPIO, DEMO_LED_RED_GPIO_NSE_MASK);
        LED_BLUE_ON();
    }
}
/*!
 * @brief Main function
 */
int main(void)
{
    /* Board init */
    BOARD_InitHardware();

    /* Initialize GPIO pin for button S1. */
    GPIO_PinInit(DEMO_SW1_GPIO, DEMO_SW1_GPIO_PIN, &(gpio_pin_config_t){kGPIO_DigitalInput, 0});

    /* Initialize GPIO pin for button S2. */
    GPIO_PinInit(DEMO_SW2_GPIO, DEMO_SW2_GPIO_PIN, &(gpio_pin_config_t){kGPIO_DigitalInput, 0});

    /* Initialize GPIO PINS for RGB LED*/
    LED_RED_INIT(0x0U);
    LED_BLUE_INIT(0x0U);

    /* Set systick reload value to generate 5ms interrupt */
    SysTick_Config(USEC_TO_COUNT(5000U, DEMO_SYSTICK_CLK_FREQ));

    /* Call non-secure application - jump to normal world */
    TZM_JumpToNormalWorld(NON_SECURE_START);

    while (1)
    {
        /* This point should never be reached */
    }
}
