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
    CLOCK_EnableClock(kCLOCK_GateAonPORT);

    /* 10M to system */
    CLOCK_AttachClk(kFROdiv1_to_AON_CPU);
    CLOCK_SetClockDiv(kCLOCK_DIVAonCPU, 1U);
    CLOCK_SetupFROAonClocking(10000000U);
    CLOCK_EnableClock(kCLOCK_GateAonAPB);

    /* 2.5M to I2C & UART*/
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
    
    /* 2.5M to CMP0 */
    CLOCK_AttachClk(kFROdiv4_to_AON_CMP0);
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

    SystemCoreClock = 10000000U;

    CLOCK_ConfigureCoreVoltage();
}

/*!
 * @brief Configures CM0+ core voltage based on system core frequency.
 *
 * Read the current CM0+ core frequency and adjusts the core voltage
 * using the PMU driver to ensure stable operation.
 * - 2 MHz: 0.7V
 * - 10 MHz: 0.8V
 *
 */
void CLOCK_ConfigureCoreVoltage(void)
{
    uint32_t freq = CLOCK_GetAonCoreSysClkFreq();

    if (freq <= 2000000U) 
    {
#if !defined(ADVC_DRIVER_USED) || !ADVC_DRIVER_USED
        // Set 0.7V for Active Mode
        PMU_UpdateVDDCoreInActiveMode(AON__PMU, VDD_CORE_AON_0_7V_VOLTAGE);
#endif
    }
    else if (freq <= 10000000U)
    {
#if !defined(ADVC_DRIVER_USED) || !ADVC_DRIVER_USED
        // Set 0.8V for Active Mode
        PMU_UpdateVDDCoreInActiveMode(AON__PMU, VDD_CORE_AON_0_8V_VOLTAGE);
#endif
    }
}

/*******************************************************************************
 ******************** Configuration BOARD_BootClockFRO40M **********************
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockFRO40M
called_from_default_init: true
outputs:
- {id: CLK_16K.outFreq, value: 16.384 kHz}
- {id: CLK_1M.outFreq, value: 1 MHz}
- {id: CPU_clock.outFreq, value: 40 MHz}
- {id: FLASH_CLK.outFreq, value: 12 MHz}
- {id: FRO_12M_clock.outFreq, value: 12 MHz}
- {id: FRO_HF_clock.outFreq, value: 40 MHz}
- {id: MAIN_CLK.outFreq, value: 40 MHz}
- {id: MRCC.FREQME_reference_clock.outFreq, value: 12 MHz}
- {id: MRCC.FREQME_target_clock.outFreq, value: 12 MHz}
- {id: MRCC.MRCC_CMP0_FUNC_CLK.outFreq, value: 40 MHz}
- {id: MRCC.MRCC_DBG_TRACE_CLK.outFreq, value: 40 MHz}
- {id: MRCC.MRCC_LPTMR0_CLK.outFreq, value: 12 MHz}
- {id: MRCC.MRCC_OSTIMER0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.MRCC_UTICK0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.MRCC_WWDT0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.WUU_clock.outFreq, value: 16.384 kHz}
- {id: MRCC_FRO_HF_DIV_CLK.outFreq, value: 40 MHz}
- {id: ROSC_CLK.outFreq, value: 32.768 kHz}
- {id: SCG.FIRC_TRIM_CLK.outFreq, value: 32.768 kHz}
- {id: SCG.SIRC_TRIM_CLK.outFreq, value: 32.768 kHz}
- {id: Slow_clock.outFreq, value: 10 MHz}
- {id: System_clock.outFreq, value: 40 MHz}
settings:
- {id: SCGMode, value: FIRC}
- {id: CMP0FUNCCLKDIV_HALT, value: Enable}
- {id: DBGTRACECLKDIV_HALT, value: Enable}
- {id: LPTMR0CLKDIV_HALT, value: Enable}
- {id: MRCC.MRCC_ADC0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_CTIMER0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_OSTIMER0_SEL.sel, value: SCG.CLK_1M}
- {id: MRCC.MRCC_PERIPH_GROUP_0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_PERIPH_GROUP_1_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_SYSTICK_SEL.sel, value: SYSCON.CPU_CLK}
- {id: SCG.SCSSEL.sel, value: SCG.FIRC_SEL}
- {id: WWDT0CLKDIV_HALT, value: Enable}
sources:
- {id: AON.ROSC.outFreq, value: 32.768 kHz, enabled: true}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Variables for BOARD_BootClockFRO40M configuration
 ******************************************************************************/
