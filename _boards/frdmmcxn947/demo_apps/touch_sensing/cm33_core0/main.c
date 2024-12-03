/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include "fsl_device_registers.h"

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "fsl_lpuart.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "main.h"
#include "freemaster.h"
#include "freemaster_serial_lpuart.h"

static void CTIMERInit(void);
static void init_freemaster_lpuart(void);
static void keypad_callback(const struct nt_control *control, enum nt_control_keypad_event event, uint32_t index);

static void aslider_callback(const struct nt_control *control, enum nt_control_aslider_event event, uint32_t position);

static void arotary_callback(const struct nt_control *control, enum nt_control_arotary_event event, uint32_t position);

/* Call when the TSI counter overflows 65535 */
static void system_callback(uint32_t event, union nt_system_event_context *context);

#if defined(__ICCARM__)
uint8_t nt_memory_pool[3700]; /* IAR EWARM compiler */
#else
uint8_t nt_memory_pool[3700] __attribute__((aligned(4))); /* Keil, GCC compiler */
#endif

/*
 * This list describes all TSA tables that should be exported to the
 * FreeMASTER application.
 */
#ifndef FMSTR_PE_USED
FMSTR_TSA_TABLE_LIST_BEGIN()
FMSTR_TSA_TABLE(nt_frmstr_tsa_table)
FMSTR_TSA_TABLE_LIST_END()
#endif

#define nt_printf(...) /* do nothing - the debug lines are used by FreeMASTER */

int main(void)
{ 
    int32_t result;
    bool one_key_only    = false; /* one key only valid is off */

    /* Init board hardware */
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();

    /*LED init*/
    LED_RED_INIT(LOGIC_LED_OFF);
    LED_GREEN_INIT(LOGIC_LED_OFF);
    LED_BLUE_INIT(LOGIC_LED_OFF);
    
    /* Initialize the OS abstraction layer */
    NT_OSA_Init();

    /* FreeMASTER communication layer initialization */
    init_freemaster_lpuart();

    /* FreeMASTER initialization */
    (void)FMSTR_Init();

    if ((result = nt_init(&System_0, nt_memory_pool, sizeof(nt_memory_pool))) != NT_SUCCESS)
    {
        /* red colour signalizes the error, to solve is increase nt_memory_pool or debug it */ 
        LED_RED_ON();
                        
        switch (result)
        {
            case NT_FAILURE:
                nt_printf("\nCannot initialize NXP Touch due to a non-specific error.\n");
                break;
            case NT_OUT_OF_MEMORY:
                nt_printf("\nCannot initialize NXP Touch due to a lack of free memory.\n");
                printf("\nCannot initialize NXP Touch due to a non-specific error.\n");
                break;
        }
        while (1); /* add code to handle this error */
    }
    /* Get free memory size of the nt_memory_pool  */
    volatile uint32_t free_mem;
    free_mem = nt_mem_get_free_size();
 
    nt_printf("\nNXP Touch is successfully initialized.\n");
    nt_printf("Unused memory: %d bytes, you can make the memory pool smaller without affecting the functionality.\n",
              free_mem);
    printf("Unused memory: %d bytes, you can make the memory pool smaller without affecting the functionality.\n",
          (int)free_mem);

    /* Enable electrodes and controls */
    nt_enable();

/* Disable FRDM-TOUCH board electrodes and controls if FRDM-TOUCH board is not connected */
#if (NT_FRDM_TOUCH_SUPPORT) == 0
    nt_electrode_disable(&El_2);
    nt_electrode_disable(&El_3);
    nt_electrode_disable(&El_4);
    nt_electrode_disable(&El_5);

    nt_electrode_disable(&El_6);
    nt_electrode_disable(&El_7);
    nt_electrode_disable(&El_8);
    nt_electrode_disable(&El_9);
    nt_electrode_disable(&El_10);
    nt_electrode_disable(&El_11);
#endif
    /* Keypad electrodes*/
    nt_control_keypad_set_autorepeat_rate(&Keypad_1, 100, 1000);
    nt_control_keypad_register_callback(&Keypad_1, &keypad_callback);

    /* Slider electrodes */
    nt_control_aslider_register_callback(&ASlider_2, &aslider_callback);    

    /* Rotary electrodes */
    nt_control_arotary_register_callback(&ARotary_3, &arotary_callback);
    
    if (one_key_only)
        nt_control_keypad_only_one_key_valid(&Keypad_1, true);


    /* System TSI overflow warning callback */
    nt_system_register_callback(&system_callback);

    CTIMERInit();
    
    while (1)
    {
        nt_task();

        /* The FreeMASTER poll call must be called in the main application loop
        to handle the communication interface and protocol.
           In the LONG_INTR FreeMASTER interrupt mode, all the processing is done
        during the communication interrupt routine and the FMSTR_Poll() is
        compiled empty. */
        FMSTR_Poll();
    }
}

