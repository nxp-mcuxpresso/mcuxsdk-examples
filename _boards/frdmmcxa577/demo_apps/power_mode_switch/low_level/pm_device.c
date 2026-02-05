/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pm_device.h"
#include "app.h"
#include "board.h"
#include "fsl_cmc.h"
#include "fsl_spc.h"
#include "fsl_vbat.h"
#include "fsl_port.h"
#include "fsl_lpuart.h"
#include "fsl_debug_console.h"
#include "power_mode_switch.h"

const resc_status_t g_resc_ctrl_table[kResc_Max_Num][APP_LOW_POWER_MODE_COUNT] = {
    /*! Clock modules */
    [kResc_Fro_192M]            = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Fro_12M]             = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Fro_16K]             = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_Osc_Rtc]             = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    
    /*! Keep SPLL and SOSC always open, because in the current demo, the CPU clock comes from SPLL, and the source of SPLL is SOSC,
     * so we cannot clock them at runtime.
     */
    [kResc_Osc_Sys]             = {kResc_Status_On,    kResc_Status_On,       kResc_Status_On,       kResc_Status_On},
    [kResc_Spll]                = {kResc_Status_On,    kResc_Status_On,       kResc_Status_On,       kResc_Status_On},

    /*! Power domain modules */
    [kResc_LdoCore]             = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_RamRetentionLdo]     = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_Core_Vdd_Lvd_Act]    = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Core_Vdd_Hvd_Act]    = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Sys_Vdd_Lvd_Act]     = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Sys_Vdd_Hvd_Act]     = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Core_Vdd_Lvd_Lp]     = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Core_Vdd_Hvd_Lp]     = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Sys_Vdd_Lvd_Lp]      = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Sys_Vdd_Hvd_Lp]      = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_GlitchDetector_Act]  = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_GlitchDetector_Lp]   = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    
    /*! SRAM modules */
    [kResc_RamA0]               = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Lp},
    [kResc_RamA3]               = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Lp},
    
    /* In certain power modes, the hardware will power off SRAM. Even if you select power on or retain here, it will have no effect. */
    [kResc_RamX]                = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Lp},
    [kResc_RamA]                = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Lp},
    [kResc_RamB]                = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Lp},

    /*! Analog modules */
    [kResc_Vref]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Usb]                 = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Vbat]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Dac0]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Dac1]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Tsi0]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Cmp0]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Cmp0_Dac]            = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
};