/*******************************************************************************
 * Code for BOARD_BootClockFRO40M configuration
 ******************************************************************************/
#if 0

void BOARD_BootClockFRO40M(void)
{
    /* Config 32k Crystal Oscillator */
    /* Monitor is disabled */
    CLOCK_SetRoscMonitorMode(kSCG_RoscMonitorDisable);
    CLOCK_SetXtal32Freq(BOARD_BOOTCLOCKFRO40M_ROSC_CLOCK);

    CLOCK_SetupFROHFClocking(40000000U, 0U);                    /* Setup FRO HF clock */

    CLOCK_SetupFRO12MClocking();                                /* Setup FRO12M clock */

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);                  /* !< Switch MAIN_CLK to FRO_HF */
    CLOCK_AttachClk(kFIRC_to_FRO_HF);                      /* !< Switch FRO_HF to FIRC */
    CLOCK_AttachClk(kCPU_CLK_to_SYSTICK);                  /* !< Switch SYSTICK to CPU_CLK */
    CLOCK_AttachClk(kCLK_1M_to_OSTIMER0);                  /* !< Switch OSTIMER0 to CLK_1M */
    CLOCK_AttachClk(kSLOW_CLK_to_CLKOUT);                  /* !< Switch CLKOUT to SLOW_CLK */
    CLOCK_AttachClk(kFRO_HF_DIV_to_ADC0);                  /* !< Switch ADC0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_UTICK0);                    /* !< Switch UTICK0 to CLK_1M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CTIMER0);               /* !< Switch CTIMER0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_CTIMER1);                   /* !< Switch CTIMER1 to CLK_1M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CMP0);                  /* !< Switch CMP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP0);         /* !< Switch PERIPH_GROUP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP1);         /* !< Switch PERIPH_GROUP1 to FRO_HF_DIV */

    /* Configure FREQME clock */
    CLOCK_EnableClock(kCLOCK_InputMux);
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);
    INPUTMUX0->FREQMEAS_REF = INPUTMUX_FREQMEAS_REF_INP(2);
    INPUTMUX0->FREQMEAS_TAR = INPUTMUX_FREQMEAS_TAR_INP(2);

    /*!< Set up dividers */
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);               /* !< Set AHBCLKDIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);           /* !< Set MRCC_FRO_HF_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivWWDT0, 1U);                /* !< Set MRCC_WWDT0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivCMP0_FUNC, 1U);            /* !< Set MRCC_CMP0_FUNC_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivTRACE, 1U);                /* !< Set MRCC_DBG_TRACE_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivLPTMR0, 1U);               /* !< Set MRCC_LPTMR0_DIV divider to value 1 */

    /* Set SystemCoreClock variable */
    SystemCoreClock = BOARD_BOOTCLOCKFRO40M_CORE_CLOCK;
}

