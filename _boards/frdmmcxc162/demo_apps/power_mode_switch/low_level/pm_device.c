/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pm_device.h"

const resc_status_t g_resc_ctrl_table[kResc_Max_Num][APP_LOW_POWER_MODE_COUNT] = {
    /*! Clock modules. */
    [kResc_Fro_144M]            = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_Fro_12M]             = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Osc_32K]             = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Fro_16K]             = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},

    /*! Power domain modules */
    [kResc_LdoCore]             = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_RamRetentionLdo]     = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_Core_Vdd_Lvd_Act]    = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Sys_Vdd_Lvd_Act]     = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Sys_Vdd_Hvd_Act]     = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Core_Vdd_Lvd_Lp]     = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Sys_Vdd_Lvd_Lp]      = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Sys_Vdd_Hvd_Lp]      = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_GlitchDetector_Act]  = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_GlitchDetector_Lp]   = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    
    /*! SRAM modules */
    [kResc_RamA]                = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Lp},
 
    /*! Analog modules */
    [kResc_Opamp0]              = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Cmp0]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Cmp0_Dac]            = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
};

/* LDO voltage level and drive strength control table for all power modes. */
static const app_core_ldo_ctrl_t g_core_ldo_ctrl_table[APP_LOW_POWER_MODE_COUNT] = {
    /* Sleep */
    {
        .valid                 = true,
        .useActiveModeConfig   = true,
        .lpIREF                = false,
        .bandgapMode           = kSPC_BandgapDisabled,
        .coreLDOVoltage        = kSPC_CoreLDO_MidDriveVoltage,
        .coreLDODriveStrength  = kSPC_CoreLDO_NormalDriveStrength,
    },
    /* DeepSleep */
    {
        .valid                 = true,
        .useActiveModeConfig   = true,
        .lpIREF                = false,
        .bandgapMode           = kSPC_BandgapDisabled,
        .coreLDOVoltage        = kSPC_CoreLDO_MidDriveVoltage,
        .coreLDODriveStrength  = kSPC_CoreLDO_NormalDriveStrength,
    },
    /* PowerDown */
    {
        .valid                 = true,
        .useActiveModeConfig   = false,
        .lpIREF                = false,
        .bandgapMode           = kSPC_BandgapDisabled,
        .coreLDOVoltage        = kSPC_CoreLDO_MidDriveVoltage,
        .coreLDODriveStrength  = kSPC_CoreLDO_LowDriveStrength,
    },
    /* DeepPowerDown */
    {
        .valid                 = true,
        .useActiveModeConfig   = false,
        .lpIREF                = false,
        .bandgapMode           = kSPC_BandgapDisabled,
        .coreLDOVoltage        = kSPC_CoreLDO_MidDriveVoltage,
        .coreLDODriveStrength  = kSPC_CoreLDO_LowDriveStrength,
    },
};

/* CPU clock control table for Active/Sleep mode. Other modes are intentionally left empty. */
static const app_cpu_clock_cfg_t g_cpu_clock_cfg_table[] = {
    /* Active */
    {
        .valid  = true,
        .source = kAPP_CpuClockSrcFroHf,
        .freqHz = BOARD_BOOTCLOCKFROHF72M_CORE_CLOCK,
    },
    /* Sleep */
    {
        .valid  = true,
        .source = kAPP_CpuClockSrcFroHf,
        .freqHz = BOARD_BOOTCLOCKFROHF72M_CORE_CLOCK,
    },
};