static void SetSystemPeripheralPowerStatus(resc_status_t resc_status, resc_name_t resc_name)
{
    switch (resc_name)
    {
        case kResc_Fro_192M:
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

        case kResc_Osc_Rtc:
            if (resc_status == kResc_Status_Off)
            {
                 VBAT_EnableCrystalOsc32k(APP_VBAT, false);
            }
            else
            {
                 VBAT_EnableCrystalOsc32k(APP_VBAT, true);
            }
            break;

        case kResc_Osc_Sys:
            if (resc_status == kResc_Status_Off)
            {
                if ((SCG0->SOSCCSR & SCG_SOSCCSR_LK_MASK) == SCG_SOSCCSR_LK_MASK)
                {
                    SCG0->SOSCCSR &= ~SCG_SOSCCSR_LK_MASK;
                }

                SCG0->SOSCCSR &= ~SCG_SOSCCSR_SOSCEN_MASK;
                SCG0->SOSCCSR |= SCG_SOSCCSR_LK_MASK;
            }
            else
            {
                if ((SCG0->SOSCCSR & SCG_SOSCCSR_LK_MASK) == SCG_SOSCCSR_LK_MASK)
                {
                    SCG0->SOSCCSR &= ~SCG_SOSCCSR_LK_MASK;
                }

                SCG0->SOSCCSR |= SCG_SOSCCSR_SOSCEN_MASK;
                SCG0->SOSCCSR |= SCG_SOSCCSR_LK_MASK;
            }
            break; 

        case kResc_Spll:
            if (resc_status == kResc_Status_Off)
            {
                if ((SCG0->SPLLCSR & SCG_SPLLCSR_SPLL_LOCK_MASK) == SCG_SPLLCSR_SPLL_LOCK_MASK)
                {
                    SCG0->SPLLCSR &= ~SCG_SPLLCSR_SPLL_LOCK_MASK;
                }
                
                SCG0->SPLLCSR &= ~(SCG_SPLLCSR_SPLLPWREN_MASK | SCG_SPLLCSR_SPLLCLKEN_MASK);
                SCG0->SPLLCSR &= ~SCG_SPLLCSR_SPLL_LOCK_MASK;
            }
            else
            {
                if ((SCG0->SPLLCSR & SCG_SPLLCSR_SPLL_LOCK_MASK) == SCG_SPLLCSR_SPLL_LOCK_MASK)
                {
                    SCG0->SPLLCSR &= ~SCG_SPLLCSR_SPLL_LOCK_MASK;
                }
                
                SCG0->SPLLCSR |= (SCG_SPLLCSR_SPLLPWREN_MASK | SCG_SPLLCSR_SPLLCLKEN_MASK);
                SCG0->SPLLCSR &= ~SCG_SPLLCSR_SPLL_LOCK_MASK;
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
                if (kStatus_Success != VBAT_EnableBackupSRAMRegulator(APP_VBAT, false))
                {
                    assert(false);
                }
                VBAT_EnableBandgapRefreshMode(APP_VBAT, false);
                VBAT_EnableBandgap(APP_VBAT, false);
            }
            else
            {
                /* FRO16K must be enabled before enabling the Bandgap: set kResc_Fro_16K to kResc_Status_On. */
                VBAT_EnableBandgap(APP_VBAT, true);
                VBAT_EnableBandgapRefreshMode(APP_VBAT, true);
                if (kStatus_Success != VBAT_EnableBackupSRAMRegulator(APP_VBAT, true))
                {
                    assert(false);
                }
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

        case kResc_Core_Vdd_Hvd_Act:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableActiveModeCoreHighVoltageDetect(APP_SPC, false);
            }
            else
            {
                SPC_EnableActiveModeCoreHighVoltageDetect(APP_SPC, true);
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

        case kResc_Core_Vdd_Hvd_Lp:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableLowPowerModeCoreHighVoltageDetect(APP_SPC, false);
            }
            else
            {
                SPC_EnableLowPowerModeCoreHighVoltageDetect(APP_SPC, true);
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
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeVddCoreGlitchDetect(APP_SPC, true);
            }
            else
            {
                SPC_DisableActiveModeVddCoreGlitchDetect(APP_SPC, false);
            }
            break;
            
        case kResc_GlitchDetector_Lp:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableLowPowerModeVddCoreGlitchDetect(APP_SPC, true);
            }
            else
            {
                SPC_DisableLowPowerModeVddCoreGlitchDetect(APP_SPC, false);
            }
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
    uint32_t sramMask = 0U;

    if (resc_name >= kResc_RamX)
    {
        sramMask = (1UL << (uint32_t)(resc_name - kResc_RamX));

        if (resc_status == kResc_Status_Off)
        {
            CMC_PowerOffSRAMAllMode(APP_CMC, sramMask);
        }
        else
        {
            /* Treat ON/LP as retained in low power modes. */
            CMC_PowerOnSRAMAllMode(APP_CMC, sramMask);
            CMC_PowerOnSRAMLowPowerOnly(APP_CMC, sramMask);
        }
    }
    else
    {
        sramMask = (1UL << (uint32_t)(resc_name - kResc_RamA0));

        if (resc_status == kResc_Status_Off)
        {
            VBAT_PowerOffSRAMsInLowPowerModes(APP_VBAT, (uint8_t)sramMask);
        }
        else
        {
            VBAT_RetainSRAMsInLowPowerModes(APP_VBAT, (uint8_t)sramMask);
            /* Default to SoC supply; board can extend to VBAT LDO supply if needed. */
            VBAT_SwitchSRAMPowerBySocSupply(APP_VBAT);
        }
    }
}