/*******************************************************************************
 ******************** Configuration BOARD_BootClockFRO36M **********************
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockFRO36M
outputs:
- {id: CLK_16K.outFreq, value: 16.384 kHz}
- {id: CLK_1M.outFreq, value: 1 MHz}
- {id: CPU_clock.outFreq, value: 36 MHz}
- {id: FLASH_CLK.outFreq, value: 12 MHz}
- {id: FRO_12M_clock.outFreq, value: 12 MHz}
- {id: FRO_HF_clock.outFreq, value: 36 MHz}
- {id: MAIN_CLK.outFreq, value: 36 MHz}
- {id: MRCC.FREQME_reference_clock.outFreq, value: 12 MHz}
- {id: MRCC.FREQME_target_clock.outFreq, value: 12 MHz}
- {id: MRCC.MRCC_CMP0_FUNC_CLK.outFreq, value: 36 MHz}
- {id: MRCC.MRCC_DBG_TRACE_CLK.outFreq, value: 36 MHz}
- {id: MRCC.MRCC_LPTMR0_CLK.outFreq, value: 12 MHz}
- {id: MRCC.MRCC_OSTIMER0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.MRCC_UTICK0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.MRCC_WWDT0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.WUU_clock.outFreq, value: 16.384 kHz}
- {id: MRCC_FRO_HF_DIV_CLK.outFreq, value: 36 MHz}
- {id: ROSC_CLK.outFreq, value: 32.768 kHz}
- {id: SCG.FIRC_TRIM_CLK.outFreq, value: 32.768 kHz}
- {id: SCG.SIRC_TRIM_CLK.outFreq, value: 32.768 kHz}
- {id: Slow_clock.outFreq, value: 9 MHz}
- {id: System_clock.outFreq, value: 36 MHz}
settings:
- {id: SCGMode, value: FIRC}
- {id: CMP0FUNCCLKDIV_HALT, value: Enable}
- {id: DBGTRACECLKDIV_HALT, value: Enable}
- {id: LPTMR0CLKDIV_HALT, value: Enable}
- {id: MRCC.MRCC_ADC0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_CTIMER0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_OSTIMER0_SEL.sel, value: SCG.CLK_1M}
- {id: MRCC.MRCC_PERIPH_GROUP_0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_PERIPH_GROUP_1_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_SYSTICK_SEL.sel, value: SYSCON.CPU_CLK}
- {id: SCG.SCSSEL.sel, value: SCG.FIRC_SEL}
- {id: WWDT0CLKDIV_HALT, value: Enable}
sources:
- {id: AON.ROSC.outFreq, value: 32.768 kHz, enabled: true}
- {id: SCG.FIRC.outFreq, value: 36 MHz}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Variables for BOARD_BootClockFRO36M configuration
 ******************************************************************************/
/*******************************************************************************
 * Code for BOARD_BootClockFRO36M configuration
 ******************************************************************************/
void BOARD_BootClockFRO36M(void)
{
    /* Config 32k Crystal Oscillator */
    /* Monitor is disabled */
    CLOCK_SetRoscMonitorMode(kSCG_RoscMonitorDisable);
    CLOCK_SetXtal32Freq(BOARD_BOOTCLOCKFRO36M_ROSC_CLOCK);

    CLOCK_SetupFROHFClocking(36000000U, 0U);                    /* Setup FRO HF clock */

    CLOCK_SetupFRO12MClocking();                                /* Setup FRO12M clock */

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);                  /* !< Switch MAIN_CLK to FRO_HF */
    CLOCK_AttachClk(kFIRC_to_FRO_HF);                      /* !< Switch FRO_HF to FIRC */
    CLOCK_AttachClk(kCPU_CLK_to_SYSTICK);                  /* !< Switch SYSTICK to CPU_CLK */
    CLOCK_AttachClk(kCLK_1M_to_OSTIMER0);                  /* !< Switch OSTIMER0 to CLK_1M */
    CLOCK_AttachClk(kSLOW_CLK_to_CLKOUT);                  /* !< Switch CLKOUT to SLOW_CLK */
    CLOCK_AttachClk(kFRO_HF_DIV_to_ADC0);                  /* !< Switch ADC0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_UTICK0);                    /* !< Switch UTICK0 to CLK_1M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CTIMER0);               /* !< Switch CTIMER0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_CTIMER1);                   /* !< Switch CTIMER1 to CLK_1M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CMP0);                  /* !< Switch CMP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP0);         /* !< Switch PERIPH_GROUP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP1);         /* !< Switch PERIPH_GROUP1 to FRO_HF_DIV */

    /* Configure FREQME clock */
    CLOCK_EnableClock(kCLOCK_InputMux);
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);
    INPUTMUX0->FREQMEAS_REF = INPUTMUX_FREQMEAS_REF_INP(2);
    INPUTMUX0->FREQMEAS_TAR = INPUTMUX_FREQMEAS_TAR_INP(2);

    /*!< Set up dividers */
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);               /* !< Set AHBCLKDIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);           /* !< Set MRCC_FRO_HF_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivWWDT0, 1U);                /* !< Set MRCC_WWDT0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivCMP0_FUNC, 1U);            /* !< Set MRCC_CMP0_FUNC_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivTRACE, 1U);                /* !< Set MRCC_DBG_TRACE_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivLPTMR0, 1U);               /* !< Set MRCC_LPTMR0_DIV divider to value 1 */

    /* Set SystemCoreClock variable */
    SystemCoreClock = BOARD_BOOTCLOCKFRO36M_CORE_CLOCK;
}