static void SetClockPeripheralPowerStatus(resc_status_t resc_status, resc_name_t resc_name)
{
    switch (resc_name)
    {
        case kResc_Fro_144M:
            if (resc_status == kResc_Status_Off)
            {
                if ((SCG0->FIRCCSR & SCG_FIRCCSR_LK_MASK) == SCG_FIRCCSR_LK_MASK)
                {
                    SCG0->FIRCCSR &= ~SCG_FIRCCSR_LK_MASK;
                }

                SCG0->FIRCCSR &= ~SCG_FIRCCSR_FIRC_FCLK_PERIPH_EN_MASK;
                SCG0->FIRCCSR &= ~SCG_FIRCCSR_FIRC_SCLK_PERIPH_EN_MASK;
                SCG0->FIRCCSR |= SCG_FIRCCSR_LK_MASK;
            }
            else
            {
                if ((SCG0->FIRCCSR & SCG_FIRCCSR_LK_MASK) == SCG_FIRCCSR_LK_MASK)
                {
                    SCG0->FIRCCSR &= ~SCG_FIRCCSR_LK_MASK;
                }

                SCG0->FIRCCSR |= SCG_FIRCCSR_FIRC_FCLK_PERIPH_EN_MASK;
                SCG0->FIRCCSR |= SCG_FIRCCSR_FIRC_SCLK_PERIPH_EN_MASK;

                while ((SCG0->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK) == 0U)
                {
                }

                SCG0->FIRCCSR |= SCG_FIRCCSR_LK_MASK;
            }
            break;
            
        case kResc_Fro_12M:
            if (resc_status == kResc_Status_Off)
            {
                if ((SCG0->SIRCCSR & SCG_SIRCCSR_LK_MASK) == SCG_SIRCCSR_LK_MASK)
                {
                    SCG0->SIRCCSR &= ~SCG_SIRCCSR_LK_MASK;
                }

                SCG0->SIRCCSR &= ~SCG_SIRCCSR_SIRC_CLK_PERIPH_EN_MASK;
                SCG0->SIRCCSR |= SCG_SIRCCSR_LK_MASK;
            }
            else
            {
                if ((SCG0->SIRCCSR & SCG_SIRCCSR_LK_MASK) == SCG_SIRCCSR_LK_MASK)
                {
                    SCG0->SIRCCSR &= ~SCG_SIRCCSR_LK_MASK;
                }

                SCG0->SIRCCSR |= SCG_SIRCCSR_SIRC_CLK_PERIPH_EN_MASK;

                while ((SCG0->SIRCCSR & SCG_SIRCCSR_SIRCVLD_MASK) == 0U)
                {
                }

                SCG0->SIRCCSR |= SCG_SIRCCSR_LK_MASK;
            }
            break;

        case kResc_Osc_32K:
            if (resc_status == kResc_Status_Off)
            {
                 VBAT_EnableCrystalOsc32k(APP_VBAT, false);
                 VBAT_GateOsc32k(APP_VBAT, kVBAT_EnableClockToDomain0);
            }
            else
            {
                 VBAT_EnableCrystalOsc32k(APP_VBAT, true);
                 VBAT_UngateOsc32k(APP_VBAT, kVBAT_EnableClockToDomain0);
            }
            break;

        case kResc_Fro_16K:
            if (resc_status == kResc_Status_Off)
            {
                 VBAT_EnableFRO16k(APP_VBAT, false);
                 VBAT_GateFRO16k(APP_VBAT, kVBAT_EnableClockToDomain0);
            }
            else
            {
                 VBAT_EnableFRO16k(APP_VBAT, true);
                 VBAT_UngateFRO16k(APP_VBAT, kVBAT_EnableClockToDomain0);
            }
            break;
            
        default:
        {
            assert(false);
            break;
        }
    }
}

static void SetVoltagePeripheralPowerStatus(resc_status_t resc_status, resc_name_t resc_name)
{
    switch (resc_name)
    {
        case kResc_LdoCore:
            if (resc_status == kResc_Status_Off)
            {
                 SPC_EnableCoreLDORegulator(APP_SPC, false);
            }
            else
            {
                 SPC_EnableCoreLDORegulator(APP_SPC, true);
            }
            break;

        case kResc_RamRetentionLdo:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableSRAMLdo(APP_SPC, false);
            }
            else
            {
                SPC_EnableSRAMLdo(APP_SPC, true);
            }
            break;

        case kResc_Core_Vdd_Lvd_Act:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableActiveModeCoreLowVoltageDetect(APP_SPC, false);
            }
            else
            {
                SPC_EnableActiveModeCoreLowVoltageDetect(APP_SPC, true);
            }
            break;

        case kResc_Sys_Vdd_Lvd_Act:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableActiveModeSystemLowVoltageDetect(APP_SPC, false);
            }
            else
            {
                SPC_EnableActiveModeSystemLowVoltageDetect(APP_SPC, true);
            }
            break;

        case kResc_Sys_Vdd_Hvd_Act:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableActiveModeSystemHighVoltageDetect(APP_SPC, false);
            }
            else
            {
                SPC_EnableActiveModeSystemHighVoltageDetect(APP_SPC, true);
            }
            break;

        case kResc_Core_Vdd_Lvd_Lp:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableLowPowerModeCoreLowVoltageDetect(APP_SPC, false);
            }
            else
            {
                SPC_EnableLowPowerModeCoreLowVoltageDetect(APP_SPC, true);
            }
            break;

        case kResc_Sys_Vdd_Lvd_Lp:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableLowPowerModeSystemLowVoltageDetect(APP_SPC, false);
            }
            else
            {
                SPC_EnableLowPowerModeSystemLowVoltageDetect(APP_SPC, true);
            }
            break;

        case kResc_Sys_Vdd_Hvd_Lp:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableLowPowerModeSystemHighVoltageDetect(APP_SPC, false);
            }
            else
            {
                SPC_EnableLowPowerModeSystemHighVoltageDetect(APP_SPC, true);
            }
            break;
            
        case kResc_GlitchDetector_Act:
