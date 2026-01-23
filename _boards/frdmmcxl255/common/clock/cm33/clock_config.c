/*
 * Copyright 2023-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "fsl_clock.h"
#include "fsl_pmu.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void) 
{
    rosc_init_config_t roscInitConfig;

    CLOCK_SetupFROHFClocking(96000000U, 0U);                    /* Setup FRO HF clock */

    CLOCK_ConfigureCoreVoltageAndFlashWaitStates();
    
    /* Set SystemCoreClock variable */
    SystemCoreClock = 96000000U;

    CLOCK_SetupFRO12MClocking();                                /* Setup FRO12M clock */

    CLOCK_EnableClock(kCLOCK_GateAonAPB);                       /* Required for access to AON*/

    /* Monitor is disabled */
    CLOCK_SetRoscMonitorMode(kSCG_RoscMonitorDisable);
    /* Configuration of the 32 kHz crystal oscillator */
    
    /* Initialize Rosc if not already initialized */
    if (!CLOCK_IsRoscInitialized())
    {
        /* Get default Rosc initialization configuration */
        CLOCK_GetDefaultInitRoscConfig(&roscInitConfig);
        
        /* Configure Rosc initialization for FRDM-MCXL255 for faster init*/
        roscInitConfig.detectionDelay = 50U;
        roscInitConfig.detectionTimeout = 0U;
        roscInitConfig.detectionDelaySwitchedMode = 50U;
        roscInitConfig.detectionTimeoutSwitchedMode = 50U;
        
        CLOCK_InitRosc(&roscInitConfig);
    }
    
    /* Enable the 32 kHz crystal oscillator output to the AON and the Main domains. */
    AON__SYSCON_AON->XTAL_32K_CLKCTRL &= ~SYSCON_AON_XTAL_32K_CLKCTRL_XTAL_32K_CLK_CTRL_MASK;
    /* Enable the clock output of the XTAL32K from the AON_RTC. */
    AON__CGU->CLK_CONFIG |= CGU_CLK_CONFIG_XTAL32_OUT_EN_MASK;
    /* Enable the XTAL 32.768KHz towards the CGU. */
    AON__RTC_AON->CONFIG |= RTC_CONFIG_XTAL32_EN_MASK;

    CLOCK_SetupFROAonClocking(10000000);

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kFIRC_to_MAIN_CLK);                  /* !< Switch MAIN_CLK to FIRC */
    CLOCK_AttachClk(kFIRC_to_FRO_HF);                      /* !< Switch FRO_HF to FIRC */
    CLOCK_AttachClk(kCPU_CLK_to_SYSTICK);                  /* !< Switch SYSTICK to CPU_CLK */
    CLOCK_AttachClk(kFRO16K_to_OSTIMER0);                 /* !< Switch OSTIMER0 to FRO_16k */
    CLOCK_AttachClk(kSLOW_CLK_to_CLKOUT);                  /* !< Switch CLKOUT to SLOW_CLK */
    CLOCK_AttachClk(kFRO_HF_DIV_to_ADC0);                  /* !< Switch ADC0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_UTICK0);                    /* !< Switch UTICK0 to CLK_1M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CTIMERg0);               /* !< Switch CTIMER0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CTIMERg1);               /* !< Switch CTIMER1,2 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO12M_to_CMP0);                  /* !< Switch CMP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO12M_to_PERIPH_GROUP0);             /* !< Switch PERIPH_GROUP0 to FRO_12M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP1);         /* !< Switch PERIPH_GROUP1 to FRO_HF_DIV */

    /* Configure FREQME clock */
    /*CLOCK_EnableClock(kCLOCK_InputMux);
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);
    INPUTMUX0->FREQMEAS_REF = INPUTMUX_FREQMEAS_REF_INP(2);
    INPUTMUX0->FREQMEAS_TAR = INPUTMUX_FREQMEAS_TAR_INP(2);*/

    /*!< Set up dividers */
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);               /* !< Set AHBCLKDIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);           /* !< Set MRCC_FRO_HF_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivWWDT0, 1U);                /* !< Set MRCC_WWDT0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivCMP0_FUNC, 1U);            /* !< Set MRCC_CMP0_FUNC_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivTRACE, 1U);                /* !< Set MRCC_DBG_TRACE_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1U);                 /* !< Set MRCC_ADC0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivPeriphGroup0, 1U);         /* !< Set MRCC_LPUART_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivPeriphGroup1, 1U);         /* !< Set MRCC_LPUART_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER0, 1U);              /* !< Set MRCC_CTIMER_grp0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER1, 2U);              /* !< Set MRCC_CTIMER_grp1_DIV divider to value 2 */
    
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP0);
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);

    /** Clocks for AON **/
    CLOCK_EnableClock(kCLOCK_GateAonPORT);

    /* 2.5M to I2C & UART */
    CLOCK_AttachClk(kFROdiv1_to_AON_COM);
    CLOCK_SetClockDiv(kCLOCK_DIVAonCMP, 1U);
    CLOCK_EnableClock(kCLOCK_GateAonUART);

    /* 2.5M to QTMR */
    CLOCK_AttachClk(kFROdiv4_to_AON_TMR);
    CLOCK_EnableClock(kCLOCK_GateAonQTMR0);
    CLOCK_EnableClock(kCLOCK_GateAonQTMR1);

    /* 2.5M (from step above) to LPTMR */
    CLOCK_AttachClk(AON_TMR_to_AON_LPTMR);
    CLOCK_EnableClock(kCLOCK_GateAonLPTMR);

    /* 2.5M to AON CMP0 */
