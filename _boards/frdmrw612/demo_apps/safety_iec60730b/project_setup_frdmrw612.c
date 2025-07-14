/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_frdmrw612.h"

#include "freemaster.h"
#include "freemaster_serial.h"
#include "freemaster_serial_usart.h"

#include "board.h"
#include "fsl_cache.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief   Initialization of CTIMER0
 *
 *          This function initializes the CTIMER0. CTIMER0 is used for Clock test.
 *
 * @param   void
 *
 * @return  None
 */
void CTIMER0_Init(void)
{  
    /* Init hardware*/
    /* Use 16 MHz clock for the Ctimer0 */
    CLOCK_AttachClk(kSFRO_to_CTIMER0);

    #if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
        /* Enable the timer clock*/
        CLOCK_EnableClock(kCLOCK_Ct32b0);
    #endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    #if !(defined(FSL_SDK_DISABLE_DRIVER_RESET_CONTROL) && FSL_SDK_DISABLE_DRIVER_RESET_CONTROL)
    /* Reset the module. */
    #if !(defined(FSL_FEATURE_CTIMER_HAS_NO_RESET) && (FSL_FEATURE_CTIMER_HAS_NO_RESET))
        RESET_PeripheralReset(kCT32B0_RST_SHIFT_RSTn);
    #endif
    #endif /* FSL_SDK_DISABLE_DRIVER_RESET_CONTROL */

    /* Setup the cimer mode and count select */
    #if !(defined(FSL_FEATURE_CTIMER_HAS_NO_INPUT_CAPTURE) && (FSL_FEATURE_CTIMER_HAS_NO_INPUT_CAPTURE))
        CTIMER0->CTCR = CTIMER_CTCR_CTMODE(0) | CTIMER_CTCR_CINSEL(0);
    #endif
        
    /* Setup the timer prescale value */
    CTIMER0->PR = CTIMER_PR_PRVAL(0);
        
    //Start CTIMER0
    CTIMER0->TCR |= CTIMER_TCR_CEN_MASK;
}

/*!
 * @brief   Initialization of Systick timer
 *
 *          This function configures the Systick as a source of interrupt
 *
 * @param   compare - defines the period of counter refresh
 *
 * @return  None
 */
void SystickInit(uint32_t compare)
{
    SysTick->VAL  = 0;
    SysTick->LOAD = compare;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

/*!
 * @brief   Setup of Clock
 *
 * @return  None
 */
void ClockInit(void)
{
    /* SDK Initialization */
    BOARD_BootClockRUN();
}

/*!
 * @brief   Setup of LPUART
 *
 * @return  None
 */
void SerialInit()
{
    /* attach FRG0 clock to FLEXCOMM3 (debug console) */
    CLOCK_SetFRGClock(BOARD_DEBUG_UART_FRG_CLK);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
  
    usart_config_t config;
  
    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUSART_ParityDisabled;
     * config.stopBitCount = kUSART_OneStopBit;
     * config.loopback = false;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    USART_GetDefaultConfig(&config);
    config.baudRate_Bps = UART_BAUD_RATE;
    config.enableTx     = true;
    config.enableRx     = true;

    USART_Init(APPLICATION_SERIAL_BASE, &config, CLOCK_GetFlexCommClkFreq(3U));
    
    FMSTR_SerialSetBaseAddress(APPLICATION_SERIAL_BASE);
}