static void SetAnalogPeripheralPowerStatus(resc_status_t resc_status, resc_name_t resc_name)
{
    switch (resc_name)
    {
        case kResc_Vref:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeAnalogModules(APP_SPC, kSPC_controlVref);
            }
            else
            {
                SPC_EnableActiveModeAnalogModules(APP_SPC, kSPC_controlVref);
            }
            break;

        case kResc_Usb:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeAnalogModules(APP_SPC, kSPC_controlUsb3vDet);
            }
            else
            {
                SPC_EnableActiveModeAnalogModules(APP_SPC, kSPC_controlUsb3vDet);
            }
            break;

        case kResc_Vbat:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeAnalogModules(APP_SPC, kSPC_controlVbat);
            }
            else
            {
                SPC_EnableActiveModeAnalogModules(APP_SPC, kSPC_controlVbat);
            }
            break;

        case kResc_Dac0:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeAnalogModules(APP_SPC, kSPC_controlDac0);
            }
            else
            {
                SPC_EnableActiveModeAnalogModules(APP_SPC, kSPC_controlDac0);
            }
            break;

        case kResc_Dac1:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeAnalogModules(APP_SPC, kSPC_controlDac1);
            }
            else
            {
                SPC_EnableActiveModeAnalogModules(APP_SPC, kSPC_controlDac1);
            }
            break;

        case kResc_Tsi0:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeAnalogModules(APP_SPC, kSPC_controlTsi0);
            }
            else
            {
                SPC_EnableActiveModeAnalogModules(APP_SPC, kSPC_controlTsi0);
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

        if (resc <= kResc_Spll)
        {
            SetSystemPeripheralPowerStatus(status, resc);
        }
        else if (resc <= kResc_GlitchDetector_Lp)
        {
            SetVoltagePeripheralPowerStatus(status, resc);
        }
        else if (resc <= kResc_RamB)
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

/*!
 * Note:
 *  1. When setting LDO_CORE Drive Strength to Normal, BGMODE must be programmed to a value that enables the Bandgap.
 *  2. Set LDO_CORE drive strength to Low will be ignored if LVD/HVDs are kept enabled.
 *  3. LDO_CORE overdrive voltage level (1.15 V) is not supported when LDO_CORE is configured for low drive.
 */
static void SetRegulatorsConfig(app_power_mode_t targetPowerMode)
{
        switch (targetPowerMode)
        {
            case kAPP_PowerModeSleep:
              {
                spc_active_mode_regulators_config_t activeModeRegulatorOption;
                
                activeModeRegulatorOption.lpBuff                                = false;
                activeModeRegulatorOption.bandgapMode                           = kSPC_BandgapDisabled;
                activeModeRegulatorOption.CoreLDOOption.CoreLDOVoltage          = kSPC_CoreLDO_MidDriveVoltage;
                activeModeRegulatorOption.CoreLDOOption.CoreLDODriveStrength    = kSPC_CoreLDO_LowDriveStrength;
                
                (void)SPC_SetActiveModeRegulatorsConfig(APP_SPC, &activeModeRegulatorOption);

                break;
              }
            case kAPP_PowerModeDeepSleep:
              {
                spc_lowpower_mode_regulators_config_t lowPowerRegulatorOption;

                lowPowerRegulatorOption.lpIREF                             = false;
                lowPowerRegulatorOption.bandgapMode                        = kSPC_BandgapDisabled;
                lowPowerRegulatorOption.CoreLDOOption.CoreLDOVoltage       = kSPC_CoreLDO_MidDriveVoltage;
                lowPowerRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_LowDriveStrength;

                (void)SPC_SetLowPowerModeRegulatorsConfig(APP_SPC, &lowPowerRegulatorOption);
    
                break;
              }
            case kAPP_PowerModePowerDown:
              {
                spc_lowpower_mode_regulators_config_t lowPowerRegulatorOption;

                lowPowerRegulatorOption.lpIREF                             = false;
                lowPowerRegulatorOption.bandgapMode                        = kSPC_BandgapDisabled;
                lowPowerRegulatorOption.CoreLDOOption.CoreLDOVoltage       = kSPC_Core_LDO_RetentionVoltage;
                lowPowerRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_LowDriveStrength;

                (void)SPC_SetLowPowerModeRegulatorsConfig(APP_SPC, &lowPowerRegulatorOption);
    
                break;
              }
            case kAPP_PowerModeDeepPowerDown:
              {
                spc_lowpower_mode_regulators_config_t lowPowerRegulatorOption;

                lowPowerRegulatorOption.lpIREF                             = false;
                lowPowerRegulatorOption.bandgapMode                        = kSPC_BandgapDisabled;
                lowPowerRegulatorOption.CoreLDOOption.CoreLDOVoltage       = kSPC_Core_LDO_RetentionVoltage;
                lowPowerRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_LowDriveStrength;

                (void)SPC_SetLowPowerModeRegulatorsConfig(APP_SPC, &lowPowerRegulatorOption);

                break;
              }
            default:
                assert(false);
                break;
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

    ApplyRescTableForMode(modeIndex);
    
    SetRegulatorsConfig(targetPowerMode);
 
    /* Keep SPC wakeup delay in sync for low power entry. */
    SPC_SetLowPowerWakeUpDelay(APP_SPC, APP_LDO_LPWKUP_DELAY);
}

void APP_PowerPostSwitchHook(void)
{
    BOARD_InitHardware();
}
