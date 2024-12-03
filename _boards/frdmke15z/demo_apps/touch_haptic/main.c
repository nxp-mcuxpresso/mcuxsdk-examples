/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

#include "fsl_ftm.h"
#include "fsl_lpit.h"

#include "main.h"
#include "freemaster.h"
#include "freemaster_serial_lpuart.h"
#include "fsl_lpuart.h"
#include "pin_mux.h"
#include "board.h"

#include "../source/filters/nt_filters_prv.h"
#include "ui_control.h"

extern void TSI_DRV_IRQHandler(uint32_t instance);

static void pit_init(void);
/*******************************************************************************
 * Functions
 ******************************************************************************/

static void init_freemaster_lpuart(void);
static void keypad_callback(const struct nt_control *control, enum nt_control_keypad_event event, uint32_t index);

static void aslider_callback(const struct nt_control *control, enum nt_control_aslider_event event, uint32_t position);

static void arotary_callback(const struct nt_control *control, enum nt_control_arotary_event event, uint32_t position);

/* Call when the TSI counter overflows 65535 */
static void system_callback(uint32_t event, union nt_system_event_context *context);

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define N_SENSORS_MAX 30     // For mem alloc

#define nt_printf(...) /* do nothing - the debug lines are used by FreeMASTER */

/*******************************************************************************
 * Types
 ******************************************************************************/

 /******************************************************************************
 * Variables
 ******************************************************************************/

#if defined(__ICCARM__)
uint8_t nt_memory_pool[4000]; /* IAR EWARM compiler */
#else
uint8_t nt_memory_pool[4000] __attribute__((aligned(4))); /* Keil, GCC compiler */
#endif

tsi_status_t recalib_status;

const lpit_config_t my_pit_config = {
    .enableRunInDebug = false, /*!< Timer doesn't run in debug mode*/
    .enableRunInDoze  = false, /*!< Timer doesn't run in doze mode*/
};

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
    bool recalib_enabled = false; /* autotunning is off */
    bool one_key_only    = false; /* one key only valid is off */

    /* Init board hardware */
    BOARD_InitPins();

    BOARD_BootClockRUN();

    /* Initialize the OS abstraction layer */
    NT_OSA_Init();

    /* FreeMASTER communication layer initialization */
    init_freemaster_lpuart();

    /* FreeMASTER initialization */
    (void)FMSTR_Init();

    if ((result = nt_init(&System_0, nt_memory_pool, sizeof(nt_memory_pool))) != NT_SUCCESS)
    {
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
    while(1); /* add code to handle this error */
    }

    nt_printf("\nNXP Touch is successfully initialized.\n");

    nt_printf("Unused memory: %d bytes, you can make the memory pool smaller without affecting the functionality.\n",
              (int)nt_mem_get_free_size());

    /* Enable electrodes and controls */
    nt_enable();

/* Disable FRDM-TOUCH board electrodes and controls if FRDM-TOUCH board is not connected */
#if (NT_FRDM_TOUCH_SUPPORT == 0)
    nt_electrode_disable(&El_3);
    nt_electrode_disable(&El_4);
    nt_electrode_disable(&El_5);
    nt_electrode_disable(&El_6);
    nt_electrode_disable(&El_7);
    nt_electrode_disable(&El_8);
    nt_electrode_disable(&El_9);
    nt_electrode_disable(&El_10);
    nt_electrode_disable(&El_11);
    nt_electrode_disable(&El_12);
#endif

    /* Keypad electrodes*/
    nt_control_keypad_set_autorepeat_rate(&Keypad_1, 100, 1000);
    nt_control_keypad_register_callback(&Keypad_1, &keypad_callback);

    /* Slider electrodes */
    nt_control_aslider_register_callback(&ASlider_2, &aslider_callback);

    /* Rotary electrodes */
    nt_control_arotary_register_callback(&ARotary_3, &arotary_callback);

    /* System TSI overflow warning callback */
    nt_system_register_callback(&system_callback);
#if (NT_SAFETY_SUPPORT == 1)
    /* Auto TSI register recalibration function, not used as default */
    if (recalib_enabled)
    {
        recalib_status = (tsi_status_t)nt_module_recalibrate(&nt_tsi_module);
    }
#endif /* NT_SAFETY_SUPPORT */
    if (one_key_only)
    {
        nt_control_keypad_only_one_key_valid(&Keypad_1, true);
    }

    
    NVIC_SetPriority( s_TsiIRQ[0], 2 );
    pit_init();
    
    ui_control_init();
    
    while (1)
    {
        nt_task();

        /* The FreeMASTER poll call must be called in the main application loop
        to handle the communication interface and protocol.
           In the LONG_INTR FreeMASTER interrupt mode, all the processing is done
        during the communication interrupt routine and the FMSTR_Poll() is
        compiled empty. */
        FMSTR_Poll();
        
        /* User interface to process audio handling */
        ui_control_main();
    }
}

void LPIT0_IRQHandler(void)
{
    /* Clear the interrupt flag.*/
    LPIT_ClearStatusFlags(LPIT0, LPIT_MSR_TIF0_MASK);
    nt_trigger();
}

void TSI_IRQHandler(void)
{
    TSI_DRV_IRQHandler(0);
}

