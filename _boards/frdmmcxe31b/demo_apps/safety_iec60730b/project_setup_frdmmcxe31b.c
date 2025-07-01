/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_frdmmcxe31b.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_lpuart.h"
#include "fsl_sar_adc.h"

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
    CLOCK_SetClkDiv(kCLOCK_DivStm0Clk, 1U);
    /* Use AIPS_PLAT_CLK clock for the STM0 */
    CLOCK_AttachClk(kAIPS_PLAT_CLK_to_STM0);
    
    /* Enable the STM clock */
    CLOCK_EnableClock(kCLOCK_Stm0);
    
    /* Initialize the STM */
    STM_USED->CR |= STM_CR_FRZ_MASK;
    STM_USED->CR |= STM_CR_CPS(0);
    
    /* Clear counter */
    STM_USED->CNT = 0U;
    
    /* Start the STM module */
    STM_USED->CR |= STM_CR_TEN_MASK;
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
    BOARD_ConfigMPU();
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
void PortInit(SIUL2_Type *base, uint32_t pin, uint8_t pinDir)
{
    if (pinDir == PIN_DIRECTION_OUT)
    {
        base->MSCR[pin] &= ~SIUL2_MSCR_IBE_MASK;
        base->MSCR[pin] |= SIUL2_MSCR_OBE(1UL);
    }
    else if (pinDir == PIN_DIRECTION_IN)
    {
        base->MSCR[pin] &= ~SIUL2_MSCR_OBE_MASK;
        base->MSCR[pin] |= SIUL2_MSCR_IBE(1UL);
    }
}

/*!
 * @brief   Setup of LPUART
 *
 */
void SerialInit(void)
{  
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

    LPUART_Init((LPUART_Type *)APPLICATION_SERIAL_BASE, &config, CLOCK_GetFreq(kCLOCK_Lpuart5Clk));

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
#define DEMO_ADC_CHANNEL0       54U
#define DEMO_ADC_CHANNEL1       55U
#define DEMO_ADC_CHANNEL2       48U

adc_channel_config_t adcChannelConfig[3U] = {
    {
        .channelIndex      = DEMO_ADC_CHANNEL0,
        .enableInt         = false,
        .enablePresample   = false,
        .enableDmaTransfer = false,
        .enableWdg         = false,
        .wdgIndex          = 0U,
    },
    {
        .channelIndex      = DEMO_ADC_CHANNEL1,
        .enableInt         = false,
        .enablePresample   = false,
        .enableDmaTransfer = false,
        .enableWdg         = false,
        .wdgIndex          = 0U,
    },
    {
        .channelIndex      = DEMO_ADC_CHANNEL2,
        .enableInt         = false,
        .enablePresample   = false,
        .enableDmaTransfer = false,
        .enableWdg         = false,
        .wdgIndex          = 0U,
    },
};