/*******************************************************************************
 ******************** Configuration BOARD_BootClockFRO20M **********************
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockFRO20M
outputs:
- {id: CLK_16K.outFreq, value: 16.384 kHz}
- {id: CLK_1M.outFreq, value: 1 MHz}
- {id: CPU_clock.outFreq, value: 20 MHz}
- {id: FLASH_CLK.outFreq, value: 12 MHz}
- {id: FRO_12M_clock.outFreq, value: 12 MHz}
- {id: FRO_HF_clock.outFreq, value: 20 MHz}
- {id: MAIN_CLK.outFreq, value: 20 MHz}
- {id: MRCC.FREQME_reference_clock.outFreq, value: 12 MHz}
- {id: MRCC.FREQME_target_clock.outFreq, value: 12 MHz}
- {id: MRCC.MRCC_CMP0_FUNC_CLK.outFreq, value: 20 MHz}
- {id: MRCC.MRCC_DBG_TRACE_CLK.outFreq, value: 20 MHz}
- {id: MRCC.MRCC_LPTMR0_CLK.outFreq, value: 12 MHz}
- {id: MRCC.MRCC_OSTIMER0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.MRCC_UTICK0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.MRCC_WWDT0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.WUU_clock.outFreq, value: 16.384 kHz}
- {id: MRCC_FRO_HF_DIV_CLK.outFreq, value: 20 MHz}
- {id: ROSC_CLK.outFreq, value: 32.768 kHz}
- {id: SCG.FIRC_TRIM_CLK.outFreq, value: 32.768 kHz}
- {id: SCG.SIRC_TRIM_CLK.outFreq, value: 32.768 kHz}
- {id: Slow_clock.outFreq, value: 5 MHz}
- {id: System_clock.outFreq, value: 20 MHz}
settings:
- {id: SCGMode, value: FIRC}
- {id: CMP0FUNCCLKDIV_HALT, value: Enable}
- {id: DBGTRACECLKDIV_HALT, value: Enable}
- {id: LPTMR0CLKDIV_HALT, value: Enable}
- {id: MRCC.MRCC_ADC0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_CTIMER0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_OSTIMER0_SEL.sel, value: SCG.CLK_1M}
- {id: MRCC.MRCC_PERIPH_GROUP_0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_PERIPH_GROUP_1_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_SYSTICK_SEL.sel, value: SYSCON.CPU_CLK}
- {id: SCG.FIRC_SEL.sel, value: SCG.FIRC_DIV2}
- {id: SCG.SCSSEL.sel, value: SCG.FIRC_SEL}
- {id: WWDT0CLKDIV_HALT, value: Enable}
sources:
- {id: AON.ROSC.outFreq, value: 32.768 kHz, enabled: true}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Variables for BOARD_BootClockFRO20M configuration
 ******************************************************************************/
/*******************************************************************************
 * Code for BOARD_BootClockFRO20M configuration
 ******************************************************************************/