void pit_init(void)
{
    /* Set the source for the LPIT module */
    CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcSircAsync);

    /* Init PIT */
    LPIT_Init(LPIT0, &my_pit_config);

    /* Set PIT0 period 5ms */
    LPIT_SetTimerPeriod(LPIT0, kLPIT_Chnl_0,  nt_kernel_data.rom->time_period * CLOCK_GetFreq(kCLOCK_ScgSircAsyncDiv2Clk) / 1000);

    /* Enable the PIT interrupt */
    LPIT_EnableInterrupts(LPIT0, LPIT_MIER_TIE0_MASK);
    EnableIRQ(LPIT0_IRQn);
    NVIC_SetPriority( LPIT0_IRQn, 1 );

    /* Start the PIT timer */
    LPIT_StartTimer(LPIT0, kLPIT_Chnl_0);
}

void keypad_callback(const struct nt_control *control, enum nt_control_keypad_event event, uint32_t index)
{
    /* Convert basic NT event ( touch/release ) to UI event ( touch/release/long_press/hold/lock/... ) */
    switch (event)
    {
        case NT_KEYPAD_RELEASE:
            ui_control_keypad_event(UI_BUTTONS_KEYPAD_RELEASE, index, 0);
            switch (index)
            {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                default:
                    break;
            }
            break;
        
        case NT_KEYPAD_TOUCH:
          
            ui_control_keypad_event(UI_BUTTONS_KEYPAD_TOUCH, index, 0);
            switch (index)
            {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                default:
                    break;
            }
            
            break;

        case NT_KEYPAD_AUTOREPEAT:

            switch (index)
            {
                case 0:
                    ui_control_keypad_event(UI_BUTTONS_KEYPAD_AUTOREPEAT, index, 0);
                    break;
                case 1:
                    ui_control_keypad_event(UI_BUTTONS_KEYPAD_AUTOREPEAT, index, 0);
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                default:
                    break;
            }             
            break;

        case NT_KEYPAD_MULTI_TOUCH:

            switch (index)
            {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
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
            ui_control_slider_event(UI_BUTTONS_SLIDER_TOUCH, position, 0);
            previousPos = position;
            break;
        case NT_ASLIDER_MOVEMENT:
            /* Position change is greater then slider position insensitivity value */
            if (_nt_abs_int32((int32_t)previousPos - (int32_t)position) > sliderPosInsens)
            {
                ui_control_slider_event(UI_BUTTONS_SLIDER_TOUCH, position, 0);
                previousPos = position;
            }
            break;
        case NT_ASLIDER_ALL_RELEASE:
            break;
        default:
            break;
    }
}

static void arotary_callback(const struct nt_control *control, enum nt_control_arotary_event event, uint32_t position)
{
    switch (event)
    {
        case NT_AROTARY_MOVEMENT:
            /* Position change is greater then rotary position insensitivity value */
            if( _nt_abs_int32((int32_t)previousPos - (int32_t)position) > rotaryPosInsens )
            {
                ui_control_rotary_event(UI_BUTTONS_ROTARY_TOUCH, position, 0);
                previousPos = position;
            }
            break;
        case NT_AROTARY_ALL_RELEASE:
            ui_control_rotary_event(UI_BUTTONS_ROTARY_RELEASE, position, 0);
            break;
        case NT_AROTARY_INITIAL_TOUCH:
            ui_control_rotary_event(UI_BUTTONS_ROTARY_TOUCH, position, 0);
            previousPos = position;
            break;
        default:
            break;
    }
}

/* Call on the TSI CNTR overflow 16-bit range (65535) */
void system_callback(uint32_t event, union nt_system_event_context *context)
{
    switch (event)
    {
        case NT_SYSTEM_EVENT_OVERRUN:
            // your code
          /* red colour signalize the error, to solve it increase nt_kernel_data.rom->time_period  */
        break;
        case NT_SYSTEM_EVENT_DATA_READY:
            /* your code */
        break;
        case NT_SYSTEM_EVENT_MODULE_DATA_READY:
            /* your code */
        break;
        case NT_SYSTEM_EVENT_DATA_OVERFLOW:
            /* your code */
        break;
        case NT_SYSTEM_EVENT_SIGNAL_LOW:
            /* your code */
        break;
        case NT_SYSTEM_EVENT_SIGNAL_HIGH:
            /* your code */
        break;
        case NT_SYSTEM_EVENT_ELEC_SHORT_VDD:
            /* your code */
        break;
        case NT_SYSTEM_EVENT_ELEC_SHORT_GND:
            /* your code */
        break;
        case NT_SYSTEM_EVENT_ELEC_SHORT_ADJ:
            /* your code */
        break;
    }
}

/*!
 * @brief LPUART Module initialization
 */
void init_freemaster_lpuart(void)
{
    lpuart_config_t config;

    CLOCK_SetIpSrc(kCLOCK_Lpuart1, BOARD_DEBUG_UART_CLKSRC);

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

    LPUART_Init((LPUART_Type*)BOARD_DEBUG_UART_BASEADDR, &config, CLOCK_GetIpFreq(kCLOCK_Lpuart1));

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((LPUART_Type*)BOARD_DEBUG_UART_BASEADDR);

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