#if !(defined(FSL_FEATURE_MCX_SPC_HAS_NO_GLITCH_DETECT) && FSL_FEATURE_MCX_SPC_HAS_NO_GLITCH_DETECT)
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeVddCoreGlitchDetect(APP_SPC, true);
            }
            else
            {
                SPC_DisableActiveModeVddCoreGlitchDetect(APP_SPC, false);
            }
#endif /* FSL_FEATURE_MCX_SPC_HAS_NO_GLITCH_DETECT */
            break;

        case kResc_GlitchDetector_Lp:
#if !(defined(FSL_FEATURE_MCX_SPC_HAS_NO_GLITCH_DETECT) && FSL_FEATURE_MCX_SPC_HAS_NO_GLITCH_DETECT)
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableLowPowerModeVddCoreGlitchDetect(APP_SPC, true);
            }
            else
            {
                SPC_DisableLowPowerModeVddCoreGlitchDetect(APP_SPC, false);
            }
#endif /* FSL_FEATURE_MCX_SPC_HAS_NO_GLITCH_DETECT */
            break;
            
        default:
        {
            assert(false);
            break;
        }
    }
}

static void SetSramPowerStatus(resc_status_t resc_status, resc_name_t resc_name)
{
    if (resc_name == kResc_RamA)
    {
        if (resc_status == kResc_Status_Off)
        {
            SPC_UnRetainSRAMArray(APP_SPC, 0x1U);
        }
        else
        {
            SPC_RetainSRAMArray(APP_SPC, 0x1U);
        }
    }
    else
    {
        assert(false);
    }
}

static void SetAnalogPeripheralPowerStatus(resc_status_t resc_status, resc_name_t resc_name)
{
    switch (resc_name)
    {
        case kResc_Opamp0:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeAnalogModules(APP_SPC, kSPC_controlOpamp0);
            }
            else
            {
                SPC_EnableActiveModeAnalogModules(APP_SPC, kSPC_controlOpamp0);
            }
            break;

        case kResc_Cmp0:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeAnalogModules(APP_SPC, kSPC_controlCmp0);
            }
            else
            {
                SPC_EnableActiveModeAnalogModules(APP_SPC, kSPC_controlCmp0);
            }
            break;

        case kResc_Cmp0_Dac:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeAnalogModules(APP_SPC, kSPC_controlCmp0Dac);
            }
            else
            {
                SPC_EnableActiveModeAnalogModules(APP_SPC, kSPC_controlCmp0Dac);
            }
            break;
            
        default:
        {
            assert(false);
            break;
        }
    }
}

static void ApplyRescTableForMode(uint8_t modeIndex)
{
    for (resc_name_t resc = (resc_name_t)0; resc < (resc_name_t)kResc_Max_Num; resc++)
    {
        resc_status_t status = g_resc_ctrl_table[(uint32_t)resc][(uint32_t)modeIndex];

        if (resc <= kResc_Fro_16K)
        {
            SetClockPeripheralPowerStatus(status, resc);
        }
        else if (resc <= kResc_GlitchDetector_Lp)
        {
            SetVoltagePeripheralPowerStatus(status, resc);
        }
        else if (resc <= kResc_RamA)
        {
            SetSramPowerStatus(status, resc);
        }
        else
        {
            SetAnalogPeripheralPowerStatus(status, resc);
        }
    }
}

static inline bool GetLowPowerModeIndex(app_power_mode_t mode, uint8_t *modeIndex)
{
    bool isLowPowerMode = ((uint8_t)mode >= (uint8_t)kAPP_PowerModeSleep) &&
                          ((uint8_t)mode < (uint8_t)kAPP_PowerModeMax);
      
    if ((modeIndex == NULL) || (!isLowPowerMode))
    {
        return false;
    }

    *modeIndex = (uint8_t)mode - (uint8_t)kAPP_PowerModeSleep;

    return (*modeIndex < APP_LOW_POWER_MODE_COUNT);
}

