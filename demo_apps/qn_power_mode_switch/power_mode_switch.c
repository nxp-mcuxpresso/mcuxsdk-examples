/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fsl_debug_console.h"
#include "app.h"
#include "fsl_rtc.h"
#include "fsl_power.h"
#include "power_mode_switch.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_iocon.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RTC_INTERRUPT_THRESHOLD (32768U * 10U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_WakeupRestore(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t g_RtcFreeRunningFlag;

/*******************************************************************************
 * Code
 ******************************************************************************/

void delay(uint32_t dly)
{
    volatile uint32_t i;

    for (i = 0U; i < dly; ++i)
    {
        __asm("NOP");
    }
}

void RTC_FR_IRQHandler(void)
{
    //    if (RTC_GetStatusFlags(RTC) & (uint32_t)kRTC_FreeRunningInterruptFlag)
    {
        g_RtcFreeRunningFlag = 1U;
    }
    SDK_ISR_EXIT_BARRIER;
}

static void switch_to_OSC32M(void)
{
    POWER_WritePmuCtrl1(SYSCON, SYSCON_PMU_CTRL1_OSC32M_DIS_MASK, SYSCON_PMU_CTRL1_OSC32M_DIS(0U));
    SYSCON->CLK_CTRL = (SYSCON->CLK_CTRL & ~SYSCON_CLK_CTRL_SYS_CLK_SEL_MASK) | SYSCON_CLK_CTRL_SYS_CLK_SEL(0U);
}

static void switch_to_XTAL(void)
{
    /* switch to XTAL after it is stable */
    while (!(SYSCON_SYS_MODE_CTRL_XTAL_RDY_MASK & SYSCON->SYS_MODE_CTRL))
        ;
    SYSCON->CLK_CTRL = (SYSCON->CLK_CTRL & ~SYSCON_CLK_CTRL_SYS_CLK_SEL_MASK) | SYSCON_CLK_CTRL_SYS_CLK_SEL(1U);
    POWER_WritePmuCtrl1(SYSCON, SYSCON_PMU_CTRL1_OSC32M_DIS_MASK, SYSCON_PMU_CTRL1_OSC32M_DIS(1U));
}

void wakeup_by_capa_sensor(void)
{
    /* config PA15 as cs's input */
    IOCON_PinMuxSet(IOCON, 0U, 15U, IOCON_FUNC1);
    /* IOCON_PinMuxSet(IOCON, 0U, 14U, IOCON_FUNC1); */

    /* config PA15 to High-Z */
    SYSCON->PIO_PULL_CFG[0] &= ~0xC0000000U;

    CLOCK_EnableClock(kCLOCK_Cs);

    POWER_WritePmuCtrl1(SYSCON, SYSCON_PMU_CTRL1_CAP_SEN_DIS_MASK, SYSCON_PMU_CTRL1_CAP_SEN_DIS(0U));

    CS->CTRL0 =
        (CS->CTRL0 & ~(CS_CTRL0_CLK_DIV_MASK | CS_CTRL0_OSC_FREQ_MASK | CS_CTRL0_SRST_MASK | CS_CTRL0_ENABLE_MASK)) |
        (79U << CS_CTRL0_CLK_DIV_SHIFT) | (10U << CS_CTRL0_OSC_FREQ_SHIFT) | (0U << CS_CTRL0_CLK_DIV_SHIFT) |
        CS_CTRL0_ENABLE_MASK;

    CS->LP_CTRL = (CS->LP_CTRL & ~(CS_LP_CTRL_THR_MASK | CS_LP_CTRL_LP_CH_MASK | CS_LP_CTRL_LP_EN_MASK |
                                   CS_LP_CTRL_DEBONCE_NUM_MASK)) |
                  (600U << CS_LP_CTRL_THR_SHIFT) | (1U << CS_LP_CTRL_LP_CH_SHIFT) | CS_LP_CTRL_LP_EN_MASK |
                  (10U << CS_LP_CTRL_DEBONCE_NUM_SHIFT);

    /* after each scan loop, idle for some time. unit: 10ms */
    /* CS->IDLE_PERIOD = 50U; */

    CS->LP_INTEN = CS_LP_INTEN_LP_INTEN_MASK;
}

void CS_WAKEUP_IRQHandler(void)
{
    CS->LP_CTRL &= ~CS_LP_CTRL_LP_EN_MASK;
    NVIC_DisableIRQ(CS_WAKEUP_IRQn);
    NVIC_ClearPendingIRQ(CS_WAKEUP_IRQn);
    SDK_ISR_EXIT_BARRIER;
}

/* #define CAP_SENSOR_WAKEUP */

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t ch;

    BOARD_InitHardware();

    RTC_Init(RTC);

    /* Enable RTC free running interrupt */
    RTC_EnableInterrupts(RTC, kRTC_FreeRunningInterruptEnable);
    /* Enable at the NVIC */
    NVIC_EnableIRQ(RTC_FR_IRQn);

    /* Enable gpio wakeup at button2 */
    SYSCON->PIO_WAKEUP_LVL0 = SYSCON->PIO_WAKEUP_LVL0 | BOARD_SW2_GPIO_PIN_MASK;
    SYSCON->PIO_WAKEUP_EN0  = SYSCON->PIO_WAKEUP_EN0 | BOARD_SW2_GPIO_PIN_MASK;

    g_RtcFreeRunningFlag = 0U;
#if defined(CAP_SENSOR_WAKEUP)
    wakeup_by_capa_sensor();
