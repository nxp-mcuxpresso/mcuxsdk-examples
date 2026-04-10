/*
 * Copyright 2019, 2021-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <setjmp.h>

#include "fsl_cmc.h"
#include "fsl_spc.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "power_mode_switch.h"
#include "peripherals.h"
#include "board.h"
#include "fsl_lpuart.h"
#if !(defined(DEMO_NOT_SUPPORT_WAKEUP_BOOT) && DEMO_NOT_SUPPORT_WAKEUP_BOOT)
#include "fsl_crc.h"
#endif
#if (defined(FSL_FEATURE_SOC_VBAT_COUNT) && (FSL_FEATURE_SOC_VBAT_COUNT > 0))
#include "fsl_vbat.h"
#endif
#include "fsl_ccm32k.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef APP_CLEAR_POWER_DOMAIN3_REQUEST
#define APP_CLEAR_POWER_DOMAIN3_REQUEST 0
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_SetSPCConfiguration(void);
static void APP_SetCMCConfiguration(void);
static void APP_DeInitVbat(void);
static void APP_WaitForSPCReady(void);

static uint8_t APP_GetWakeupTimeout(void);
static app_wakeup_source_t APP_SelectWakeupSource(app_power_mode_t targetMode);
static void APP_GetWakeupConfig(app_power_mode_t targetMode);

static void APP_PowerPreSwitchHook(void);
static void APP_PowerPostSwitchHook(void);

static void APP_ShowPowerMode(cmc_low_power_mode_t powerMode);
static void APP_EnterSleep1Mode(void);
static void APP_EnterDeepSleep1Mode(void);
static void APP_EnterDeepSleep2Mode(void);
static void APP_EnterDeepSleep3Mode(void);
static void APP_EnterDeepSleep4Mode(void);
static void APP_EnterPowerDown1Mode(void);
static void APP_EnterPowerDown2Mode(void);
static void APP_EnterPowerDown3Mode(void);
static void APP_EnterPowerDown4Mode(void);
static void APP_EnterDeepPowerDown1Mode(void);
static void APP_EnterDeepPowerDown2Mode(void);
static void APP_PowerModeSwitch(app_power_mode_t targetPowerMode);
static app_power_mode_t APP_GetTargetPowerMode(void);
static uint8_t APP_GetFilteredInputChar(void);
static void APP_FlushDebugConsoleRx(void);

#if !(defined(DEMO_NOT_SUPPORT_WAKEUP_BOOT) && DEMO_NOT_SUPPORT_WAKEUP_BOOT)
static void APP_SaveRuntimeContext(void);
static void APP_RestoreRuntimeContext(void);
static void APP_SetPowerDownModeWakeupConfig(void);
static void APP_WakeupFunction(void);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint8_t s_wakeupTimeout; /* Wakeup timeout. (Unit: Second) */
static volatile app_wakeup_source_t g_selectedWakeupSource;

AT_ALWAYS_ON_DATA(app_power_mode_t g_targetPowerMode);
AT_ALWAYS_ON_DATA_INIT(char *g_modeNameArray[]) = APP_MODE_NAME_ARRAY;
AT_ALWAYS_ON_DATA_INIT(char *g_modeDescArray[]) = APP_MODE_DESC_ARRAY;

#if !(defined(DEMO_NOT_SUPPORT_WAKEUP_BOOT) && DEMO_NOT_SUPPORT_WAKEUP_BOOT)
AT_ALWAYS_ON_DATA(uint32_t g_scbVtor);
AT_ALWAYS_ON_DATA(uint32_t g_scbIcsr);
AT_ALWAYS_ON_DATA(uint32_t g_scbAircr);
AT_ALWAYS_ON_DATA(uint32_t g_scbCCR);
AT_ALWAYS_ON_DATA(uint8_t g_scbShp[12U]);
AT_ALWAYS_ON_DATA(uint32_t g_scbShcsr);
AT_ALWAYS_ON_DATA(uint32_t g_nvicIser[16U]);
AT_ALWAYS_ON_DATA(uint8_t g_nvicIp[496U]);
AT_ALWAYS_ON_DATA(uint32_t g_cpuControl);
AT_ALWAYS_ON_DATA(uint32_t g_wakeupEntry[12U]);
AT_ALWAYS_ON_DATA(jmp_buf g_coreContext);

extern uint32_t m_warmboot_stack_end;
#endif

#ifndef DEMO_ASSERT_SW_RESET
#define DEMO_ASSERT_SW_RESET    NVIC_SystemReset()
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    uint32_t freq;
    cmc_low_power_mode_t curmode;
    bool needSetWakeup = false;

    BOARD_InitHardware();

    APP_SetSPCConfiguration();

    PRINTF("\r\n----------------------- Normal Boot -----------------------\r\n");

    while (1)
    {
        PRINTF("\r\nSticky Reset Reason 0x%x.\r\n", CMC_GetStickySystemResetStatus(APP_CMC));
        CMC_ClearStickySystemResetStatus(CMC0, CMC_GetStickySystemResetStatus(APP_CMC));
        APP_SetCMCConfiguration();
        APP_DeInitVbat();

        freq    = CLOCK_GetFreq(kCLOCK_CoreSysClk);
        curmode = CMC_GetMAINPowerMode(APP_CMC);
        PRINTF("\r\n###########################    Power Mode Switch Demo    ###########################\r\n");
        PRINTF("    Core Clock = %dHz \r\n", freq);

        APP_ShowPowerMode(curmode);
        g_targetPowerMode = APP_GetTargetPowerMode();

        /* If target mode is Active mode, don't need to set wakeup source. */
        if (g_targetPowerMode == kAPP_PowerModeActive)
        {
            needSetWakeup = false;
        }
        else
        {
            needSetWakeup = true;
        }
        PRINTF("%s\r\n", g_modeDescArray[(uint8_t)(g_targetPowerMode - kAPP_PowerModeActive)]);
        if (needSetWakeup)
        {
            APP_GetWakeupConfig(g_targetPowerMode);
            APP_PowerPreSwitchHook();
            APP_PowerModeSwitch(g_targetPowerMode);
            APP_PowerPostSwitchHook();
            PRINTF("\r\n************* Wake Up From %s Mode *************\r\n",
                   g_modeNameArray[(uint8_t)(g_targetPowerMode - kAPP_PowerModeActive)]);
        }

        PRINTF("\r\n-----------------------Next loop -----------------------\r\n");
    }
}