void BOARD_BootClockFRO20M(void)
{
    /* Config 32k Crystal Oscillator */
    /* Monitor is disabled */
    CLOCK_SetRoscMonitorMode(kSCG_RoscMonitorDisable);
    CLOCK_SetXtal32Freq(BOARD_BOOTCLOCKFRO20M_ROSC_CLOCK);

    CLOCK_SetupFROHFClocking(40000000U, 1U);                    /* Setup FRO HF clock */

    CLOCK_SetupFRO12MClocking();                                /* Setup FRO12M clock */

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);                  /* !< Switch MAIN_CLK to FRO_HF */
    CLOCK_AttachClk(kFIRC_DIV_to_FRO_HF);                  /* !< Switch FRO_HF to FIRC_DIV */
    CLOCK_AttachClk(kCPU_CLK_to_SYSTICK);                  /* !< Switch SYSTICK to CPU_CLK */
    CLOCK_AttachClk(kCLK_1M_to_OSTIMER0);                  /* !< Switch OSTIMER0 to CLK_1M */
    CLOCK_AttachClk(kSLOW_CLK_to_CLKOUT);                  /* !< Switch CLKOUT to SLOW_CLK */
    CLOCK_AttachClk(kFRO_HF_DIV_to_ADC0);                  /* !< Switch ADC0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_UTICK0);                    /* !< Switch UTICK0 to CLK_1M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CTIMER0);               /* !< Switch CTIMER0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_CTIMER1);                   /* !< Switch CTIMER1 to CLK_1M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CMP0);                  /* !< Switch CMP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP0);         /* !< Switch PERIPH_GROUP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP1);         /* !< Switch PERIPH_GROUP1 to FRO_HF_DIV */

    /* Configure FREQME clock */
    CLOCK_EnableClock(kCLOCK_InputMux);
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);
    INPUTMUX0->FREQMEAS_REF = INPUTMUX_FREQMEAS_REF_INP(2);
    INPUTMUX0->FREQMEAS_TAR = INPUTMUX_FREQMEAS_TAR_INP(2);

    /*!< Set up dividers */
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);               /* !< Set AHBCLKDIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);           /* !< Set MRCC_FRO_HF_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivWWDT0, 1U);                /* !< Set MRCC_WWDT0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivCMP0_FUNC, 1U);            /* !< Set MRCC_CMP0_FUNC_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivTRACE, 1U);                /* !< Set MRCC_DBG_TRACE_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivLPTMR0, 1U);               /* !< Set MRCC_LPTMR0_DIV divider to value 1 */

    /* Set SystemCoreClock variable */
    SystemCoreClock = BOARD_BOOTCLOCKFRO20M_CORE_CLOCK;
}

/*******************************************************************************
 ******************** Configuration BOARD_BootClockFRO18M **********************
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockFRO18M
outputs:
- {id: CLK_16K.outFreq, value: 16.384 kHz}
- {id: CLK_1M.outFreq, value: 1 MHz}
- {id: CPU_clock.outFreq, value: 18 MHz}
- {id: FLASH_CLK.outFreq, value: 12 MHz}
- {id: FRO_12M_clock.outFreq, value: 12 MHz}
- {id: FRO_HF_clock.outFreq, value: 18 MHz}
- {id: MAIN_CLK.outFreq, value: 18 MHz}
- {id: MRCC.FREQME_reference_clock.outFreq, value: 12 MHz}
- {id: MRCC.FREQME_target_clock.outFreq, value: 12 MHz}
- {id: MRCC.MRCC_CMP0_FUNC_CLK.outFreq, value: 18 MHz}
- {id: MRCC.MRCC_DBG_TRACE_CLK.outFreq, value: 18 MHz}
- {id: MRCC.MRCC_LPTMR0_CLK.outFreq, value: 12 MHz}
- {id: MRCC.MRCC_OSTIMER0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.MRCC_UTICK0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.MRCC_WWDT0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.WUU_clock.outFreq, value: 16.384 kHz}
- {id: MRCC_FRO_HF_DIV_CLK.outFreq, value: 18 MHz}
- {id: ROSC_CLK.outFreq, value: 32.768 kHz}
- {id: SCG.FIRC_TRIM_CLK.outFreq, value: 32.768 kHz}
- {id: SCG.SIRC_TRIM_CLK.outFreq, value: 32.768 kHz}
- {id: Slow_clock.outFreq, value: 4.5 MHz}
- {id: System_clock.outFreq, value: 18 MHz}
settings:
- {id: SCGMode, value: FIRC}
- {id: CMP0FUNCCLKDIV_HALT, value: Enable}
- {id: DBGTRACECLKDIV_HALT, value: Enable}
- {id: LPTMR0CLKDIV_HALT, value: Enable}
- {id: MRCC.MRCC_ADC0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_CTIMER0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_OSTIMER0_SEL.sel, value: SCG.CLK_1M}
- {id: MRCC.MRCC_PERIPH_GROUP_0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_PERIPH_GROUP_1_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_SYSTICK_SEL.sel, value: SYSCON.CPU_CLK}
- {id: SCG.FIRC_SEL.sel, value: SCG.FIRC_DIV2}
- {id: SCG.SCSSEL.sel, value: SCG.FIRC_SEL}
- {id: WWDT0CLKDIV_HALT, value: Enable}
sources:
- {id: AON.ROSC.outFreq, value: 32.768 kHz, enabled: true}
- {id: SCG.FIRC.outFreq, value: 36 MHz}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Variables for BOARD_BootClockFRO18M configuration
 ******************************************************************************/
