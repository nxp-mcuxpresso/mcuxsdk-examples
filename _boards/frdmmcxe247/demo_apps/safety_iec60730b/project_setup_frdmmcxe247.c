/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_frdmmcxe247.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_lpuart.h"

#include "freemaster.h"
#include "freemaster_serial_lpuart.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
 * @brief   Watchdog configuration function
 *
 *          Enables the watchdog. Also in Wait and Stop mode. Updates are allowed
 *
 * @param   wd_setup_value      //watchdog setup value for timeout
 *
 * @return  None
 */
void WatchdogEnable(uint32_t wd_setup_value)
{
    WDOG->CNT   = WDOG_UPDATE_KEY;                  /* Update of Watchdog registers */
    WDOG->TOVAL = (uint16_t)wd_setup_value; /* Watchdog timeout value */

    /* Watchdog enabled in chip stop mode. */
    /* Watchdog enabled in chip wait mode. */
    /* Updates allowed. */
    /* Watchdog enabled. */
    /* Enables support for 32-bit refresh/unlock command write words. 16-bit or 8-bit is NOT supported. */
    /* LPO clock , 128 KHz */

    WDOG->CS = WDOG_CS_STOP_MASK | WDOG_CS_WAIT_MASK | WDOG_CS_UPDATE_MASK | WDOG_CS_EN_MASK | WDOG_CS_CMD32EN_MASK | WDOG_CS_CLK(1);
}

/*!
 * @brief   Watchdog disabling function
 *
 * @param   *WDOGx - pointer to the base address of the periphery
 *
 * @return  None
 */
void WatchdogDisable(void)
{
    __asm("cpsid i");
    WDOG->CNT   = WDOG_UPDATE_KEY;
    WDOG->TOVAL = 0xFFFF;
    WDOG->CS    = (uint32_t)((WDOG->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;
    __asm("cpsie i");
}

/*!
 * @brief   Initialization of Systick timer
 *
 *          This function configures the Systick as a source of interrupt
 *
 * @param   reload_value - defines the period of counter refresh
 *
 * @return  None
 */
void SystickInit(uint32_t reload_value)
{
    SysTick->VAL  = 0;
    SysTick->LOAD = reload_value;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

/*!
 * @brief   Initialization of LPTMR
 *
 *          This function initializes the LPTMR. LPTMR is used for clock test.
 *
 * @param   void
 *
 * @return  None
 */
void ReferenceTimerInit(void)
{   
    /* Ungate the LPTMR clock*/
    CLOCK_EnableClock(kCLOCK_Lptmr0);
    
    LPTMR_USED->CSR = 0;                                       /* time counter mode */
    LPTMR_USED->CSR = LPTMR_CSR_TCF_MASK | LPTMR_CSR_TFC_MASK; /* CNR reset on overflow */
    LPTMR_USED->PSR |= LPTMR_PSR_PBYP_MASK;   /* prescaler bypassed, selected clock directly clocks the CNR */
    LPTMR_USED->PSR &= (~LPTMR_PSR_PCS_MASK); /* clear prescaler clock 0 selected   */

    LPTMR_USED->PSR |= LPTMR_PSR_PCS(0);

    LPTMR_USED->CMR = 0;                    /* clear the compare register */
    LPTMR_USED->CSR &= ~LPTMR_CSR_TEN_MASK; /* stop the timer, this clears the TFC flag */
    LPTMR_USED->CSR |= LPTMR_CSR_TEN_MASK;  /* start the timer */
}

/*!
 * @brief   Setup of clock
 *
 *
 * @param   void
 *
 * @return  None
 *
 *
 */
void ClockInit(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_BootClockRUN();
    
#if FLASH_TEST_ENABLED
    CLOCK_EnableClock(kCLOCK_Crc0); /* ungate CRC clock */
#endif
}

/*!
 * @brief  Sets port direction and mux.
 *
 * @param  gpio   - definition from iec60730b_dio.h, for example FS_DIO_GPIOA for GPIO A.
 *         pcr    - base address of PCR register.
 *         pinNum - pin number.
 *         pinDir - pin direction.
 *         pinMux - pin mux (usually GPIO_MUX == 1).
 *
 * @return  None
 */
void PortInit(uint32_t gpio, uint32_t pcr, uint8_t pinNum, uint8_t pinDir, uint8_t pinMux)
{
    volatile GPIO_Type *pGPIO     = (GPIO_Type *)gpio;
    volatile PORT_Type *pPORT_PCR = (PORT_Type *)pcr;

    pPORT_PCR->PCR[pinNum] &= (~PORT_PCR_MUX_MASK);
    pPORT_PCR->PCR[pinNum] |= PORT_PCR_MUX(pinMux); /* Alternative */

    if (pinDir == PIN_DIRECTION_OUT)
    {
        pGPIO->PDDR |= (1 << pinNum); /* PINx = 1 = output */
    }
    else if (pinDir == PIN_DIRECTION_IN)
    {
        pGPIO->PDDR &= ~(1 << pinNum); /* PINx = 0 = input */
    }
}

/*!
 * @brief   Setup of LPUART
 *
 */
void SerialInit(void)
{
    CLOCK_SetIpSrc(kCLOCK_Lpuart2, BOARD_DEBUG_UART_CLKSRC);
  
    lpuart_config_t config;

    /*
     * config.baudRate_Bps = UART_BAUD_RATE;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = UART_BAUD_RATE;
    config.enableTx     = false;
    config.enableRx     = false;

    LPUART_Init((LPUART_Type *)APPLICATION_SERIAL_BASE, &config, CLOCK_GetIpFreq(kCLOCK_Lpuart2));

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((LPUART_Type *)APPLICATION_SERIAL_BASE);
}

/*!
 * @brief   Initialization of ADC_x
 *
 *          single-ended 12-bit conversion
 *
 * @param   ADC_x - instance of ADC = ADC0 or ADC1
 *
 * @return  None
 */
void AdcInit(ADC_Type *base)
{
    uint32_t tmp32;

    /* Enable the clock. */
    CLOCK_EnableClock(kCLOCK_Adc0);

    /* ADCx_CFG1. */
    tmp32 = (base->CFG1 & ~(ADC_CFG1_ADICLK_MASK | ADC_CFG1_ADIV_MASK | ADC_CFG1_MODE_MASK));
    tmp32 |= (ADC_CFG1_ADICLK(0) | ADC_CFG1_ADIV(0) |
              ADC_CFG1_MODE(1));
    base->CFG1 = tmp32;

    /* ADCx_CFG2. */
    tmp32 = (base->CFG2 & ~ADC_CFG2_SMPLTS_MASK);
    tmp32 |= ADC_CFG2_SMPLTS(12U);
    base->CFG2 = tmp32;

    /* ADCx_SC2. */
    tmp32 = (base->SC2 & ~ADC_SC2_REFSEL_MASK);
    tmp32 |= ADC_SC2_REFSEL(0);
    base->SC2 = tmp32;

    /* ADCx_SC3. */
    tmp32 = (base->SC3 & ~ADC_SC3_ADCO_MASK);
    base->SC3 = tmp32;
}