/*!
 * @brief In active mode, all HVDs/LVDs are disabled, DCDC regulated to 1.5V, Core LDO regulated to 1.1V;
 * In low power modes, all HVDs/LVDs are disabled, bandgap is disabled, DCDC regulated to 1.25V, Core LDO regulated to
 * 1.0V.
 *
 * @note Platform-conditional paths:
 *   - VDD_SYS H/LVD disable is compiled only when FSL_FEATURE_SPC_HAS_VDD_SYS=1
 *     (KW45 / KW47 / MCXW72 / MCXW727).
 *   - VDD1P8 LVD disable is compiled only when FSL_FEATURE_SPC_HAS_VDD1P8_LVD=1
 *     (KW43).
 *   - System LDO and LP bandgap-buffer (lpBuff) fields are only assigned and
 *     programmed when FSL_FEATURE_SPC_HAS_SYS_LDO / FSL_FEATURE_SPC_HAS_LPBUFF
 *     are set; KW43 does not have either.
 *   - The staged LP Core LDO transition is compiled only when
 *     DEMO_NEED_STAGED_LP_CORELDO_WRITE is defined to 1 in the board's
 *     app.h, for boards whose LP_CFG reset default is not the demo's target
 *     (Mid voltage). It satisfies the SPC driver's interlock that LP Core
 *     LDO voltage can only change while drive strength is Normal; see the
 *     inline comment at the staged block for details.
 *   - When DEMO_KEEP_LP_IREF_ENABLED is defined to 1 in the board's app.h,
 *     lowPowerRegulatorOption.lpIREF is kept true so LP_CFG[LP_IREFEN]
 *     stays set in DPDOWN. Required on platforms where LP IREF is a
 *     shared PMC reference for an always-on rail (e.g. KW43 VDD_SYS_LV).
 * The DCDC and LP regulator writes use a short retry loop so a transient
 * kStatus_SPC_Busy does not fail the configuration; this path is universal
 * and does not change behavior on platforms that succeed on the first try.
 */
