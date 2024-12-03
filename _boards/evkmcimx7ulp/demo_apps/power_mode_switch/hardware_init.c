/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_msmc.h"
#include "fsl_pmc0.h"
#include "fsl_lpuart.h"
#include "fsl_lptmr.h"
#include "power_mode_switch.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "lpm.h"
#include "app_srtm.h"
/*${header:end}*/

/*${function:start}*/
extern const scg_sys_clk_config_t g_sysClkConfigVlpr;
extern const scg_sys_clk_config_t g_sysClkConfigHsrun;
extern bool disableWirelessPinsInVLLS;
extern bool disableJtagPinsInVLLS;
extern void LLWU0_IRQHandler(void);
extern bool BOARD_IsRunOnQSPI(void);

static uint32_t iomuxBackup[32 + 20]; /* Backup 32 PTA and 20 PTB IOMUX registers */

static const pmc0_hsrun_mode_config_t s_pmc0HsrunModeConfig = {
    .coreRegulatorVoltLevel = 33U, /* 0.596 + 33 * 0.01083 = 0.95339 */
    .enableForwardBias      = 1U};

static const pmc0_run_mode_config_t s_pmc0RunModeConfig = {.coreRegulatorVoltLevel =
                                                               28U}; /* 0.596 + 28 * 0.01083 = 0.89924 */

static const pmc0_vlpr_mode_config_t s_pmc0VlprModeConfig = {
    .arrayRegulatorSelect   = 0U,
    .coreRegulatorSelect    = 0U,
    .lvdMonitorSelect       = 0U,
    .hvdMonitorSelect       = 0U,
    .enableForceHpBandgap   = 0U,
    .coreRegulatorVoltLevel = 24U, /* 0.596 + 24 * 0.01083 = 0.85592 */
    .enableReverseBackBias  = 1U};

static const pmc0_stop_mode_config_t s_pmc0StopModeConfig = {.coreRegulatorVoltLevel =
                                                                 28U}; /* 0.596 + 28 * 0.01083 = 0.89924 */

static const pmc0_vlps_mode_config_t s_pmc0VlpsModeConfig = {
    .arrayRegulatorSelect   = 0U,
    .coreRegulatorSelect    = 0U,
    .lvdMonitorSelect       = 0U,
    .hvdMonitorSelect       = 0U,
    .enableForceHpBandgap   = 0U,
    .coreRegulatorVoltLevel = 28U, /* 0.596 + 28 * 0.01083 = 0.89924 */
    .enableReverseBackBias  = 1U};

static const pmc0_lls_mode_config_t s_pmc0LlsModeConfig = {
    .arrayRegulatorSelect   = 0U,
    .coreRegulatorSelect    = 0U,
    .lvdMonitorSelect       = 0U,
    .hvdMonitorSelect       = 0U,
    .enableForceHpBandgap   = 0U,
    .coreRegulatorVoltLevel = 13U, /* 0.596 + 13 * 0.01083 = 0.73679 */
    .enableReverseBackBias  = 1U};

static const pmc0_vlls_mode_config_t s_pmc0VllsModeConfig = {
    .arrayRegulatorSelect = 2U, .lvdMonitorSelect = 0U, .hvdMonitorSelect = 0U, .enableForceHpBandgap = 0U};
 static const pmc0_bias_config_t bcfg = {
    .RBBNWellVoltageLevelSelect = 5, .RBBPWellVoltageLevelSelect = 5, .DisableRBBPullDown = 0, .FBBNWellVoltageLevelSelect = 5, .FBBPWellVoltageLevelSelect = 5};

static void APP_InitDebugConsole(void)
{
    CLOCK_SetIpSrc(BOARD_DEBUG_UART_PCC_ADDRESS, APP_DEBUG_UART_DEFAULT_CLKSRC);
    uint32_t uartClkSrcFreq = CLOCK_GetIpFreq(BOARD_DEBUG_UART_PCC_ADDRESS);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, APP_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}
void APP_InitPMC0(void)
{
    /* Initialize PMC0 */
    PMC0_SetBiasConfig(&bcfg);
    PMC0_ConfigureHsrunMode(&s_pmc0HsrunModeConfig);
    PMC0_ConfigureRunMode(&s_pmc0RunModeConfig);
    PMC0_ConfigureVlprMode(&s_pmc0VlprModeConfig);
    PMC0_ConfigureStopMode(&s_pmc0StopModeConfig);
    PMC0_ConfigureVlpsMode(&s_pmc0VlpsModeConfig);
    PMC0_ConfigureLlsMode(&s_pmc0LlsModeConfig);
    PMC0_ConfigureVllsMode(&s_pmc0VllsModeConfig);
}