/*******************************************************************************
 * Code for BOARD_BootClockFRO18M configuration
 ******************************************************************************/
void BOARD_BootClockFRO18M(void)
{
    /* Config 32k Crystal Oscillator */
    /* Monitor is disabled */
    CLOCK_SetRoscMonitorMode(kSCG_RoscMonitorDisable);
    CLOCK_SetXtal32Freq(BOARD_BOOTCLOCKFRO18M_ROSC_CLOCK);

    CLOCK_SetupFROHFClocking(36000000U, 1U);                    /* Setup FRO HF clock */

    CLOCK_SetupFRO12MClocking();                                /* Setup FRO12M clock */

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);                  /* !< Switch MAIN_CLK to FRO_HF */
    CLOCK_AttachClk(kFIRC_DIV_to_FRO_HF);                  /* !< Switch FRO_HF to FIRC_DIV */
    CLOCK_AttachClk(kCPU_CLK_to_SYSTICK);                  /* !< Switch SYSTICK to CPU_CLK */
    CLOCK_AttachClk(kCLK_1M_to_OSTIMER0);                  /* !< Switch OSTIMER0 to CLK_1M */
    CLOCK_AttachClk(kSLOW_CLK_to_CLKOUT);                  /* !< Switch CLKOUT to SLOW_CLK */
    CLOCK_AttachClk(kFRO_HF_DIV_to_ADC0);                  /* !< Switch ADC0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_UTICK0);                    /* !< Switch UTICK0 to CLK_1M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CTIMER0);               /* !< Switch CTIMER0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_CTIMER1);                   /* !< Switch CTIMER1 to CLK_1M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CMP0);                  /* !< Switch CMP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP0);         /* !< Switch PERIPH_GROUP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP1);         /* !< Switch PERIPH_GROUP1 to FRO_HF_DIV */

    /* Configure FREQME clock */
    CLOCK_EnableClock(kCLOCK_InputMux);
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);
    INPUTMUX0->FREQMEAS_REF = INPUTMUX_FREQMEAS_REF_INP(2);
    INPUTMUX0->FREQMEAS_TAR = INPUTMUX_FREQMEAS_TAR_INP(2);

    /*!< Set up dividers */
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);               /* !< Set AHBCLKDIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);           /* !< Set MRCC_FRO_HF_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivWWDT0, 1U);                /* !< Set MRCC_WWDT0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivCMP0_FUNC, 1U);            /* !< Set MRCC_CMP0_FUNC_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivTRACE, 1U);                /* !< Set MRCC_DBG_TRACE_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivLPTMR0, 1U);               /* !< Set MRCC_LPTMR0_DIV divider to value 1 */

    /* Set SystemCoreClock variable */
    SystemCoreClock = BOARD_BOOTCLOCKFRO18M_CORE_CLOCK;
}