static void APP_SetSPCConfiguration(void)
{
    status_t status;
    uint32_t retry;
    spc_active_mode_regulators_config_t activeModeRegulatorOption;
    spc_lowpower_mode_regulators_config_t lowPowerRegulatorOption;

    (void)memset(&activeModeRegulatorOption, 0, sizeof(activeModeRegulatorOption));
    (void)memset(&lowPowerRegulatorOption, 0, sizeof(lowPowerRegulatorOption));

    APP_WaitForSPCReady();

    /* Disable LVDs and HVDs in Active mode. */
    SPC_EnableActiveModeCoreHighVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeCoreLowVoltageDetect(APP_SPC, false);
#if defined(FSL_FEATURE_SPC_HAS_VDD_SYS) && FSL_FEATURE_SPC_HAS_VDD_SYS
    SPC_EnableActiveModeSystemHighVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeSystemLowVoltageDetect(APP_SPC, false);
#endif
#if defined(FSL_FEATURE_SPC_HAS_VDD1P8_LVD) && FSL_FEATURE_SPC_HAS_VDD1P8_LVD
    SPC_EnableActiveModeVDD1P8LowVoltageDetect(APP_SPC, false);
#endif
    SPC_EnableActiveModeIOHighVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeIOLowVoltageDetect(APP_SPC, false);

    APP_WaitForSPCReady();

    activeModeRegulatorOption.bandgapMode                      = kSPC_BandgapEnabledBufferDisabled;
    activeModeRegulatorOption.DCDCOption.DCDCVoltage           = kSPC_DCDC_NormalVoltage; /* DCDC regulate to 1.5V. */
    activeModeRegulatorOption.DCDCOption.DCDCDriveStrength     = kSPC_DCDC_NormalDriveStrength;
#if defined(FSL_FEATURE_SPC_HAS_LPBUFF) && FSL_FEATURE_SPC_HAS_LPBUFF
    activeModeRegulatorOption.lpBuff                           = false;
#endif
#if defined(FSL_FEATURE_SPC_HAS_SYS_LDO) && FSL_FEATURE_SPC_HAS_SYS_LDO
    activeModeRegulatorOption.SysLDOOption.SysLDOVoltage       = kSPC_SysLDO_NormalVoltage;
    activeModeRegulatorOption.SysLDOOption.SysLDODriveStrength = kSPC_SysLDO_NormalDriveStrength;
#endif
    activeModeRegulatorOption.CoreLDOOption.CoreLDOVoltage     =
        kSPC_CoreLDO_NormalVoltage; /* Core LDO regulate to 1.1V. */
#if defined(FSL_FEATURE_SPC_HAS_CORELDO_VDD_DS) && FSL_FEATURE_SPC_HAS_CORELDO_VDD_DS
    activeModeRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength;
#endif /* FSL_FEATURE_SPC_HAS_CORELDO_VDD_DS */
    status = kStatus_SPC_Busy;
    for (retry = 0U; (retry < 8U) && (status == kStatus_SPC_Busy); retry++)
    {
        APP_WaitForSPCReady();
        status = SPC_SetActiveModeDCDCRegulatorConfig(APP_SPC, &activeModeRegulatorOption.DCDCOption);
    }
    if (status != kStatus_Success)
    {
        PRINTF("Fail to set DCDC in Active Mode!\r\n");
    }
    APP_WaitForSPCReady();
#if defined(FSL_FEATURE_SPC_HAS_SYS_LDO) && FSL_FEATURE_SPC_HAS_SYS_LDO
    if (SPC_SetActiveModeSystemLDORegulatorConfig(APP_SPC, &activeModeRegulatorOption.SysLDOOption) != kStatus_Success)
    {
        PRINTF("Fail to set System LDO in Active Mode!\r\n ");
    }
#endif
    if (SPC_SetActiveModeBandgapModeConfig(APP_SPC, activeModeRegulatorOption.bandgapMode) != kStatus_Success)
    {
        PRINTF("Fail to set bandgap mode in Active Mode!\r\n");
    }
    status = SPC_SetActiveModeCoreLDORegulatorConfig(APP_SPC, &activeModeRegulatorOption.CoreLDOOption);
    if (status != kStatus_Success)
    {
        PRINTF("Fail to set Core LDO in Active mode!\r\n");
    }
#if defined(FSL_FEATURE_SPC_HAS_LPBUFF) && FSL_FEATURE_SPC_HAS_LPBUFF
    SPC_EnableActiveModeCMPBandgapBuffer(APP_SPC, activeModeRegulatorOption.lpBuff);
#endif

    APP_WaitForSPCReady();

#if defined(DEMO_KEEP_LP_IREF_ENABLED) && DEMO_KEEP_LP_IREF_ENABLED
    /*
     * LP IREF is a shared PMC current reference on some platforms -- not
     * only used by OSC32K but also as the bias for an always-on domain
     * (e.g. VDD_SYS_LV on KW43). Disabling LP_IREF in DPDOWN would corrupt
     * that AON rail, so the board keeps it enabled by setting this knob.
     * LP_CFG[LP_IREFEN] is hardware-forced to 1 in every non-DPDOWN LP
     * mode, so this field only matters for DPDOWN entry.
     */
    lowPowerRegulatorOption.lpIREF                             = true;
#else
    lowPowerRegulatorOption.lpIREF                             = false;
#endif
    lowPowerRegulatorOption.bandgapMode                        = kSPC_BandgapDisabled;
    lowPowerRegulatorOption.CoreIVS                            = false;
    lowPowerRegulatorOption.DCDCOption.DCDCVoltage             = kSPC_DCDC_LowUnderVoltage;
    lowPowerRegulatorOption.DCDCOption.DCDCDriveStrength       = kSPC_DCDC_LowDriveStrength;
#if defined(FSL_FEATURE_SPC_HAS_LPBUFF) && FSL_FEATURE_SPC_HAS_LPBUFF
    lowPowerRegulatorOption.lpBuff                             = false;
#endif
#if defined(FSL_FEATURE_SPC_HAS_SYS_LDO) && FSL_FEATURE_SPC_HAS_SYS_LDO
    lowPowerRegulatorOption.SysLDOOption.SysLDODriveStrength   = kSPC_SysLDO_LowDriveStrength;
#endif
    lowPowerRegulatorOption.CoreLDOOption.CoreLDOVoltage       = kSPC_CoreLDO_MidDriveVoltage;
    lowPowerRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_LowDriveStrength;

#if defined(DEMO_NEED_STAGED_LP_CORELDO_WRITE) && DEMO_NEED_STAGED_LP_CORELDO_WRITE
    /*
     * Opt-in workaround for boards where a single-shot
     * SPC_SetLowPowerModeRegulatorsConfig cannot reach the demo's target
     * LP Core LDO state (voltage = Mid, drive strength = Low) from the
     * LP_CFG reset default, because the driver's interlock rejects
     * "change voltage while drive strength is Low" with
     * kStatus_SPC_CORELDOVoltageSetFail
     *
     * Stage the transition in two steps: first enable the LP bandgap and
     * set Core LDO to the target voltage with Normal drive strength
     * (allowed path), then let the full LP regulator config below take the
     * Low-drive path where the voltage check passes because preVoltage now
     * already matches. The staged call's bandgap setting is overwritten by
     * the final LP regulator config a few lines later, so this does not
     * change the final LP bandgap state.
     *
     * A board defines DEMO_NEED_STAGED_LP_CORELDO_WRITE to 1 in its app.h
     * when its LP_CFG[CORELDO_VDD_LVL] reset default is not the demo's
     * target (Mid). Currently only frdmkw43 sets this knob (KW43 boots
     * with CORELDO_VDD_LVL = 10b / Normal 1.1 V per KW43 RM LP_CFG reset
     * row); KW45 / KW47 / MCXW72 / MCXW727 boot with the reset value
     * already at Mid so the single-shot path succeeds for them.
     */
    spc_lowpower_mode_core_ldo_option_t stagedLowPowerCoreLDOOption = lowPowerRegulatorOption.CoreLDOOption;
    stagedLowPowerCoreLDOOption.CoreLDODriveStrength                = kSPC_CoreLDO_NormalDriveStrength;

    APP_WaitForSPCReady();
    status = SPC_SetLowPowerModeBandgapmodeConfig(APP_SPC, kSPC_BandgapEnabledBufferDisabled);
    if (status != kStatus_Success)
    {
        PRINTF("Fail to stage bandgap for Low Power Mode, status = 0x%x.\r\n", status);
        return;
    }

    APP_WaitForSPCReady();
    status = SPC_SetLowPowerModeCoreLDORegulatorConfig(APP_SPC, &stagedLowPowerCoreLDOOption);
    if (status != kStatus_Success)
    {
        PRINTF("Fail to stage Core LDO for Low Power Mode, status = 0x%x.\r\n", status);
        return;
    }
#endif /* DEMO_NEED_STAGED_LP_CORELDO_WRITE */

    status = kStatus_SPC_Busy;
    for (retry = 0U; (retry < 8U) && (status == kStatus_SPC_Busy); retry++)
    {
        APP_WaitForSPCReady();
        status = SPC_SetLowPowerModeRegulatorsConfig(APP_SPC, &lowPowerRegulatorOption);
    }

    if (status != kStatus_Success)
    {
        PRINTF("Fail to set regulators in Low Power Mode, status = 0x%x.\r\n", status);
        return;
    }

    APP_WaitForSPCReady();

    SPC_SetLowPowerWakeUpDelay(APP_SPC, 0xFFFFU);
}

static void APP_WaitForSPCReady(void)
{
    while (SPC_GetBusyStatusFlag(APP_SPC))
    {
    }
}