void CTIMER0_IRQHandler(void)
{
    /* Clear the interrupt flag.*/
    nt_trigger();

    /* Clear the match interrupt flag. */
    CTIMER0->IR |= CTIMER_IR_MR0INT(1U);
    
    /* Add empty instructions for correct interrupt flag clearing */
    __DSB();
    __ISB();
}

void TSI_END_OF_SCAN_DriverIRQHandler(void)
{
    TSI_DRV_IRQHandler(0);
}
void TSI_OUT_OF_SCAN_DriverIRQHandler(void)
{
    TSI_DRV_IRQHandler(0);
}

static void CTIMERInit(void)
{
    /* Use 96 MHz clock for some of the Ctimer0. */
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);
    
    /* Enable Timer0 clock. */
    SYSCON->AHBCLKCTRLSET[1] |= SYSCON_AHBCLKCTRL1_TIMER0_MASK;

    /* Enable Timer0 clock reset. */
    SYSCON->PRESETCTRLSET[1] = SYSCON_PRESETCTRL1_TIMER0_RST_MASK;             /* Set bit. */
    while (0u == (SYSCON->PRESETCTRL1 & SYSCON_PRESETCTRL1_TIMER0_RST_MASK))   /* Wait until it reads 0b1 */
    {
    }
    
    /* Clear Timer0 clock reset. */                                  
    SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL1_TIMER0_RST_MASK;             /* Clear bit */
    while (SYSCON_PRESETCTRL1_TIMER0_RST_MASK ==                               /* Wait until it reads 0b0 */     
          (SYSCON->PRESETCTRL1 & SYSCON_PRESETCTRL1_TIMER0_RST_MASK))
    {
    }

    /* Configure match control register. */
    CTIMER0->MCR |= CTIMER_MCR_MR0R(1U)  |   /* Enable reset of TC after it matches with MR0. */
                    CTIMER_MCR_MR0I(1U);     /* Enable interrupt generation after TC matches with MR0. */
    
    /* Configure match register. */
    CTIMER0->MR[0] = (nt_kernel_data.rom->time_period * CLOCK_GetFreq(kCLOCK_FroHf))  /* Get CTimer0 frequency for correct set Match register value. */
                     / 1000;                 /* Set slow control loop frequency in Hz. */
    
    /* Configure interrupt register. */
    CTIMER0->IR = CTIMER_IR_MR0INT_MASK;     /* Set interrupt flag for match channel 0. */
    NVIC_SetPriority(CTIMER0_IRQn, 1U);
    NVIC_EnableIRQ(CTIMER0_IRQn);            /* Enable LEVEL1 interrupt and update the call back function. */

    /* Configure timer control register. */
    CTIMER0->TCR |= CTIMER_TCR_CEN_MASK;     /* Start the timer counter. */
}

static void keypad_callback(const struct nt_control *control, enum nt_control_keypad_event event, uint32_t index)
{
    switch (event)
    {
        case NT_KEYPAD_RELEASE:
            {  
            LED_RED_OFF();
            LED_GREEN_OFF();
            LED_BLUE_OFF();
            }
            break;
        case NT_KEYPAD_TOUCH:

            switch (index)
            {
                case 0:
                    /* WHILE on, full brightness */
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_ON();
                break;
            case 1:
                /* WHILE on, full brightness */
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_ON();
                break;
            case 2:
                /* RED on, full brightness */
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
                break;
            case 3:
                /* BLUE on, full brightness */
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_ON();
                break;
            case 4:
                /* GREEN on, full brightness */
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_OFF();
                break;
            default:
                    break;
            }
            break;

        case NT_KEYPAD_AUTOREPEAT:

            break;

        case NT_KEYPAD_MULTI_TOUCH:
            switch (index)
            {
            case 0:
                    /* BLUE + RED on, full brightness */
                    LED_RED_ON();
                    LED_GREEN_OFF();
                    LED_BLUE_ON();
                    break;
                case 1:
                    /* GREEN + BLUE on, full brightness */
                    LED_RED_OFF();
                    LED_GREEN_ON();
                    LED_BLUE_ON();
                    break;
                case 2:
                    /* GREEN + RED on, full brightness */
                    LED_RED_ON();
                    LED_GREEN_ON();
                    LED_BLUE_OFF();
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
}

static void aslider_callback(const struct nt_control *control, enum nt_control_aslider_event event, uint32_t position)
{
  switch (event)
    {
        case NT_ASLIDER_INITIAL_TOUCH:
            nt_printf("\n Touch: %d", position);
            if (position < 20)
            {
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 20) && (position < 40))
            {
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 40) && (position < 60))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 60) && (position < 80))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_ON();
            }
            else if (position >= 80)
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_ON();
            }
            break;
        case NT_ASLIDER_MOVEMENT:
            nt_printf("\n Movement: %d", position);
            if (position < 20)
            {
                LED_RED_TOGGLE();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 20) && (position < 40))
            {
                LED_RED_TOGGLE();
                LED_GREEN_TOGGLE();
                LED_BLUE_OFF();
            }
            else if ((position >= 40) && (position < 60))
            {
                LED_RED_OFF();
                LED_GREEN_TOGGLE();
                LED_BLUE_OFF();
            }
            else if ((position >= 60) && (position < 80))
            {
                LED_RED_OFF();
                LED_GREEN_TOGGLE();
                LED_BLUE_TOGGLE();
            }
            else if (position >= 80)
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_TOGGLE();
            }
            break;
        case NT_ASLIDER_ALL_RELEASE:
            nt_printf("\n Release: %d", position);
            LED_RED_OFF();
            LED_GREEN_OFF();
            LED_BLUE_OFF();
            break;
    }
}