/*******************************************************************************
 ******************** Configuration BOARD_BootClockFRO12M **********************
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockFRO12M
outputs:
- {id: CLK_16K.outFreq, value: 16.384 kHz}
- {id: CLK_1M.outFreq, value: 1 MHz}
- {id: CPU_clock.outFreq, value: 12 MHz}
- {id: FLASH_CLK.outFreq, value: 12 MHz}
- {id: FRO_12M_clock.outFreq, value: 12 MHz}
- {id: FRO_HF_clock.outFreq, value: 40 MHz}
- {id: MAIN_CLK.outFreq, value: 12 MHz}
- {id: MRCC.FREQME_reference_clock.outFreq, value: 12 MHz}
- {id: MRCC.FREQME_target_clock.outFreq, value: 12 MHz}
- {id: MRCC.MRCC_CMP0_FUNC_CLK.outFreq, value: 40 MHz}
- {id: MRCC.MRCC_DBG_TRACE_CLK.outFreq, value: 12 MHz}
- {id: MRCC.MRCC_LPTMR0_CLK.outFreq, value: 12 MHz}
- {id: MRCC.MRCC_OSTIMER0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.MRCC_UTICK0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.MRCC_WWDT0_CLK.outFreq, value: 1 MHz}
- {id: MRCC.WUU_clock.outFreq, value: 16.384 kHz}
- {id: MRCC_FRO_HF_DIV_CLK.outFreq, value: 40 MHz}
- {id: ROSC_CLK.outFreq, value: 32.768 kHz}
- {id: SCG.FIRC_TRIM_CLK.outFreq, value: 32.768 kHz}
- {id: SCG.SIRC_TRIM_CLK.outFreq, value: 32.768 kHz}
- {id: Slow_clock.outFreq, value: 3 MHz}
- {id: System_clock.outFreq, value: 12 MHz}
settings:
- {id: CMP0FUNCCLKDIV_HALT, value: Enable}
- {id: DBGTRACECLKDIV_HALT, value: Enable}
- {id: LPTMR0CLKDIV_HALT, value: Enable}
- {id: MRCC.MRCC_ADC0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_CTIMER0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_OSTIMER0_SEL.sel, value: SCG.CLK_1M}
- {id: MRCC.MRCC_PERIPH_GROUP_0_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_PERIPH_GROUP_1_SEL.sel, value: MRCC.FRO_HF_DIVCLK}
- {id: MRCC.MRCC_SYSTICK_SEL.sel, value: SYSCON.CPU_CLK}
- {id: WWDT0CLKDIV_HALT, value: Enable}
sources:
- {id: AON.ROSC.outFreq, value: 32.768 kHz, enabled: true}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Variables for BOARD_BootClockFRO12M configuration
 ******************************************************************************/
/*******************************************************************************
 * Code for BOARD_BootClockFRO12M configuration
 ******************************************************************************/
void BOARD_BootClockFRO12M(void)
{
    /* Config 32k Crystal Oscillator */
    /* Monitor is disabled */
    CLOCK_SetRoscMonitorMode(kSCG_RoscMonitorDisable);
    CLOCK_SetXtal32Freq(BOARD_BOOTCLOCKFRO12M_ROSC_CLOCK);

    CLOCK_SetupFROHFClocking(40000000U, 0U);                    /* Setup FRO HF clock */

    CLOCK_SetupFRO12MClocking();                                /* Setup FRO12M clock */

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);                  /* !< Switch MAIN_CLK to FRO12M */
    CLOCK_AttachClk(kFIRC_to_FRO_HF);                      /* !< Switch FRO_HF to FIRC */
    CLOCK_AttachClk(kCPU_CLK_to_SYSTICK);                  /* !< Switch SYSTICK to CPU_CLK */
    CLOCK_AttachClk(kCLK_1M_to_OSTIMER0);                  /* !< Switch OSTIMER0 to CLK_1M */
    CLOCK_AttachClk(kSLOW_CLK_to_CLKOUT);                  /* !< Switch CLKOUT to SLOW_CLK */
    CLOCK_AttachClk(kFRO_HF_DIV_to_ADC0);                  /* !< Switch ADC0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_UTICK0);                    /* !< Switch UTICK0 to CLK_1M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CTIMER0);               /* !< Switch CTIMER0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_CTIMER1);                   /* !< Switch CTIMER1 to CLK_1M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CMP0);                  /* !< Switch CMP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP0);         /* !< Switch PERIPH_GROUP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP1);         /* !< Switch PERIPH_GROUP1 to FRO_HF_DIV */

    /* Configure FREQME clock */
    CLOCK_EnableClock(kCLOCK_InputMux);
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);
    INPUTMUX0->FREQMEAS_REF = INPUTMUX_FREQMEAS_REF_INP(2);
    INPUTMUX0->FREQMEAS_TAR = INPUTMUX_FREQMEAS_TAR_INP(2);

    /*!< Set up dividers */
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);               /* !< Set AHBCLKDIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);           /* !< Set MRCC_FRO_HF_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivWWDT0, 1U);                /* !< Set MRCC_WWDT0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivCMP0_FUNC, 1U);            /* !< Set MRCC_CMP0_FUNC_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivTRACE, 1U);                /* !< Set MRCC_DBG_TRACE_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivLPTMR0, 1U);               /* !< Set MRCC_LPTMR0_DIV divider to value 1 */

    /* Set SystemCoreClock variable */
    SystemCoreClock = BOARD_BOOTCLOCKFRO12M_CORE_CLOCK;
}
#endif /* #if 0 */