/*!
 * @brief 1. Set power mode protection; 2. Disable low power mode debug; 3. Enable Flash Doze mode.
 */
static void APP_SetCMCConfiguration(void)
{
    CMC_SetPowerModeProtection(APP_CMC, kCMC_AllowAllLowPowerModes);
    CMC_LockPowerModeProtectionSetting(APP_CMC);
    CMC_EnableDebugOperation(APP_CMC, false);
    CMC_ConfigFlashMode(APP_CMC, false, true, false);
}

/*!
 * @brief Disable Backup SRAM regulator, FRO16K and Bandgap which locates in VBAT power domain for most of power modes.
 *
 */
static void APP_DeInitVbat(void)
{
#if (defined(FSL_FEATURE_SOC_VBAT_COUNT) && (FSL_FEATURE_SOC_VBAT_COUNT > 0))
    VBAT_EnableBackupSRAMRegulator(APP_VBAT, false);
    VBAT_EnableFRO16k(APP_VBAT, false);
    while (VBAT_CheckFRO16kEnabled(APP_VBAT))
        ;
    VBAT_EnableBandgap(APP_VBAT, false);
    while (VBAT_CheckBandgapEnabled(APP_VBAT))
        ;
#endif
}

static app_wakeup_source_t APP_SelectWakeupSource(app_power_mode_t targetMode)
{
    app_wakeup_source_t wakeupSource;
    uint8_t ch;

    while (1)
    {
        PRINTF("\r\nSelect the desired wakeup source:\r\n");
#if !(defined(DEMO_PD_MODE_NOT_OK) && DEMO_PD_MODE_NOT_OK)
#if DEMO_HAS_POWER_SWITCH_OFF
        if (targetMode != kAPP_PowerSwitchOff)
        {
            PRINTF("Press %c to select LPTMR0\r\n", kAPP_WakeupSourceLptmr);
            PRINTF("Press %c to select button\r\n", kAPP_WakeupSourceWakeupButton);
        }
#else
        PRINTF("Press %c to select LPTMR0\r\n", kAPP_WakeupSourceLptmr);
        PRINTF("Press %c to select button\r\n", kAPP_WakeupSourceWakeupButton);
#endif
#endif

#if (defined(FSL_FEATURE_SOC_VBAT_COUNT) && (FSL_FEATURE_SOC_VBAT_COUNT > 0))
        PRINTF("Press %c to Select VBAT\r\n", kAPP_WakeupSourceVbat);
#endif

        PRINTF("\r\nWaiting for wakeup source select...\r\n");

        ch = APP_GetFilteredInputChar();
        PUTCHAR(ch);
        PRINTF("\n");
        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        wakeupSource = (app_wakeup_source_t)ch;
#if !(defined(DEMO_PD_MODE_NOT_OK) && DEMO_PD_MODE_NOT_OK)
#if DEMO_HAS_POWER_SWITCH_OFF
        if (targetMode != kAPP_PowerSwitchOff)
#endif
        {
            if ((wakeupSource == kAPP_WakeupSourceLptmr) || (wakeupSource == kAPP_WakeupSourceWakeupButton))
            {
                return wakeupSource;
            }
        }
#endif
#if (defined(FSL_FEATURE_SOC_VBAT_COUNT) && (FSL_FEATURE_SOC_VBAT_COUNT > 0))
        if (wakeupSource == kAPP_WakeupSourceVbat)
        {
            return wakeupSource;
        }
#endif

        PRINTF("Wrong input, please re-try!\r\n");
    }
}

/* Get wakeup timeout and wakeup source. */
static void APP_GetWakeupConfig(app_power_mode_t targetMode)
{
    g_selectedWakeupSource = APP_SelectWakeupSource(targetMode);

    switch (g_selectedWakeupSource)
    {
        case kAPP_WakeupSourceLptmr:
        {
            /* Wakeup source is LPTMR, user should input wakeup timeout value. */
            s_wakeupTimeout = APP_GetWakeupTimeout();
            PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
            /* Isolate some external power domain to save power. */
            SPC_SetExternalVoltageDomainsConfig(APP_SPC, APP_LPTMR_LOW_POWER_ISO_DOMAINS, 0x0U);
            WUU_SetInternalWakeUpModulesConfig(APP_WUU, WUU_WAKEUP_LPTMR_IDX, kWUU_InternalModuleInterrupt);
            LPTMR_SetTimerPeriod(APP_LPTMR, (32000U * s_wakeupTimeout) - 1U);
            LPTMR_EnableInterrupts(APP_LPTMR, kLPTMR_TimerInterruptEnable);
            LPTMR_StartTimer(APP_LPTMR);
            EnableIRQ(LPTMR0_IRQn);
            break;
        }

        case kAPP_WakeupSourceWakeupButton:
        {
            PRINTF(
                "Use wakeup button will increase current consumption due to VDD_IO_ABC power domain can not be "
                "isolated.\r\n");
            PRINTF("Please press %s to wakeup device.\r\n", WUU_WAKEUP_BUTTON_NAME);
            /* Isolate some external power domain to save power. */
            SPC_SetExternalVoltageDomainsConfig(APP_SPC, APP_WAKEUP_BUTTON_LOW_POWER_ISO_DOMAINS, 0x0U);
            wuu_external_wakeup_pin_config_t wakeupButtonConfig;
            wakeupButtonConfig.edge  = kWUU_ExternalPinRisingEdge;
            wakeupButtonConfig.event = kWUU_ExternalPinInterrupt;
            wakeupButtonConfig.mode  = kWUU_ExternalPinActiveAlways;
            WUU_SetExternalWakeUpPinsConfig(APP_WUU, WUU_WAKEUP_BUTTON_IDX, &wakeupButtonConfig);
            break;
        }

#if (defined(FSL_FEATURE_SOC_VBAT_COUNT) && (FSL_FEATURE_SOC_VBAT_COUNT > 0))
        case kAPP_WakeupSourceVbat:
        {
#if defined(SWITCH_WAKEUP_BUTTON_NAME)
            PRINTF("Select wakeup from VBAT domain(Aka Smart power switch domain)!\r\n");
            PRINTF("Please press %s to wakeup device", SWITCH_WAKEUP_BUTTON_NAME);
#else
            PRINTF("Waiting for WAKEUP pin's falling edge input.\r\n");
#endif
            /* Isolate some external power domain to save power. */
            SPC_SetExternalVoltageDomainsConfig(APP_SPC, APP_VBAT_LOW_POWER_ISO_DOMAINS, 0x0U);
            /* Clear all status flags. */
            VBAT_ClearStatusFlags(APP_VBAT, kVBAT_StatusFlagPORDetect | kVBAT_StatusFlagWakeupPin |
                                                kVBAT_StatusFlagBandgapTimer0 | kVBAT_StatusFlagBandgapTimer1);
            VBAT_DisableInterrupts(APP_VBAT, kVBAT_AllInterruptsEnable);
            VBAT_DisableWakeup(APP_VBAT, kVBAT_AllWakeupsEnable);
            WUU_SetInternalWakeUpModulesConfig(APP_WUU, WUU_WAKEUP_VBAT_IDX, kWUU_InternalModuleInterrupt);
            EnableIRQ(VBAT_IRQn);
            VBAT_EnableInterrupts(APP_VBAT, kVBAT_InterruptEnableWakeupPin);
            /* To wakeup from deep sleep or more higher mode, it is needed  to enable wakeup feature. */
            VBAT_EnableWakeup(APP_VBAT, kVBAT_WakeupEnableWakeupPin);
            break;
        }
#endif
        default:
        {
            assert(false);
            break;
        }
    }

#if defined(DEMO_NOT_SUPPORT_WAKEUP_BOOT) && DEMO_NOT_SUPPORT_WAKEUP_BOOT
    /*
     * On boards without wakeup-boot support, Power Down and Deep
     * Power Down wake up through the normal boot path (the banner
     * is reprinted) rather than resuming in place. Announce this
     * explicitly so the test harness can tell "normal boot after
     * wake" apart from the regular "Next loop" path.
     */
    if (targetMode >= kAPP_PowerModePowerDown1)
    {
        PRINTF("Note: Wakeup cause a reset!\r\n");
    }
#endif
}

