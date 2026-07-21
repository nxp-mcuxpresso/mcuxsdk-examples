/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_frdmmcxn947t.h"

#include "freemaster.h"
#include "freemaster_serial.h"
#include "freemaster_serial_lpuart.h"

#include "board.h"
#include "fsl_ctimer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


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
    /* Set clock divider for WWDT clock source. */
    CLOCK_SetClkDiv(kCLOCK_DivWdt0Clk, 1U);

    /* Enable FRO 1M clock for WWDT module. */
    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO1MHZ_CLK_ENA_MASK;

    CLOCK_EnableClock(kCLOCK_Wwdt0);

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
    CLOCK_SetClkDiv(CLOCK_DIVIDER, CLOCK_DIVIDE_VALUE);
    CLOCK_AttachClk(USED_CLOCK_SOURCE);

    ctimer_config_t config;
    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(REF_TIMER_USED, &config);
    CTIMER_StartTimer(REF_TIMER_USED);

//    SYSCON->AHBCLKCTRL.AHBCLKCTRL0 |= SYSCON_AHBCLKCTRL0_RTC_MASK;
//
//    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO1MHZ_CLK_ENA_MASK;
//
//    SYSCON->CTIMERCLKSEL.CTIMERCLKSEL0 = 0x4;                         /* OSCILATOR 1MHZ */
//    SYSCON->AHBCLKCTRL.AHBCLKCTRL1 |= SYSCON_AHBCLKCTRL1_TIMER0_MASK; /*Enable clock to Ctimer0*/
//
//    SYSCON->PRESETCTRL.PRESETCTRL0 &= ~(SYSCON_PRESETCTRL1_TIMER0_RST_MASK); // Reset the CTIMER0
//    SYSCON->PRESETCTRL.PRESETCTRL0 |= (SYSCON_PRESETCTRL1_TIMER0_RST_MASK);
//
//    CTIMER0->CTCR &= ~(CTIMER_CTCR_CTMODE_MASK);
//    CTIMER0->TCR |= CTIMER_TCR_CEN_MASK;     /*Enable counter*/
//    CTIMER0->TCR |= CTIMER_TCR_CRST_MASK;    /*Counter reset*/
//    CTIMER0->TCR &= ~(CTIMER_TCR_CRST_MASK); /*Counter stop reset*/
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
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO HF to ADC0 */
    CLOCK_SetClkDiv(kCLOCK_DivAdc0Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_ADC0);

    /* enable clock for GPIO*/
    CLOCK_EnableClock(kCLOCK_Gpio0);
        /* enable clock for GPIO*/
    CLOCK_EnableClock(kCLOCK_Gpio1);
        /* enable clock for GPIO*/
    CLOCK_EnableClock(kCLOCK_Gpio2);
            /* enable clock for GPIO*/
    CLOCK_EnableClock(kCLOCK_Gpio3);
            /* enable clock for GPIO*/
    CLOCK_EnableClock(kCLOCK_Gpio4);

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
    /* Enable clock to ADC0 module */
    CLOCK_EnableClock(kCLOCK_Adc0);
    
    /* Enable VREF */
    SPC0->ACTIVE_CFG1 = 3U;

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
                       | ADC_CFG_REFSEL(2U)   /* Reference voltage. */
                       | ADC_CFG_TPRICTRL(0U) /* Trigger priority policy. */
                       | ADC_CFG_PWREN(1U);   /* LPADC Analog Pre-Enable */

    /* Disable PAUSE between conversions */
    TESTED_ADC->PAUSE = 0U;

    TESTED_ADC->TCTRL[0] = ADC_TCTRL_TCMD(1);

    /* Enable ADC */
    TESTED_ADC->CTRL |= ADC_CTRL_ADCEN_MASK;

}