static void BOARD_InitClock(void)
{
    CLOCK_EnableClock(kCLOCK_PctlA);
    CLOCK_EnableClock(kCLOCK_PctlB);
    CLOCK_EnableClock(kCLOCK_Rgpio2p0);

    BOARD_BootClockRUN();
    CLOCK_SetVlprModeSysClkConfig(&g_sysClkConfigVlpr);
    CLOCK_SetHsrunModeSysClkConfig(&g_sysClkConfigHsrun);

    CLOCK_SetIpSrc(kCLOCK_Lpi2c3, kCLOCK_IpSrcSircAsync);
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcSystem);

    /* Use AUXPLL main clock source */
    CLOCK_SetIpSrcDiv(kCLOCK_Sai0, kCLOCK_IpSrcRtcAuxPllAsync, 0, 0);
    /* SAI clock is needed by Codec initialization. */
    CLOCK_EnableClock(kCLOCK_Sai0);

#ifdef ENABLE_RAM_VECTOR_TABLE
    /* Use RAM vector if needed. Any irq installation can trigger the RAM vector table copy and VTOR update. */
    InstallIRQHandler(LLWU0_IRQn, (uint32_t)LLWU0_IRQHandler);
#endif
}

static void BOARD_InitClockAndPins(void)
{
    BOARD_InitClock();
    BOARD_InitPins();
    APP_SRTM_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
}

void BOARD_InitHardware(void)
{
    /* Power related. */
    SMC_SetPowerModeProtection(MSMC0, kSMC_AllowPowerModeAll);

    BOARD_InitClockAndPins();
    APP_InitPMC0();
    APP_InitDebugConsole();
}

static void APP_Suspend(void)
{
    uint32_t i;

    /* Backup PTA/PTB IOMUXC registers */
    for (i = kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA0; i <= kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTB19; i++)
    {
        iomuxBackup[i] = IOMUXC0->SW_MUX_CTL_PAD[i];
    }

    /* Set PTA0 - PTA2 pads to analog. */
    for (i = kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA0; i <= kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA2; i++)
    {
        IOMUXC0->SW_MUX_CTL_PAD[i] = 0;
    }

    /* PTA3 */
    if ((LLWU->PE1 & LLWU_PE1_WUPE1_MASK) == 0)
    {
        /* VOL+ button is not a wakeup source, can be disabled */
        IOMUXC0->SW_MUX_CTL_PAD[kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA3] = 0;
    }
    else
    {
        /* It's wakeup source, need to set as GPIO */
        IOMUXC0->SW_MUX_CTL_PAD[kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA3] = IOMUXC0_SW_MUX_CTL_PAD_MUX_MODE(1);
    }

    /* Set PTA4 - PTA13 pads to analog. */
    for (i = kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA4; i <= kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA13; i++)
    {
        IOMUXC0->SW_MUX_CTL_PAD[i] = 0;
    }

    if (disableWirelessPinsInVLLS)
    {
        IOMUXC0->SW_MUX_CTL_PAD[kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA14] = 0;
        IOMUXC0->SW_MUX_CTL_PAD[kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA15] = 0;
    }
    else
    {
        /* PTA14 - PTA15 keep unchanged to avoid wireless driver failure in Linux resume. */
    }

    /* Set PTA16 - PTA25 pads to analog. */
    for (i = kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA16; i <= kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA25; i++)
    {
        IOMUXC0->SW_MUX_CTL_PAD[i] = 0;
    }

    if (disableJtagPinsInVLLS)
    {
        for (i = kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA26; i <= kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA30; i++)
        {
            IOMUXC0->SW_MUX_CTL_PAD[i] = 0;
        }
    }
    else
    {
        /* PTA26 - PTA30 keep unchanged as JTAG pads. */
    }

    /* PTA31 */
    if ((LLWU->PE1 & LLWU_PE1_WUPE7_MASK) == 0)
    {
        /* WL_HOST_WAKE is not a wakeup source, can be disabled */
        IOMUXC0->SW_MUX_CTL_PAD[kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA31] = 0;
    }

    /* Set PTB0 - PTB6 pads to analog */
    for (i = kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTB0; i <= kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTB6; i++)
    {
        IOMUXC0->SW_MUX_CTL_PAD[i] = 0;
    }

    /* PTB7 */
    if ((LLWU->PE1 & LLWU_PE1_WUPE11_MASK) == 0)
    {
        /* BT_HOST_WAKE is not a wakeup source, can be disabled */
        IOMUXC0->SW_MUX_CTL_PAD[kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTB7] = 0;
    }

    /* PTB8 keep unchanged as QSPI pad in XIP. Otherwise set it to analog. */
    if (!BOARD_IsRunOnQSPI())
    {
        IOMUXC0->SW_MUX_CTL_PAD[kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTB8] = 0;
    }

    /* Set PTB9 - PTB14 pads to analog */
    for (i = kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTB9; i <= kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTB14; i++)
    {
        IOMUXC0->SW_MUX_CTL_PAD[i] = 0;
    }

    /* PTB15 - PTB19 keep unchanged as QSPI pads in XIP. Otherwise set them to analog. */
    if (!BOARD_IsRunOnQSPI())
    {
        for (i = kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTB15; i <= kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTB19; i++)
        {
            IOMUXC0->SW_MUX_CTL_PAD[i] = 0;
        }
    }

    /* Save SRTM context */
    APP_SRTM_Suspend();
}