/*!
 * Get input from user about wakeup timeout
 */
static uint8_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");
        PRINTF("The allowed range is 1s ~ 9s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        timeout = APP_GetFilteredInputChar();
        PRINTF("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        PRINTF("Wrong value!\r\n");
    }
}

static void APP_PowerPreSwitchHook(void)
{
    /* Wait for debug console output finished. */
    while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
    {
    }
    APP_DeinitDebugConsole();

    if (g_selectedWakeupSource != kAPP_WakeupSourceWakeupButton)
    {
        APP_DeinitWakeupButton();
    }
}

static void APP_PowerPostSwitchHook(void)
{
    if (SPC_CheckPowerDomainLowPowerRequest(APP_SPC, kSPC_PowerDomain0))
    {
        SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain0);
    }
    if (SPC_CheckPowerDomainLowPowerRequest(APP_SPC, kSPC_PowerDomain1))
    {
        SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain1);
    }
    if (SPC_CheckPowerDomainLowPowerRequest(APP_SPC, kSPC_PowerDomain2))
    {
        RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK));
        RFMC->RF2P4GHZ_CTRL &= ~RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;
        SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain2);
    }
#if APP_CLEAR_POWER_DOMAIN3_REQUEST
    if (SPC_CheckPowerDomainLowPowerRequest(APP_SPC, kSPC_PowerDomain3))
    {
        SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain3);
    }
#endif
    SPC_ClearLowPowerRequest(APP_SPC);

    CLOCK_EnableClock(kCLOCK_Lpuart0);
    APP_InitDebugConsole();
}

static void APP_ShowPowerMode(cmc_low_power_mode_t powermode)
{
    if (powermode == kCMC_ActiveMode)
    {
        PRINTF("    Power mode: Active\r\n");
    }
}

static app_power_mode_t APP_GetTargetPowerMode(void)
{
    uint8_t ch;
    app_power_mode_t inputPowerMode;

    while (1)
    {
        PRINTF("\r\nPlease Select the desired power mode: \n\r\n");

        for (app_power_mode_t powerModeIndex = kAPP_PowerModeActive; powerModeIndex < kAPP_PowerModeMax;
             powerModeIndex++)
        {
            PRINTF("\tPress %c to enter %s mode!\r\n", powerModeIndex,
                   g_modeNameArray[(uint8_t)(powerModeIndex - kAPP_PowerModeActive)]);
        }
        PRINTF("\r\n\tWaiting for power mode select...\r\n\r\n");

        APP_FlushDebugConsoleRx();
        ch = APP_GetFilteredInputChar();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        PUTCHAR(ch);
        PUTCHAR('\n');

        inputPowerMode = (app_power_mode_t)ch;
        if ((inputPowerMode > kAPP_PowerModeMin) && (inputPowerMode < kAPP_PowerModeMax))
        {
            return inputPowerMode;
        }
        PRINTF("Wrong input, please re-try!\r\n");
    }
}

static uint8_t APP_GetFilteredInputChar(void)
{
    int chInput;
    uint8_t ch;

    while (1)
    {
        chInput = GETCHAR();
        if ((chInput < 0) || (chInput > 0xFF))
        {
            continue;
        }

        ch = (uint8_t)chInput;
        if ((ch == '\r') || (ch == '\n') || (ch == '\0'))
        {
            continue;
        }
        if ((ch < 0x20U) || (ch > 0x7EU))
        {
            APP_FlushDebugConsoleRx();
            continue;
        }

        return ch;
    }
}

static void APP_FlushDebugConsoleRx(void)
{
    while ((kLPUART_RxDataRegFullFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)) != 0U)
    {
        (void)LPUART_ReadByte((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR);
    }
}

