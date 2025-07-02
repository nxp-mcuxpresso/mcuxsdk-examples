/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_frdmmcxc444.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_lpuart.h"
#include "fsl_common.h"
#include "freemaster.h"
#include "freemaster_serial_lpuart.h"

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
 * @param   *WDOGx - pointer to the base address of the periphery
 *          timeout
 *
 * @return  None
 */
void WatchdogEnable(uint32_t wd_setup_value) /* = 0xC or 0x4 */
{
    SIM->COPC = (SIM_COPC_COPW(0) | SIM_COPC_COPCLKS(0) | SIM_COPC_COPT(3) | SIM_COPC_COPSTPEN(0) | SIM_COPC_COPDBGEN(0) | SIM_COPC_COPCLKSEL(0));
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
    SIM->COPC = SIM_COPC_COPT(0);
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
 * @brief   Setup of clock
 *
 * @param   void
 *
 * @return  None
 *
 *
 */
void ClockInit(void)
{
    BOARD_BootClockRUN();
}

/*!
 * @brief   Initialization of LPTMR
 *
 *          This function initializes the LPTMR. LPTMR is used for clock test.
 *
 * @param   call_type - for demo compatibility - not used
 *
 * @return  None
 */
void ReferenceTimerInit(uint8_t call_type)
{
    /* enable clock for LPTMR */
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK; /* enable clock gate to LPTMR */

    LPTMR0->CSR = 0;                                       /* time counter mode */
    LPTMR0->CSR = LPTMR_CSR_TCF_MASK | LPTMR_CSR_TFC_MASK; /* CNR reset on overflow */
    LPTMR0->PSR |= LPTMR_PSR_PBYP_MASK;   /* prescaler bypassed, selected clock directly clocks the CNR */
    LPTMR0->PSR &= (~LPTMR_PSR_PCS_MASK); /* clear prescaler clock 0 selected MCGIRCLK  */

    LPTMR0->PSR |= LPTMR_PSR_PCS(0);

    LPTMR0->CMR = 0;                   /* clear the compare register */
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK; /* enable timer */
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

    /* set GPIO and PORT pointers for PIN */
    if (gpio == GPIOA_BASE)
    {
        SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; /* Clock enabled */
    }
    else if (gpio == GPIOB_BASE)
    {
        SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; /* Clock enabled */
    }
    else if (gpio == GPIOC_BASE)
    {
        SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; /* Clock enabled */
    }
    else if (gpio == GPIOD_BASE)
    {
        SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK; /* Clock enabled */
    }
    else if (gpio == GPIOE_BASE)
    {
        SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; /* Clock enabled */
    }

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
 * @brief   Initialization of ADC0
 *
 *          8 MHz System Oscillator Bus Clock is the source clock.
 *          single-ended 12-bit conversion
 *
 * @param   void
 *
 * @return  None
 */
void AdcInit(void)
{
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; /* enable clock gate to ADC0 */

    ADC0->SC1[0] =
        0; /* conversion complete interrupt disabled, single-ended conversions anp input channels are selected */
    ADC0->SC1[0] |= ADC_SC1_ADCH(0x1F); /* module disabled */
    ADC0->SC1[1] =
        0; /* conversion complete interrupt disabled, single-ended conversions anp input channels are selected */
    ADC0->SC1[1] |= ADC_SC1_ADCH(0x1F); /* module disabled */
    ADC0->CFG1 = 0;
    ADC0->CFG1 |= ADC_CFG1_MODE(1); /* single-ended 12-bit conversion */

    ADC0->SC2 |= ADC_SC2_REFSEL(1);
    ADC0->SC3 = 0;
    ADC0->SC3 |= ADC_SC3_AVGS(1);
    ADC0->SC3 |= ADC_SC3_AVGE_MASK;

    PMC->REGSC |= PMC_REGSC_BGBE_MASK; /* Bandgap buffer enable */
}

void SerialInit(void)
{
    BOARD_InitPins();
    
    CLOCK_SetLpuart0Clock(1);

    lpuart_config_t config;

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = UART_BAUD_RATE;
    config.enableTx     = false;
    config.enableRx     = false;

    LPUART_Init(UART_USED, &config, CLOCK_GetPeriphClkFreq());

#if FMSTR_SERIAL_ENABLE
    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((LPUART_Type *)UART_USED);
#endif // FMSTR_SERIAL_ENABLE
}
