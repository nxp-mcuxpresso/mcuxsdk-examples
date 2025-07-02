/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_frdmmcxa153.h"

#if FMSTR_SERIAL_ENABLE
#include "freemaster.h"
#include "freemaster_serial.h"
#include "freemaster_serial_lpuart.h"
#endif

#include "board.h"
#include "fsl_ctimer.h"

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
    uint32_t reg_offset         = CLK_GATE_REG_OFFSET(kCLOCK_GateWWDT0);
    uint32_t bit_shift          = CLK_GATE_BIT_SHIFT(kCLOCK_GateWWDT0);
    volatile uint32_t *pClkCtrl = (volatile uint32_t *)((uint32_t)(&(MRCC0->MRCC_GLB_CC0_SET)) + reg_offset);

    /* Unlock clock configuration */
    SYSCON->CLKUNLOCK &= ~SYSCON_CLKUNLOCK_UNLOCK_MASK;
    
    *pClkCtrl = (1UL << bit_shift);
    
    /* Freeze clock configuration */
    SYSCON->CLKUNLOCK |= SYSCON_CLKUNLOCK_UNLOCK_MASK;

    USED_WDOG->TC     = WWDT_TC_COUNT(wd_setup_value); /* refresh value */
    USED_WDOG->MOD    = WWDT_MOD_WDRESET(1) | WWDT_MOD_WDEN(1);
    USED_WDOG->WINDOW = 0xFFFFFF; /* Disable Window mode */

    __asm("CPSID i");
    USED_WDOG->FEED = 0xAA; /* Start WDOG */
    USED_WDOG->FEED = 0x55;
    __asm("CPSIE i");
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
    /* Use FRO HF clock for some of the Ctimers */
    CLOCK_SetClockDiv(CLOCK_DIVIDER, CLOCK_DIVIDE_VALUE);
    CLOCK_AttachClk(USED_CLOCK_SOURCE);
  
    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(REF_TIMER_USED, &config);
    CTIMER_StartTimer(REF_TIMER_USED);
    
//    CLOCK_EnableClock(kCLOCK_GateCTIMER0);
//    RESET_PeripheralReset(kCTIMER0_RST_SHIFT_RSTn);
//    
//    REF_TIMER_USED->CTCR = CTIMER_CTCR_CTMODE(0U) | CTIMER_CTCR_CINSEL(0U);
//    REF_TIMER_USED->PR = CTIMER_PR_PRVAL(CLOCK_DIVIDER);
//    REF_TIMER_USED->TCR |= CTIMER_TCR_CEN_MASK;
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
    /* attach FRO 12M to LPUART0 (debug console) */
    RESET_PeripheralReset(kLPUART0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kPORT0_RST_SHIFT_RSTn);
    
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_SetClockDiv(kCLOCK_DivLPUART0, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    
    /* Init hardware*/
    /* Release peripheral RESET */
    RESET_PeripheralReset(kCTIMER0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kCRC_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kPORT2_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO2_RST_SHIFT_RSTn);
    
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1u);
    CLOCK_AttachClk(kFRO12M_to_ADC0);
    
    /* enable clock for GPIO*/
    CLOCK_EnableClock(kCLOCK_GateGPIO2);    
    
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
    gpio_pin_config_t portConfig = {pinDir ? kGPIO_DigitalOutput : kGPIO_DigitalInput, 0U};

    /* Initialization */
    GPIO_PinInit((GPIO_Type *)gpio, pinNum, &portConfig);
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
    /* Attach peripheral clock */
    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1u);
    CLOCK_AttachClk(kFRO12M_to_ADC0);
    CLOCK_EnableClock(kCLOCK_GateADC0);
    
    /* enable VREF */
    SPC0->ACTIVE_CFG1 |= 0x1;
    
    /* Disable the module before setting configuration. */
    TESTED_ADC->CTRL &= ~ADC_CTRL_ADCEN_MASK;
    
    /* Reset ADC module */
    TESTED_ADC->CTRL |= ADC_CTRL_RST_MASK;
    TESTED_ADC->CTRL &= ~ADC_CTRL_RST_MASK;

    /* Reset FIFO(s) */
#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2))
    TESTED_ADC->CTRL |= ADC_CTRL_RSTFIFO0_MASK;
    TESTED_ADC->CTRL |= ADC_CTRL_RSTFIFO1_MASK;
#else
    TESTED_ADC->CTRL |= ADC_CTRL_RSTFIFO_MASK;
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */

    /* Disable ADC */
    TESTED_ADC->CTRL &= ~ADC_CTRL_ADCEN_MASK;

    /* Conf ADC CFG register */
    TESTED_ADC->CFG |= ADC_CFG_PUDLY(0x80)    /* Power up delay. */
                       | ADC_CFG_REFSEL(0U)   /* Reference voltage. */
                       | ADC_CFG_TPRICTRL(0U) /* Trigger priority policy. */
                       | ADC_CFG_PWREN(1U);   /* LPADC Analog Pre-Enable */

    /* Disable PAUSE between conversions */
    TESTED_ADC->PAUSE = 0U;

    TESTED_ADC->TCTRL[0] = ADC_TCTRL_TCMD(1);

    /* Enable ADC */
    TESTED_ADC->CTRL |= ADC_CTRL_ADCEN_MASK;
    

}