static void APP_PowerModeSwitch(app_power_mode_t targetPowerMode)
{
    if (targetPowerMode != kAPP_PowerModeActive)
    {
        switch (targetPowerMode)
        {
            case kAPP_PowerModeSleep1:
                APP_EnterSleep1Mode();
                break;
            case kAPP_PowerModeDeepSleep1:
                APP_EnterDeepSleep1Mode();
                break;
            case kAPP_PowerModeDeepSleep2:
                APP_EnterDeepSleep2Mode();
                break;
            case kAPP_PowerModeDeepSleep3:
                APP_EnterDeepSleep3Mode();
                break;
            case kAPP_PowerModeDeepSleep4:
                APP_EnterDeepSleep4Mode();
                break;
#if !(defined(DEMO_PD_MODE_NOT_OK) && DEMO_PD_MODE_NOT_OK)
            case kAPP_PowerModePowerDown1:
                APP_EnterPowerDown1Mode();
                break;
            case kAPP_PowerModePowerDown2:
                APP_EnterPowerDown2Mode();
                break;
            case kAPP_PowerModePowerDown3:
                APP_EnterPowerDown3Mode();
                break;
            case kAPP_PowerModePowerDown4:
                APP_EnterPowerDown4Mode();
                break;
            case kAPP_PowerModeDeepPowerDown1:
                APP_EnterDeepPowerDown1Mode();
                break;
            case kAPP_PowerModeDeepPowerDown2:
                APP_EnterDeepPowerDown2Mode();
                break;
#if DEMO_HAS_POWER_SWITCH_OFF
            case kAPP_PowerSwitchOff:
                SPC_PowerModeControlPowerSwitch(APP_SPC);
                APP_EnterDeepPowerDown1Mode();
                break;
#endif
#endif
            default:
                assert(false);
                break;
        }
    }
}

static void APP_EnterSleep1Mode(void)
{
    cmc_power_domain_config_t config;

    /* Set NBU into Sleep Mode */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0x1);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    /* Set MAIN_CORE and MAIN_WAKE power domain into sleep mode. */
    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_SleepMode;
    config.wake_domain = kCMC_SleepMode;
    CMC_EnterLowPowerMode(APP_CMC, &config);
}

static void APP_EnterDeepSleep1Mode(void)
{
    cmc_power_domain_config_t config;

    /* Enable CORE VDD Voltage scaling. */
    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);

    /* Set NBU into Deep Sleep Mode */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0x3);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    /* Set MAIN_CORE and MAIN_WAKE power domain into Deep Sleep Mode. */
    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_DeepSleepMode;
    config.wake_domain = kCMC_DeepSleepMode;

    CMC_EnterLowPowerMode(APP_CMC, &config);
}

static void APP_EnterDeepSleep2Mode(void)
{
    cmc_power_domain_config_t config;

    /* Enable CORE VDD Voltage scaling. */
    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);

    /* Power off some CM33 RAMs in deep sleep mode.  */
    CMC_PowerOffSRAMLowPowerOnly(APP_CMC, APP_DEEPSLEEP2_SRAM_POWER_MODE);

    /* Set NBU into Deep Sleep Mode. */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0x3);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    /* Set MAIN_CORE and MAIN_WAKE power domain into deep sleep mode. */
    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_DeepSleepMode;
    config.wake_domain = kCMC_DeepSleepMode;
    CMC_EnterLowPowerMode(APP_CMC, &config);
    /* Due to some CM33 RAMs are powered off in this mode, after waking up a hardfault will be asserted, do software
     * reset to re-start this demo. */
    DEMO_ASSERT_SW_RESET;
}

static void APP_EnterDeepSleep3Mode(void)
{
    cmc_power_domain_config_t config;

    /* Set MAIN_CORE and MAIN_WAKE power domain into deep sleep mode. */
    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_DeepSleepMode;
    config.wake_domain = kCMC_DeepSleepMode;

    /* Set NBU into Deep Sleep Mode. */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0x3);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    /* Enable CORE VDD Voltage scaling. */
    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);
    CMC_PowerOffSRAMLowPowerOnly(APP_CMC, APP_DEEPSLEEP3_SRAM_POWER_MODE);

    CMC_EnterLowPowerMode(APP_CMC, &config);
    /* Due to some CM33 RAMs are powered off in this mode, after waking up a hardfault will be asserted, do software
     * reset to re-start this demo. */
    DEMO_ASSERT_SW_RESET;
}

static void APP_EnterDeepSleep4Mode(void)
{
    cmc_power_domain_config_t config;

    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_DeepSleepMode;
    config.wake_domain = kCMC_DeepSleepMode;

    /* Set NBU into Deep Sleep Mode. */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0x3);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);
    CMC_PowerOffSRAMLowPowerOnly(APP_CMC, APP_DEEPSLEEP4_SRAM_POWER_MODE);

    CMC_EnterLowPowerMode(APP_CMC, &config);
    /* Due to some CM33 RAMs are powered off in this mode, after waking up a busfault will be asserted, do software
     * reset to re-start this demo. */
    DEMO_ASSERT_SW_RESET;
}

#if !(defined(DEMO_PD_MODE_NOT_OK) && DEMO_PD_MODE_NOT_OK)
static void APP_EnterPowerDown1Mode(void)
{
    cmc_power_domain_config_t config;

    /* Set NBU into Power Down mode. */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0x7);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_PowerDownMode;
    config.wake_domain = kCMC_DeepSleepMode;

    CMC_PowerOffSRAMLowPowerOnly(APP_CMC, APP_POWERDOWN1_SRAM_POWER_MODE);
    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);

#if !(defined(DEMO_NOT_SUPPORT_WAKEUP_BOOT) && DEMO_NOT_SUPPORT_WAKEUP_BOOT)
    APP_SetPowerDownModeWakeupConfig();

    APP_SaveRuntimeContext();

    CLOCK_DisableClock(kCLOCK_Secsubsys);
    memset(&g_coreContext, 0, sizeof(g_coreContext));
    if (setjmp(g_coreContext) == 0)
    {
        CMC_EnterLowPowerMode(APP_CMC, &config);
    }
    /* Wake up from power down1 mode. */
    APP_RestoreRuntimeContext();
    __enable_irq();
