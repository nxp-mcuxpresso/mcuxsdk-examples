/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_lpcxpresso55s36.h"
#include "board.h"
#include "pin_mux.h"

#include "fsl_lpadc.h"
#include "fsl_power.h"
#include "fsl_clock.h"
#include "fsl_usart.h"
#include "fsl_iocon.h"
#include "freemaster.h"
#include "freemaster_serial.h"
#include "freemaster_serial_usart.h"

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
    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO1MHZ_CLK_ENA_MASK;   /* Enable FRO_1MHz */
    SYSCON->WDTCLKDIV = 0;                                          /* WD clock 1MHz */
    SYSCON->AHBCLKCTRL0 |= SYSCON_AHBCLKCTRL0_WWDT_MASK; /* Enable clock to WDT */

    uint32_t bitMask = 0x400000;

    /* reset register is in SYSCON */
    /* set bit */
    SYSCON->PRESETCTRLSET[0] = bitMask;
    /* wait until it reads 0b1 */
    while (0u == (SYSCON->PRESETCTRL0 & bitMask))
    {
    }

    /* clear bit */
    SYSCON->PRESETCTRLCLR[0] = bitMask;
    /* wait until it reads 0b0 */
    while (bitMask == (SYSCON->PRESETCTRL0 & bitMask))
    {
    }

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
 * @param   *WDOGx - pointer to the base address of the periphery
 *
 * @return  None
 */
void WatchdogDisable(void)
{
    /* Wdog is disabled on LPCdisabled after reset by default */
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
    /* Use 12 MHz clock for some of the Ctimers */
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 1u, true);
    CLOCK_AttachClk(kPLL0_to_CTIMER0);
  
    BOARD_BootClockPLL100M();
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
    SYSCON->CTIMERCLKSEL[0] = 0x1;                         /* 100MHZ HF FRO CLOCK */
    SYSCON->AHBCLKCTRL1 |= SYSCON_AHBCLKCTRL1_TIMER0_MASK; /* Enable clock to Ctimer0 */

    SYSCON->PRESETCTRL0 &= ~(SYSCON_PRESETCTRL1_TIMER0_RST_MASK);
    SYSCON->PRESETCTRL0 |= (SYSCON_PRESETCTRL1_TIMER0_RST_MASK);

    CTIMER0->CTCR &= ~(CTIMER_CTCR_CTMODE_MASK);

    CTIMER0->TCR |= CTIMER_TCR_CEN_MASK;     /* Enable counter */
    CTIMER0->TCR |= CTIMER_TCR_CRST_MASK;    /* Counter reset */
    CTIMER0->TCR &= ~(CTIMER_TCR_CRST_MASK); /* Counter stop reset */
}
/*!
 * @brief  Sets port direction and mux
 *
 * @param
 *
 * @return  None
 */
void PortSetup(uint8_t *pByte,
               uint32_t *pDir,
               uint32_t *pIocon,
               uint32_t pinDir,
               uint32_t pinNum,
               uint32_t pull,
               uint32_t clock_enable_shift)
{
    /* Enable clock to GPIO module */
    SYSCON->AHBCLKCTRL0 |= (1 << clock_enable_shift);

    *pIocon |= IOCON_PIO_DIGIMODE(1);  /*Enable Digi mode*/
    *pIocon &= ~(IOCON_PIO_MODE_MASK); /*Clear PULL setting*/
    *pIocon |= IOCON_PIO_MODE(pull);   /*Set pullup*/

    if (pinDir == PIN_DIRECTION_OUT)
    {
        *pDir |= (1 << pinNum); /* PINx = 1 = output */
    }
    else if (pinDir == PIN_DIRECTION_IN)
    {
        *pDir &= ~(1 << pinNum); /* PINx = 0 = input */
    }
}

/*!
 * @brief   Initialization of ADC0
 *
 *          8 MHz System Oscillator Bus Clock is the source clock.
 *          single-ended 16-bit conversion
 *
 * @param   void
 *
 * @return  None
 */
void AdcInit(void)
{     
    CLOCK_SetClkDiv(kCLOCK_DivAdc0Clk, 2U, true);
    CLOCK_AttachClk(kPLL0_to_ADC0);
    
    /* Disable VREF power down */ 
    PMC->PDRUNCFGCLR0 = kPDRUNCFG_PD_VREF;    
    
    lpadc_config_t configStruct;
    lpadc_conv_command_config_t commandConfigStruct;
    lpadc_conv_trigger_config_t triggerConfigStruct; 
           
    LPADC_GetDefaultConfig(&configStruct);
    configStruct.enableAnalogPreliminary = true;
    configStruct.referenceVoltageSource  = kLPADC_ReferenceVoltageAlt3;
    configStruct.conversionAverageMode   = kLPADC_ConversionAverage128;
    configStruct.powerLevelMode          = kLPADC_PowerLevelAlt4;
    LPADC_Init(TESTED_ADC, &configStruct); 
    LPADC_DoOffsetCalibration(TESTED_ADC);
    LPADC_DoAutoCalibration(TESTED_ADC);
    LPADC_GetDefaultConvCommandConfig(&commandConfigStruct);
    commandConfigStruct.channelNumber = 8U;
    LPADC_GetDefaultConvTriggerConfig(&triggerConfigStruct);
    triggerConfigStruct.targetCommandId       = 1;
    triggerConfigStruct.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(TESTED_ADC, 0U, &triggerConfigStruct); /* Configurate the trigger0. */
}

/************************************************/
void SerialInit(void)
{
  
  BOARD_InitPins();
  /* attach main clock divide to FLEXCOMM0 (debug console) */
  CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

   usart_config_t config;
    /*
     *   usartConfig->baudRate_Bps = UART_BAUD_RATE;
     *   usartConfig->parityMode = kUSART_ParityDisabled;
     *   usartConfig->stopBitCount = kUSART_OneStopBit;
     *   usartConfig->bitCountPerChar = kUSART_8BitsPerChar;
     *   usartConfig->loopback = false;
     *   usartConfig->enableTx = false;
     *   usartConfig->enableRx = false;
    */
    USART_GetDefaultConfig(&config);
    /* Override the Default configuration to satisfy FreeMASTER needs */
    config.baudRate_Bps = UART_BAUD_RATE;
    config.enableTx = true;
    config.enableRx = true;

    USART_Init((USART_Type*)BOARD_DEBUG_UART_BASEADDR, &config, BOARD_DEBUG_UART_CLK_FREQ);

#if FMSTR_SERIAL_ENABLE
    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((USART_Type*)BOARD_DEBUG_UART_BASEADDR);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(BOARD_UART_IRQ);
    EnableGlobalIRQ(0);
#endif
#endif // FMSTR_SERIAL_ENABLE
}
