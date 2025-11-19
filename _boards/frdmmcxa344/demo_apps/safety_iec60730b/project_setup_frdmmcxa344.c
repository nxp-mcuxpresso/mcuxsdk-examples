/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_frdmmcxa344.h"

#if FMSTR_SERIAL_ENABLE
#include "freemaster.h"
#include "freemaster_serial.h"
#include "freemaster_serial_lpuart.h"
#endif

#include "board.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief   Watchdog configuration function
 *
 *          Enables the watchdog. Also in Wait and Stop mode. Updates are allowed
 *
 * @param   timeout
 * @param   window
 * @param   prescaler
 *
 * @return  None
 */
void WatchdogEnable(uint32_t wd_setup_value)
{

}

/*!
 * @brief   Watchdog disabling function
 *
 * @param   void
 *
 * @return  None
 */
void WatchdogDisable(void)
{
    
}

/*!
 * @brief   Initialization of CTIMER
 *
 *          This function initializes the CTIMER. CTIMER is used for After reset WDog test.
 *
 * @param   void
 *
 * @return  None
 */
void CTIMER_initialisation(void)
{

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
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_SetClockDiv(kCLOCK_DivLPUART2, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    RESET_PeripheralReset(BOARD_DEBUG_UART_RST);
    
//    /* Release peripheral reset */
//    RESET_ReleasePeripheralReset(kADC0_RST_SHIFT_RSTn);
//
//    /* Attach peripheral clock */
//    CLOCK_SetClockDiv(kCLOCK_DivADC, 1u);
//    CLOCK_AttachClk(kFRO_LF_DIV_to_ADC);    
//    
//    /* GPIO3: Peripheral clock is enabled */
//    CLOCK_EnableClock(kCLOCK_GateGPIO3);
//    
//    /* PORT3: Peripheral clock is enabled */
//    CLOCK_EnableClock(kCLOCK_GatePORT3);
//  
//    /* GPIO3 peripheral is released from reset */
//    RESET_ReleasePeripheralReset(kGPIO3_RST_SHIFT_RSTn);
//    
//    /* PORT3 peripheral is released from reset */
//    RESET_ReleasePeripheralReset(kPORT3_RST_SHIFT_RSTn);
//    
//    /* Release peripheral reset */
//    RESET_ReleasePeripheralReset(kCRC0_RST_SHIFT_RSTn);

    /* SDK Initialization */
    BOARD_InitBootClocks();
}

/*!
 * @brief   Setup of LPUART
 *
 * @return  None
 */
void SerialInit()
{
    lpuart_config_t LPUART_config;

    /*
     * config.baudRate_Bps = UART_BAUD_RATE;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&LPUART_config);
    LPUART_config.baudRate_Bps = UART_BAUD_RATE;
    LPUART_config.enableTx = true;
    LPUART_config.enableRx = true;

    LPUART_Init(APPLICATION_SERIAL_BASE, &LPUART_config, BOARD_DEBUG_UART_CLK_FREQ);

    FMSTR_SerialSetBaseAddress(APPLICATION_SERIAL_BASE);
}

/*!
 * @brief  Sets port direction and mux
 *
 * @note   How to determine pin GPIOx + pin number from schematic? E.g. GPIO_AD_B1_07 from schematic could
 *         be find in ref. manual in corresponding SW_MUX_CTL register (IOMUXC chapter)- GPIO1, 23.
 *
 * @param  gpio   - definition from device header file, for example GPIO1_BASE for GPIO1
 *         pinNum - pin number
 *         pinDir - pin direction (0 = input, 1 = output)
 *
 * @return  None
 */
void PortSetup(uint32_t gpio, uint8_t pinNum, uint8_t pinDir)
{

}

/*!
 * @brief   Initialization of ADC0
 *
 *
 *
 * @param   void
 *
 * @return  None
 */
void AdcInit(void)
{

}