#else
    CMC_EnterLowPowerMode(APP_CMC, &config);
#endif
}

static void APP_EnterPowerDown2Mode(void)
{
    cmc_power_domain_config_t config;

    /* Set NBU into Power Down Mode. */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0x7);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    /* Set CORE_MAIN, CORE_WAKE power domain into power down mode. */
    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_PowerDownMode;
    config.wake_domain = kCMC_PowerDownMode;

    CMC_PowerOffSRAMLowPowerOnly(APP_CMC, APP_POWERDOWN2_SRAM_POWER_MODE);
    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);
#if !(defined(DEMO_NOT_SUPPORT_WAKEUP_BOOT) && DEMO_NOT_SUPPORT_WAKEUP_BOOT)
    /* Set wakeup function. */
    APP_SetPowerDownModeWakeupConfig();

    APP_SaveRuntimeContext();
    memset(&g_coreContext, 0, sizeof(g_coreContext));
    if (setjmp(g_coreContext) == 0)
    {
        CMC_EnterLowPowerMode(APP_CMC, &config);
    }
    /* Wake up from power down2 mode. */
    APP_RestoreRuntimeContext();
    __enable_irq();
#else
    CMC_EnterLowPowerMode(APP_CMC, &config);
#endif
}

static void APP_EnterPowerDown3Mode(void)
{
    cmc_power_domain_config_t config;

    /* Power off all radio RAMs. */
    RF_CMC1->RAM_PWR = 0x000004FF;
    /* Set NBU into Power Down Mode. */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0x7);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_PowerDownMode;
    config.wake_domain = kCMC_PowerDownMode;

    CMC_PowerOffSRAMLowPowerOnly(APP_CMC, APP_POWERDOWN3_SRAM_POWER_MODE);
    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);
#if !(defined(DEMO_NOT_SUPPORT_WAKEUP_BOOT) && DEMO_NOT_SUPPORT_WAKEUP_BOOT)
    APP_SetPowerDownModeWakeupConfig();

    APP_SaveRuntimeContext();
    memset(&g_coreContext, 0, sizeof(g_coreContext));
    if (setjmp(g_coreContext) == 0)
    {
        CMC_EnterLowPowerMode(APP_CMC, &config);
    }
    /* Wake up from power down2 mode. */
    APP_RestoreRuntimeContext();
    __enable_irq();
#else
    CMC_EnterLowPowerMode(APP_CMC, &config);
#endif
}

static void APP_EnterPowerDown4Mode(void)
{
    cmc_power_domain_config_t config;

    /* Power off all radion RAMs. */
    RF_CMC1->RAM_PWR = 0x000004FF;
    /* Set NBU into Power Down Mode. */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0x7);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_PowerDownMode;
    config.wake_domain = kCMC_PowerDownMode;

    CMC_PowerOffSRAMLowPowerOnly(APP_CMC, APP_POWERDOWN4_SRAM_POWER_MODE);
    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);
#if !(defined(DEMO_NOT_SUPPORT_WAKEUP_BOOT) && DEMO_NOT_SUPPORT_WAKEUP_BOOT)
    APP_SetPowerDownModeWakeupConfig();
    APP_SaveRuntimeContext();
    memset(&g_coreContext, 0, sizeof(g_coreContext));
    if (setjmp(g_coreContext) == 0)
    {
        CMC_EnterLowPowerMode(APP_CMC, &config);
    }
    /* Wake up from power down2 mode. */
    APP_RestoreRuntimeContext();
    __enable_irq();
#else
    CMC_EnterLowPowerMode(APP_CMC, &config);
#endif
}

static void APP_EnterDeepPowerDown1Mode(void)
{
    cmc_power_domain_config_t config;

    /* Power off all radio RAMs. */
    RF_CMC1->RAM_PWR = 0x000004FF;
    /* Set NBU into Deep Power Down Mode. */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0xF);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    /* Set CORE_MAIN, CORE_WAKE into deep power down mode. */
    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_DeepPowerDown;
    config.wake_domain = kCMC_DeepPowerDown;

    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);
#if !(defined(DEMO_NOT_SUPPORT_WAKEUP_BOOT) && DEMO_NOT_SUPPORT_WAKEUP_BOOT)
    APP_SetPowerDownModeWakeupConfig();

#if (defined(FSL_FEATURE_SOC_VBAT_COUNT) && (FSL_FEATURE_SOC_VBAT_COUNT > 0))
    /* Enable VBAT backup SRAM regulator to supply STCM5, because CORE_LDO will be disabled in deep power down mode. */
    vbat_fro16k_config_t vbatFro16kConfig = {
        .enableFRO16k       = true,
        .enableFRO16kOutput = true,
    };
    VBAT_ConfigFRO16k(VBAT0, &vbatFro16kConfig);
    VBAT_EnableBandgap(VBAT0, true);
    VBAT_EnableBandgapRefreshMode(VBAT0, true);
    VBAT_EnableBackupSRAMRegulator(VBAT0, true);
#endif

    APP_SaveRuntimeContext();
    memset(&g_coreContext, 0, sizeof(g_coreContext));
    if (setjmp(g_coreContext) == 0)
    {
        CMC_EnterLowPowerMode(APP_CMC, &config);
    }
    /* Wake up from Deep power down mode. */
    APP_RestoreRuntimeContext();
    __enable_irq();
#else
#if (defined(FSL_FEATURE_SOC_VBAT_COUNT) && (FSL_FEATURE_SOC_VBAT_COUNT > 0))
    /* Enable VBAT backup SRAM regulator to supply STCM5, because CORE_LDO will be disabled in deep power down mode. */
    vbat_fro16k_config_t vbatFro16kConfig = {
        .enableFRO16k       = true,
        .enableFRO16kOutput = true,
    };
    VBAT_ConfigFRO16k(VBAT0, &vbatFro16kConfig);
    VBAT_EnableBandgap(VBAT0, true);
    VBAT_EnableBandgapRefreshMode(VBAT0, true);
    VBAT_EnableBackupSRAMRegulator(VBAT0, true);