static void APP_Resume(bool resume)
{
    uint32_t i;

    /* Restore PTA/PTB IOMUXC registers */
    for (i = kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTA0; i <= kIOMUXC0_IOMUXC0_SW_MUX_CTL_PAD_PTB19; i++)
    {
        IOMUXC0->SW_MUX_CTL_PAD[i] = iomuxBackup[i];
    }

    if (resume)
    {
        /* Only init clock. Pins are recovered at the beginning of resume (LPM_Resume). */
        BOARD_InitClock();
        /* Need to recover LPTMR clock, though LPTMR itself can keep its state in VLLS. */
        CLOCK_EnableClock(kCLOCK_Lptmr1); /* Wakeup timer */
    }

    APP_SRTM_Resume(resume);
}

void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, lpm_power_mode_t targetMode)
{
    if ((LPM_PowerModeRun != targetMode) && (LPM_PowerModeHsrun != targetMode) && (LPM_PowerModeVlpr != targetMode))
    {
        /* Wait for debug console output finished. */
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();
        /*
         * Set pin for current leakage.
         * Debug console RX pin: Set to pinmux to analog.
         * Debug console TX pin: Set to pinmux to analog.
         */
        IOMUXC_SetPinMux(IOMUXC_PTA19_CMP1_IN3_3V, 0);
        IOMUXC_SetPinConfig(IOMUXC_PTA19_CMP1_IN3_3V, 0);
        IOMUXC_SetPinMux(IOMUXC_PTA18_CMP1_IN1_3V, 0);
        IOMUXC_SetPinConfig(IOMUXC_PTA18_CMP1_IN1_3V, 0);

        if (LPM_PowerModeVlls == targetMode)
        {
            APP_Suspend();
        }
    }
}

void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, lpm_power_mode_t targetMode, bool result)
{
    if ((LPM_PowerModeRun != targetMode) && (LPM_PowerModeHsrun != targetMode) && (LPM_PowerModeVlpr != targetMode))
    {
        if (LPM_PowerModeVlls == targetMode)
        {
            APP_Resume(result);
        }
        /*
         * Debug console RX pin was set to disable for current leakage, need to re-configure pinmux.
         * Debug console TX pin was set to disable for current leakage, need to re-configure pinmux.
         */
        IOMUXC_SetPinMux(IOMUXC_PTA18_LPUART0_TX, 0U);
        IOMUXC_SetPinConfig(IOMUXC_PTA18_LPUART0_TX, IOMUXC0_SW_MUX_CTL_PAD_PE_MASK | IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
        IOMUXC_SetPinMux(IOMUXC_PTA19_LPUART0_RX, 0U);
        IOMUXC_SetPinConfig(IOMUXC_PTA19_LPUART0_RX, IOMUXC0_SW_MUX_CTL_PAD_PE_MASK | IOMUXC0_SW_MUX_CTL_PAD_PS_MASK);
        APP_InitDebugConsole();
    }
    PRINTF("== Power switch %s ==\r\n", result ? "OK" : "FAIL");
}
/*${function:end}*/