static void SetRegulatorsConfig(app_power_mode_t targetPowerMode)
{
    uint8_t modeIndex = 0U;
    bool modeIndexValid = GetLowPowerModeIndex(targetPowerMode, &modeIndex);

    assert(modeIndexValid);
    if (!modeIndexValid)
    {
        return;
    }

    const app_core_ldo_ctrl_t *ctrl = &g_core_ldo_ctrl_table[modeIndex];

    assert(ctrl->valid);
    if (!ctrl->valid)
    {
        return;
    }

    if (ctrl->useActiveModeConfig)
    {
        spc_active_mode_regulators_config_t activeModeRegulatorOption;

        activeModeRegulatorOption.bandgapMode = ctrl->bandgapMode;
        activeModeRegulatorOption.CoreLDOOption.CoreLDOVoltage = ctrl->coreLDOVoltage;
        activeModeRegulatorOption.CoreLDOOption.CoreLDODriveStrength = ctrl->coreLDODriveStrength;

        (void)SPC_SetActiveModeRegulatorsConfig(APP_SPC, &activeModeRegulatorOption);
    }
    else
    {
        spc_lowpower_mode_regulators_config_t lowPowerRegulatorOption;

        lowPowerRegulatorOption.lpIREF = ctrl->lpIREF;
        lowPowerRegulatorOption.bandgapMode = ctrl->bandgapMode;
        lowPowerRegulatorOption.CoreLDOOption.CoreLDOVoltage       = ctrl->coreLDOVoltage;
        lowPowerRegulatorOption.CoreLDOOption.CoreLDODriveStrength = ctrl->coreLDODriveStrength;

        (void)SPC_SetLowPowerModeRegulatorsConfig(APP_SPC, &lowPowerRegulatorOption);
    }
}

static const app_cpu_clock_cfg_t *GetCpuClockCfg(app_power_mode_t mode)
{
    switch (mode)
    {
        case kAPP_PowerModeActive:
            return &g_cpu_clock_cfg_table[0];
        case kAPP_PowerModeSleep:
            return &g_cpu_clock_cfg_table[1];
        default:
            return NULL;
    }
}

/*! Please note   */
static void ApplyCpuClockCfg(const app_cpu_clock_cfg_t *cfg)
{
    if ((cfg == NULL) || (!cfg->valid))
    {
        return;
    }

    if ((cfg->source == kAPP_CpuClockSrcFro12M) && (cfg->freqHz == BOARD_BOOTCLOCKFRO12M_CORE_CLOCK))
    {
        BOARD_BootClockFRO12M();
    }
    else if ((cfg->source == kAPP_CpuClockSrcFroHf) && (cfg->freqHz == BOARD_BOOTCLOCKFROHF36M_CORE_CLOCK))
    {
        BOARD_BootClockFROHF36M();
    }
    else if ((cfg->source == kAPP_CpuClockSrcFroHf) && (cfg->freqHz == BOARD_BOOTCLOCKFROHF72M_CORE_CLOCK))
    {
        BOARD_BootClockFROHF72M();
    }
    else
    {
        assert(false);
    }
}

void APP_PowerPreSwitchHook(app_power_mode_t targetPowerMode)
{
    uint8_t modeIndex = 0U;
    bool modeIndexValid = GetLowPowerModeIndex(targetPowerMode, &modeIndex);

    assert(modeIndexValid);

    if (!modeIndexValid)
    {
        return;
    }

    /* Wait for debug console output finished. */
    while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
    {
    }

    DbgConsole_Deinit();
    PORT_SetPinMux(APP_DEBUG_CONSOLE_RX_PORT, APP_DEBUG_CONSOLE_RX_PIN, kPORT_MuxAsGpio);
    PORT_SetPinMux(APP_DEBUG_CONSOLE_TX_PORT, APP_DEBUG_CONSOLE_TX_PIN, kPORT_MuxAsGpio);

    ApplyCpuClockCfg(GetCpuClockCfg(targetPowerMode));
    ApplyRescTableForMode(modeIndex);
    SetRegulatorsConfig(targetPowerMode);
 
    /* Keep SPC wakeup delay in sync for low power entry. */
    SPC_SetLowPowerWakeUpDelay(APP_SPC, APP_SPC_LDO_LPWKUP_DELAY);
}

void APP_PowerPostSwitchHook(void)
{
    BOARD_InitHardware();
}