//    CLOCK_AttachClk(kFROdiv4_to_AON_ACMP0);
    CLOCK_EnableClock(kCLOCK_GateAonACMP0);
    CLOCK_SetClockDiv(kCLOCK_DIVAonACMP0CLK0, 1U);
    CLOCK_SetClockDiv(kCLOCK_DIVAonACMP0CLK1, 1U);

    /* 2.5M to to LPADC */
    CLOCK_AttachClk(kFROdiv4_to_AON_LPADC);
    CLOCK_EnableClock(kCLOCK_GateAonLPADC);

    /* PMUIRC fro16k to sLCD */
    CLOCK_AttachClk(kFRO16K_to_AON_KPP);
    CLOCK_EnableClock(kCLOCK_GateAonKPP);

    /* PMUIRC fro16k to KPP */
    CLOCK_AttachClk(kFRO16K_to_AON_LCD);
    CLOCK_EnableClock(kCLOCK_GateAonLCD);

    CLOCK_ConfigureCoreVoltageAndFlashWaitStates();
}

/*!
 * @brief Configures CM33 core voltage and flash wait states based on system core frequency.
 *
 * Read the current CM33 core frequency and adjusts the core voltage
 * using the PMU driver to ensure stable operation. It also sets the appropriate number
 * of flash wait states based on the frequency:
 * - 48 MHz: Mid Drive mode (1.0V), 1 wait state
 * - 96 MHz: Normal Drive mode (1.1V), 2 wait state
 *
 */
void CLOCK_ConfigureCoreVoltageAndFlashWaitStates(void)
{
    uint32_t freq = CLOCK_GetCoreSysClkFreq();

    if (freq <= 48000000U) 
    {
        // Mid Drive Mode 1.0V
        PMU_UpdateVDDCore1P1InActiveMode(AON__PMU, VDD_CORE_MAIN_1V_VOLTAGE);
    }
    else if (freq <= 96000000U)
    {
        // Normal Drive Mode 1.1V
        PMU_UpdateVDDCore1P1InActiveMode(AON__PMU, VDD_CORE_MAIN_1_1V_VOLTAGE);
    }
    CLOCK_SetFlashWaitStateBasedOnFreq(freq);
}
