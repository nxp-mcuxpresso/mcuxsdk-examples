/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_evkmimxrt1180.h"

#include "freemaster.h"
#include "freemaster_serial.h"
#include "freemaster_serial_lpuart.h"

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
    uint32_t prescaler = 0;
    uint16_t window    = 0;

    __asm("cpsid i");

    /* USED_WDOG */
    USED_WDOG->CNT   = RTWDOG_UPDATE_KEY;                /* Unlock sequence */
    USED_WDOG->TOVAL = (wd_setup_value); /* Set timeout */

    /* Enable rtwdog, LPO clock, interrupt disabled, update enabled, 32b refresh, window mode, prescaler 255
     * enabled/disabled */
    USED_WDOG->CS = RTWDOG_CS_EN_MASK | RTWDOG_CS_CLK(1) | RTWDOG_CS_INT(0) | RTWDOG_CS_UPDATE(1) |
                    RTWDOG_CS_CMD32EN_MASK | (window == 0 ? RTWDOG_CS_WIN(0) : RTWDOG_CS_WIN(1)) |
                    (prescaler == 0 ? RTWDOG_CS_PRES(0) : RTWDOG_CS_PRES(1));

    if (window > 0)
    {
        USED_WDOG->WIN = (uint16_t)window;
    }

    __asm("cpsie i");
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
    __asm("cpsid i");

    /* USED_WDOG */
    USED_WDOG->CNT   = RTWDOG_UPDATE_KEY; /* Unlock sequence */
    USED_WDOG->TOVAL = (uint16_t)0xFFFF;  /* Write any nonzero value */
    /* Clear the EN bit to disable watchdog */
    USED_WDOG->CS = (uint32_t)((USED_WDOG->CS) & ~RTWDOG_CS_EN_MASK);

    __asm("cpsie i");
}

/*!
 * @brief   Initialization of GPT1 timer
 *
 *          Firstly used for watchdog test, then for clock test
 *
 * @param   clkSource -
 * @param   compare   - 32bit value
 * @param   prescaler - valid range 1-4096
 *
 * @return  None
 */
void GPT1_Init(uint32_t clkSource, uint32_t compare, uint32_t prescaler)
{
    clock_ip_name_t gpt_clock = kCLOCK_Gpt1;

    CLOCK_EnableClock(gpt_clock);

    /* Reset registers */
    GPT1->CR = GPT_CR_SWR(1U);

    /* Wait enable, stop enable, counter is reset when timer is disabled */
    GPT1->CR |= GPT_CR_WAITEN_MASK | GPT_CR_STOPEN_MASK | GPT_CR_ENMOD_MASK;

    /* Clock source and prescaler */
    if (clkSource == GPT_SRC_24M)
    {
        GPT1->CR |= GPT_CR_CLKSRC(clkSource) | GPT_CR_EN_24M_MASK; /* Set 24MHz clock source */
    }
    else
    {
        GPT1->CR |= GPT_CR_CLKSRC(clkSource); /* Set clock source */
    }

    /* set prescaler */
    if (GPT1->CR & GPT_CR_EN_24M_MASK)
    {
        if (prescaler > 0 && prescaler <= GPT_PR_PRESCALER24M_MASK + 1)
        {
            GPT1->PR = GPT_PR_PRESCALER24M(prescaler - 1);
        }
    }
    else
    {
        if (prescaler > 0 && prescaler <= GPT_PR_PRESCALER_MASK + 1)
        {
            GPT1->PR = GPT_PR_PRESCALER(prescaler - 1);
        }
    }

    GPT1->OCR[0] = GPT_OCR_COMP(compare); /* Compare value */

    /* Choose whether generates interrupt or not */
    // GPT1->IR = GPT_IR_OF1IE_MASK; /* Output compare 1 interrupt enable */
    // NVIC_EnableIRQ(GPT1_IRQn);    /* Enable interrupt */

    GPT1->CR |= GPT_CR_EN_MASK; /* Enable GPT */
}

/*!
 * @brief   Initialization of GPT2 timer
 *
 *          Used for clock test
 *
 * @param   clkSource -
 * @param   compare   - 32bit value
 * @param   prescaler - valid range 1-4096
 *
 * @return  None
 */
void GPT2_Init(uint32_t clkSource, uint32_t compare, uint32_t prescaler)
{
    /* Gpt2 serial clock, gpt2 bus clock enable */
    clock_ip_name_t gpt_clock = kCLOCK_Gpt2;

    CLOCK_EnableClock(gpt_clock);

    /* Reset registers */
    GPT2->CR = GPT_CR_SWR(1U);

    /* Wait enable, stop enable, counter is reset when timer is disabled */
    GPT2->CR |= GPT_CR_WAITEN_MASK | GPT_CR_STOPEN_MASK | GPT_CR_ENMOD_MASK;

    /* Clock source and prescaler */
    if (clkSource == GPT_SRC_24M)
    {
        GPT2->CR |= GPT_CR_CLKSRC(clkSource) | GPT_CR_EN_24M_MASK; /* Set 24MHz clock source */
    }
    else
    {
        GPT2->CR |= GPT_CR_CLKSRC(clkSource); /* Set clock source */
    }

    if (prescaler > 0 && prescaler <= GPT_PR_PRESCALER_MASK + 1)
    {
        GPT2->PR = GPT_PR_PRESCALER(prescaler - 1);
    }

    GPT2->OCR[0] = GPT_OCR_COMP(compare); /* Compare value */

    /* Choose whether generates interrupt or not */
    // GPT2->IR = GPT_IR_OF1IE_MASK; /* Output compare 1 interrupt enable */
    // NVIC_EnableIRQ(GPT2_IRQn);    /* Enable interrupt */

    GPT2->CR |= GPT_CR_EN_MASK; /* Enable GPT */
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
    const lpuart_config_t LPUART_1_config = {.baudRate_Bps    = UART_BAUD_RATE,
                                             .parityMode      = kLPUART_ParityDisabled,
                                             .dataBitsCount   = kLPUART_EightDataBits,
                                             .isMsb           = false,
                                             .stopBitCount    = kLPUART_OneStopBit,
                                             .txFifoWatermark = 0,
                                             .rxFifoWatermark = 0,
                                             .enableRxRTS     = false,
                                             .enableTxCTS     = false,
                                             .txCtsSource     = kLPUART_CtsSourcePin,
                                             .txCtsConfig     = kLPUART_CtsSampleAtStart,
                                             .rxIdleType      = kLPUART_IdleTypeStartBit,
                                             .rxIdleConfig    = kLPUART_IdleCharacter1,
                                             .enableTx        = true,
                                             .enableRx        = true};

    LPUART_Init(APPLICATION_SERIAL_BASE, &LPUART_1_config, BOARD_DebugConsoleSrcFreq());

    #if FMSTR_SERIAL_ENABLE
        FMSTR_SerialSetBaseAddress(APPLICATION_SERIAL_BASE);
    #endif //FMSTR_SERIAL_ENABLE
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
    /* Enable clock to ADC1 module */
    CLOCK_EnableClock(kCLOCK_Adc1);

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

/*!
 * @brief   This function performs the MPU (Memory Protection Unit) configuration
 *          for the various memory regions inc1uding cache setup and the exact
 *          behaviour of this function is contro11ed by a number of defined symbo1s.
 *
 * @param   none
 *
 * @return  None
 */
void MPUSetup(void)
{
    BOARD_ConfigMPU();
}