#if defined(MCXN947T_cm33_core0_H_)
/*This function only enable CLOCK to PORT which are use by TSI peripheral */
void tsi_port_clock_enable(void)
{
    PORT_Type *used_port[] = TSI_PORT_USE;
    // 0 = NULL = termination;
    for (int i = 0; used_port[i] != 0; i++)
    {
        switch ((uint32_t)used_port[i])
        {
            case (uint32_t)PORT0:
                SYSCON->AHBCLKCTRL0 |= SYSCON_AHBCLKCTRL0_PORT0_MASK; /* Clock enabled */
                break;

            case (uint32_t)PORT1:
               SYSCON->AHBCLKCTRL0 |= SYSCON_AHBCLKCTRL0_PORT1_MASK; /* Clock enabled */
                break;

            case (uint32_t)PORT2:
                SYSCON->AHBCLKCTRL0 |= SYSCON_AHBCLKCTRL0_PORT2_MASK; /* Clock enabled */
                break;

            case (uint32_t)PORT3:
                SYSCON->AHBCLKCTRL0 |= SYSCON_AHBCLKCTRL0_PORT3_MASK; /* Clock enabled */
                break;

            case (uint32_t)PORT4:
                SYSCON->AHBCLKCTRL0 |= SYSCON_AHBCLKCTRL0_PORT4_MASK; /* Clock enabled */
                break;
        }
    }
}

/*!
 * @brief   Initialization of TSI in Self capacitive mode
 *
 * Configyures TSI in self capacitive (single-wire) mode
 * conversion complete interrupt disabled
 *
 * @param   void
 *
 * @return  None
 */
void Tsi0InitSelfCap(void)
{
    /* Self capacitive mode: No Sensitivity Boost (basic) */
    TSI->CONFIG   = 0x18000000; //0x07800000; /* Reset value, Chanel 0 */
    TSI->TSHD     = 0x0; /* Threshold 0x0 - reset value */
    TSI->GENCS    = 0x00003020; //0x02001000; /* Reset value SW trigger */
    TSI->MUL      = 0x0; /* Reset value */
    TSI->SINC     = 0x003b0001; //TSI_SINC_DECIMATION(7)| TSI_SINC_ORDER_MASK;
    TSI->SSC0     = 0x20310001; //TSI_SSC0_SSC_MODE(2); /* SSC disabled */
    TSI->SSC1     = 0x00600040;
    TSI->SSC2     = 0x10080101;
    TSI->BASELINE = 0x00010000; /* Default value */
    TSI->CHMERGE  = 0x0; /* Default NO MERGE */
    TSI->SHIELD   = 0x4000000; /* Default -  not used */
    TSI->MISC     = 0x0; /* Default value */
    TSI->TRIG     = 0x0; /* Default value not used */

    TSI->GENCS |= TSI_GENCS_TSIEN_MASK; /* enable tsi module */
}

/*!
 * @brief   Initialization of TSI in Mutual capacitive mode
 *
 * Configyures TSI in Mutual capacitive (dual-wire) mode
 * conversion complete interrupt disabled
 *
 * @param   void
 *
 * @return  None
 */
void Tsi0InitMutualCap(void)
{
    /* Self capacitive mode: No Sensitivity Boost (basic) */
    TSI->CONFIG_MUTUAL   = 0x18000001; //0x07800000; /* Reset value, Chanel 0 */
    TSI->TSHD     = 0x0; /* Threshold 0x0 - reset value */
    TSI->GENCS    = 0x00003020; //0x02001000; /* Reset value SW trigger */
    TSI->MUL      = 0x0; /* Reset value */
    TSI->SINC     = 0x003b0001; //TSI_SINC_DECIMATION(7)| TSI_SINC_ORDER_MASK;
    TSI->SSC0     = 0x20310001; //TSI_SSC0_SSC_MODE(2); /* SSC disabled */
    TSI->SSC1     = 0x00600040;
    TSI->SSC2     = 0x10080101;
    TSI->BASELINE = 0x00010000; /* Default value */
    TSI->CHMERGE  = 0x0; /* Default NO MERGE */
    TSI->SHIELD   = 0x4000000; /* Default -  not used */
    TSI->MISC     = 0x0; /* Default value */
    TSI->TRIG     = 0x0; /* Default value not used */

    TSI->GENCS |= TSI_GENCS_TSIEN_MASK; /* enable tsi module */
}

void Tsi0ClockEnable(void)
{
    SYSCON->AHBCLKCTRL3 |= SYSCON_AHBCLKCTRL3_TSI(1);
    SYSCON->TSICLKSEL = SYSCON_TSICLKSEL_SEL(4); /* FRO 12MHz for TSI */
    SYSCON->TSICLKDIV = 0x0; // DIVIDe by 1, not reset,
}

#endif
