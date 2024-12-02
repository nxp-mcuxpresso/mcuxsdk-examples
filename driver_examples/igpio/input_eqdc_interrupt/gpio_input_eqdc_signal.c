/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
static void delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Whether the SW is turned on */
volatile bool g_InputSignal = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Interrupt service fuction of switch.
 */
void EXAMPLE_GPIO_IRQHandler(void)
{
    /* clear the interrupt status */
    GPIO_PortClearInterruptFlags(EXAMPLE_SW_GPIO, 1U << EXAMPLE_SW_GPIO_PIN);
    /* Generate index pulse signal, positive pulse width 30ms */
    GPIO_PortToggle(EXAMPLE_EQDC_GPIO, 1u << EXAMPLE_EQDC_INDEX_PIN);
    delay(); 
    GPIO_PortToggle(EXAMPLE_EQDC_GPIO, 1u << EXAMPLE_EQDC_INDEX_PIN);
    /* Change state of switch. */
    g_InputSignal = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Define the init structure for the output signal*/
    gpio_pin_config_t phaseA_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    gpio_pin_config_t phaseB_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    gpio_pin_config_t index_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {kGPIO_DigitalInput, 0, kGPIO_IntRisingEdge};

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Print a note to terminal. */
    PRINTF("\r\n GPIO Driver example\r\n");
    PRINTF("\r\n Out put signals for EQDC module.\r\n");

    /* Init output signal GPIO. */
    GPIO_PinInit(EXAMPLE_EQDC_GPIO, EXAMPLE_EQDC_PHASEA_PIN, &phaseA_config);
    GPIO_PinInit(EXAMPLE_EQDC_GPIO, EXAMPLE_EQDC_PHASEB_PIN, &phaseB_config);
    GPIO_PinInit(EXAMPLE_EQDC_GPIO, EXAMPLE_EQDC_INDEX_PIN, &index_config);

    /* Init input switch GPIO. */
    EnableIRQ(EXAMPLE_SW_IRQ);
    GPIO_PinInit(EXAMPLE_SW_GPIO, EXAMPLE_SW_GPIO_PIN, &sw_config);

    /* Enable GPIO pin interrupt */
    GPIO_PortEnableInterrupts(EXAMPLE_SW_GPIO, 1U << EXAMPLE_SW_GPIO_PIN);
    
    while (1)
    {
        /* Generate Phase A waveform, period 400ms */
        SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);    
        GPIO_PortToggle(EXAMPLE_EQDC_GPIO, 1u << EXAMPLE_EQDC_PHASEA_PIN);
        
        /* Generate Phase B waveform, period 400ms, delay 100ms */
        SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        GPIO_PortToggle(EXAMPLE_EQDC_GPIO, 1u << EXAMPLE_EQDC_PHASEB_PIN);
        
        /* Generate INDEX interrupt, positive pulse width 30ms */
         if (g_InputSignal)
        {
            delay();
            if (1 == GPIO_PinRead(EXAMPLE_SW_GPIO, EXAMPLE_SW_GPIO_PIN))
            {
                PRINTF(" %s is turned on, INDEX generated.\r\n", EXAMPLE_SW_NAME);
            }
            /* Reset state of switch. */
            g_InputSignal = false;
        } 
    }
}

static void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __NOP(); /* delay */
    }
}