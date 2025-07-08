/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_mcimx93evk.h"

#if FMSTR_SERIAL_ENABLE
#include "freemaster.h"
#include "freemaster_serial.h"
#include "freemaster_serial_lpuart.h"
#endif

#include "board.h"
#include "fsl_rgpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief   Watchdog disabling function
 *
 * @param   void
 *
 * @return  None
 */
void WatchdogDisable(void)
{
    __asm("cpsid i");
    WDOG1->CNT   = WDOG_UPDATE_KEY;
    WDOG1->TOVAL = 0xFFFF;
    WDOG1->CS    = (uint32_t)((WDOG1->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;
    __asm("cpsie i");
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
void ReferenceTimerInit(void)
{
    REF_TIMER_USED->CSR = 0;                                            /* time counter mode */
    REF_TIMER_USED->CSR = LPTMR_CSR_TCF_MASK | LPTMR_CSR_TFC_MASK;      /* CNR reset on overflow */
    REF_TIMER_USED->PSR &= (~LPTMR_PSR_PBYP_MASK);                      /* prescaler enabled */
    REF_TIMER_USED->PSR &= (~LPTMR_PSR_PRESCALE_MASK);                  /* Divide 2 */
    REF_TIMER_USED->PSR &= (~LPTMR_PSR_PCS_MASK);                       /* clear prescaler clock 0 selected MCGIRCLK  */

    REF_TIMER_USED->PSR |= LPTMR_PSR_PCS(2);                            /*32 kHz for CLocktest */

    REF_TIMER_USED->CMR = 0;                                            /* clear the compare register */
    REF_TIMER_USED->CSR |= LPTMR_CSR_TEN_MASK;                          /* enable timer */
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
    lpuart_config_t LPUART_1_config;

    /*
     * config.baudRate_Bps = UART_BAUD_RATE;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&LPUART_1_config);
    LPUART_1_config.baudRate_Bps = UART_BAUD_RATE;
    LPUART_1_config.enableTx = false;
    LPUART_1_config.enableRx = false;

    LPUART_Init(APPLICATION_SERIAL_BASE, &LPUART_1_config, BOARD_DEBUG_UART_CLK_FREQ);

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
    /* Port configuration structure */
    rgpio_pin_config_t portConfig = {pinDir ? kRGPIO_DigitalOutput : kRGPIO_DigitalInput, 0U};

    /* Initialization */
    RGPIO_PinInit((RGPIO_Type *)gpio, pinNum, &portConfig);
}