static void arotary_callback(const struct nt_control *control, enum nt_control_arotary_event event, uint32_t position)
{
  switch (event)
    {
        case NT_AROTARY_INITIAL_TOUCH:
            nt_printf("\n Touch: %d", position);
            if (position < 25)
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 25) && (position < 50))
            {
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 50) && (position < 75))
            {
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 75) && (position < 100))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 100) && (position < 125))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_ON();
            }
            else if ((position >= 125) && (position < 150))
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_ON();
            }    
            else if ((position >= 150) && (position < 175))
            {
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_ON();
            }
            else if (position >= 175)
            {
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_ON();
            }
            break;
        case NT_AROTARY_MOVEMENT:
            nt_printf("\n Movement: %d", position);
            if (position < 25)
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 25) && (position < 50))
            {
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 50) && (position < 75))
            {
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 75) && (position < 100))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 100) && (position < 125))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_ON();
            }
            else if ((position >= 125) && (position < 150))
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_ON();
            }    
            else if ((position >= 150) && (position < 175))
            {
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_ON();
            }
            else if (position >= 175)
            {
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_ON();
            }
            break;
        case NT_AROTARY_ALL_RELEASE:
            nt_printf("\n Release: %d", position);
            break;
    }

    /* Recalculate and set RGB LED */
}

/* Call on the TSI CNTR overflow 16-bit range (65535) */
void system_callback(uint32_t event, union nt_system_event_context *context)
{
    switch (event)
    {
        case NT_SYSTEM_EVENT_OVERRUN:
        {
            /* red colour signalize the error, to solve it increase nt_kernel_data.rom->time_period  */
            LED_RED_ON();
            nt_printf("\n Overrun occurred increase nt_kernel_data.rom->time_period param \n");
            printf("\n Overrun occurred increase nt_kernel_data.rom->time_period param \n");

        }
        case NT_SYSTEM_EVENT_DATA_READY:
            // your code
        break;
        case NT_SYSTEM_EVENT_MODULE_DATA_READY:
            // your code
        break;
        case NT_SYSTEM_EVENT_DATA_OVERFLOW:
            // your code
        break;
    }
}

/*!
 * @brief LPUART Module initialization (LPUART is a the standard block included e.g. in K66F)
 */
static void init_freemaster_lpuart(void)
{
    lpuart_config_t config;

    /*
     * config.baudRate_Bps = 19200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = 19200U;
    config.enableTx     = false;
    config.enableRx     = false;

    LPUART_Init((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR, &config, BOARD_DEBUG_UART_CLK_FREQ);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(BOARD_UART_IRQ);
    EnableGlobalIRQ(0);
#endif
}

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
/*
*   Application interrupt handler of communication peripheral used in interrupt modes
*   of FreeMASTER communication.
*
*   NXP MCUXpresso SDK framework defines interrupt vector table as a part of "startup_XXXXXX.x"
*   assembler/C file. The table points to weakly defined symbols, which may be overwritten by the
*   application specific implementation. FreeMASTER overrides the original weak definition and
*   redirects the call to its own handler.
*
*/

void BOARD_UART_IRQ_HANDLER(void)
{
    /* Call FreeMASTER Interrupt routine handler */
    FMSTR_SerialIsr();
}
#endif