#else
    CMC_PowerOffSRAMLowPowerOnly(APP_CMC, APP_DEEPPOWERDOWN1_SRAM_POWER_MODE);
#endif

    CMC_EnterLowPowerMode(APP_CMC, &config);
#endif
}

static inline void APP_EnterDeepPowerDown2Mode(void)
{
    cmc_power_domain_config_t config;

    /* Power off all radio RAMs. */
    RF_CMC1->RAM_PWR = 0x000004FF;
    /* Set NBU into Deep Power Down Mode. */
    RFMC->RF2P4GHZ_CTRL = (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0xF);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;

    /* Set CORE_MAIN, CORE_WAKE into deep power down mode. */
    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_DeepPowerDown;
    config.wake_domain = kCMC_DeepPowerDown;

    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);
#if !(defined(DEMO_NOT_SUPPORT_WAKEUP_BOOT) && DEMO_NOT_SUPPORT_WAKEUP_BOOT)
    APP_SetPowerDownModeWakeupConfig();
    APP_SaveRuntimeContext();
    memset(&g_coreContext, 0, sizeof(g_coreContext));
    if (setjmp(g_coreContext) == 0)
    {
        CMC_EnterLowPowerMode(APP_CMC, &config);
    }
#else
    CMC_PowerOffSRAMLowPowerOnly(APP_CMC, APP_DEEPPOWERDOWN2_SRAM_POWER_MODE);
    CMC_EnterLowPowerMode(APP_CMC, &config);
#endif
}
#endif

#if !(defined(DEMO_NOT_SUPPORT_WAKEUP_BOOT) && DEMO_NOT_SUPPORT_WAKEUP_BOOT)
static void APP_SaveRuntimeContext(void)
{
    uint32_t i = 0UL;

    g_scbVtor  = SCB->VTOR;
    g_scbIcsr  = SCB->ICSR;
    g_scbAircr = ((SCB->AIRCR) & ~SCB_AIRCR_VECTKEY_Msk) | (0x5FA << SCB_AIRCR_VECTKEY_Pos);
    g_scbCCR   = SCB->CCR;
    for (i = 0UL; i < 12UL; i++)
    {
        g_scbShp[i] = SCB->SHPR[i];
    }
    g_scbShcsr = SCB->SHCSR;
    for (i = 0UL; i < 16U; i++)
    {
        g_nvicIser[i] = NVIC->ISER[i];
    }
    for (i = 0UL; i < 496UL; i++)
    {
        g_nvicIp[i] = NVIC->IPR[i];
    }
    g_cpuControl = __get_CONTROL();
}

static void APP_RestoreRuntimeContext(void)
{
    uint32_t i;

    SCB->VTOR  = g_scbVtor;
    SCB->ICSR  = g_scbIcsr;
    SCB->AIRCR = g_scbAircr;
    SCB->CCR   = g_scbCCR;
    for (i = 0UL; i < 12UL; i++)
    {
        SCB->SHPR[i] = g_scbShp[i];
    }
    SCB->SHCSR = g_scbShcsr;
    for (i = 0UL; i < 16UL; i++)
    {
        NVIC->ISER[i] = g_nvicIser[i];
    }
    for (i = 0UL; i < 496UL; i++)
    {
        NVIC->IPR[i] = g_nvicIp[i];
    }
}

static void APP_SetPowerDownModeWakeupConfig(void)
{
    /* The first word from wakeup address must be SP. */
    g_wakeupEntry[0] = SDK_SIZEALIGN_DOWN(((uint32_t)&m_warmboot_stack_end), 8);
    /* The second word from wakeup address must be PC. */
    g_wakeupEntry[1] = (uint32_t)APP_WakeupFunction;

    for (uint32_t i = 2UL; i < 12UL; i++)
    {
        g_wakeupEntry[i] = i;
    }
    /* Set wake up address. */
    SPC_SetWakeUpValue(APP_SPC, (uint32_t)g_wakeupEntry);

    uint32_t crcResult;
    crc_config_t config;
    config.polynomial         = 0x04C11DB7U;
    config.seed               = 0xFFFFFFFFU;
    config.reflectIn          = false;
    config.reflectOut         = false;
    config.complementChecksum = false;
    config.crcBits            = kCrcBits32;
    config.crcResult          = kCrcFinalChecksum;

    CLOCK_DisableClock(kCLOCK_Crc0);
    CRC_Init(CRC0, &config);
    CRC_WriteData(CRC0, (const uint8_t *)((uint32_t)g_wakeupEntry), 48);
    crcResult        = CRC_Get32bitResult(CRC0);
    REGFILE1->REG[0] = crcResult;
}

static void APP_WakeupFunction(void)
{
    uint32_t ramAddress = 0x4000000UL;
    /* Please note that RAM ECC is enabled in KW45B41Z A1.
     * After waking up from low power modes that RAM blocks are powered off,
     * corresponding RAM blocks should be re-initialized.
     */
    while (ramAddress < 0x4004000UL)
    {
        *(uint32_t *)ramAddress = 0UL;
        ramAddress = ramAddress + 4UL;
    }

    ramAddress = 0x20004000UL;
    while (ramAddress < 0x20010000UL)
    {
        *(uint32_t *)ramAddress = 0UL;
        ramAddress = ramAddress + 4UL;
    }

    WDOG0->CS    = (uint32_t)((WDOG0->CS) & ~WDOG_CS_EN_MASK);
    WDOG0->TOVAL = 0xFFFF;
    WDOG0->CS    = (uint32_t)(((WDOG0->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK);
    WDOG0->TOVAL = 0xFFFF;
    WDOG0->CNT   = 0xD928C520U;

#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));    /* set CP10, CP11 Full Access in Secure mode */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    SCB_NS->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access in Non-secure mode */
#endif                                                    /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
#endif                                                    /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

    /* Restore the CONTROL register. */
    __ASM volatile ("MSR control, %0" : : "r" (g_cpuControl) : "memory");
    __ISB();
    longjmp(g_coreContext, true);
}
#endif