#endif
    PRINTF("\r\n####################  Power Mode Switch Demo ####################\n\r\n");
    PRINTF("    Sys Clock = %dHz \r\n", CLOCK_GetFreq(kCLOCK_CoreSysClk));
    PRINTF("    Ahb Clock = %dHz \r\n", CLOCK_GetFreq(kCLOCK_BusClk));

    while (1)
    {
        if (g_RtcFreeRunningFlag)
        {
            g_RtcFreeRunningFlag = 0U;
            PRINTF("Waked up by rtc free running interrupt.\r\n");
        }

        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press %c to enter: Active        - Normal RUN mode\r\n", kPmActive + 'a');
        PRINTF("Press %c to enter: Sleep         - Sleep mode\r\n", kPmSleep + 'a');
        PRINTF("Press %c to enter: Power down 0  - Power down 0 mode\r\n", kPmPowerDown0 + 'a');
        PRINTF("Press %c to enter: Power down 1  - Power down 1 mode\r\n", kPmPowerDown1 + 'a');

        PRINTF("\r\nWaiting for power mode select...\r\n\r\n");

        /* Wait for user input */
        ch = GETCHAR();
        PRINTF("\r\n");

        RTC_SetFreeRunningInterruptThreshold(RTC, RTC_GetFreeRunningCount(RTC) + RTC_INTERRUPT_THRESHOLD);
        RTC_FreeRunningEnable(RTC, true);

        if ((ch >= 'A') && (ch <= 'Z'))
        {
            ch += 'a' - 'A';
        }

        switch ((power_mode_t)(ch - 'a'))
        {
            case kPmActive:
                PRINTF(" Now in active mode for about 10 seconds.\r\n");
                delay(500000);
                CLOCK_DisableClock(kCLOCK_Flexcomm0);
                delay(10000000);
                CLOCK_EnableClock(kCLOCK_Flexcomm0);
                break;
            case kPmSleep:
                PRINTF(" Now in sleep mode for about 10 seconds.\r\n");
                delay(500000);
                CLOCK_DisableClock(kCLOCK_Flexcomm0);
                POWER_EnterSleep();
                CLOCK_EnableClock(kCLOCK_Flexcomm0);
                break;
            case kPmPowerDown0:
                PRINTF(" Now in power down 0 mode for about 10 seconds.\r\n");
                __disable_irq();
                switch_to_OSC32M();
#if defined(CAP_SENSOR_WAKEUP)
                NVIC_ClearPendingIRQ(CS_WAKEUP_IRQn);
                NVIC_EnableIRQ(CS_WAKEUP_IRQn);
                CS->LP_CTRL |= CS_LP_CTRL_LP_EN_MASK;
#endif
                POWER_LatchIO();
                CLOCK_DisableClock(kCLOCK_Flexcomm0);
                POWER_EnterPowerDown(0);
                CLOCK_EnableClock(kCLOCK_Flexcomm0);
                POWER_RestoreIO();
                switch_to_XTAL();
                /* after waking up from pwoer down, usart config is lost, recover it */
                BOARD_WakeupRestore();
                __enable_irq();
                break;
            case kPmPowerDown1:
                PRINTF(" Now in power down 1 mode, press button 2 to wakeup.\r\n");
                /* Enable GPIO wakeup */
                NVIC_ClearPendingIRQ(EXT_GPIO_WAKEUP_IRQn);
                NVIC_EnableIRQ(EXT_GPIO_WAKEUP_IRQn);
                __disable_irq();
                switch_to_OSC32M();
#if (defined(BOARD_XTAL1_CLK_HZ) && (BOARD_XTAL1_CLK_HZ == CLK_RCO_32KHZ))
                POWER_WritePmuCtrl1(SYSCON, SYSCON_PMU_CTRL1_RCO32K_DIS_MASK, SYSCON_PMU_CTRL1_RCO32K_DIS(1U));
#elif (defined(BOARD_XTAL1_CLK_HZ) && (BOARD_XTAL1_CLK_HZ == CLK_XTAL_32KHZ))
                POWER_WritePmuCtrl1(SYSCON, SYSCON_PMU_CTRL1_XTAL32K_DIS_MASK, SYSCON_PMU_CTRL1_XTAL32K_DIS(1U));
#endif
                POWER_LatchIO();
                POWER_EnterPowerDown(0);
                POWER_RestoreIO();
                switch_to_XTAL();

                NVIC_DisableIRQ(EXT_GPIO_WAKEUP_IRQn);
                NVIC_ClearPendingIRQ(EXT_GPIO_WAKEUP_IRQn);

#if (defined(BOARD_XTAL1_CLK_HZ) && (BOARD_XTAL1_CLK_HZ == CLK_RCO_32KHZ))
                POWER_WritePmuCtrl1(SYSCON, SYSCON_PMU_CTRL1_RCO32K_DIS_MASK, SYSCON_PMU_CTRL1_RCO32K_DIS(0U));
#elif (defined(BOARD_XTAL1_CLK_HZ) && (BOARD_XTAL1_CLK_HZ == CLK_XTAL_32KHZ))
                POWER_WritePmuCtrl1(SYSCON, SYSCON_PMU_CTRL1_XTAL32K_DIS_MASK, SYSCON_PMU_CTRL1_XTAL32K_DIS(0U));
#endif
                /* after waking up from pwoer down, usart config is lost, recover it */
                BOARD_WakeupRestore();
                __enable_irq();
                break;
            default:
                PRINTF("\r\nInput is invalid!\r\n");
                break;
        }
        RTC_FreeRunningEnable(RTC, false);

        delay(1000000);
        PRINTF("\r\nNext loop\r\n");
    }
}
