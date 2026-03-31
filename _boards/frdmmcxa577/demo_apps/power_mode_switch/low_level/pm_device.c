/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pm_device.h"

static const resc_status_t g_resc_ctrl_table[kResc_Max_Num][APP_LOW_POWER_MODE_COUNT] = {
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
    [kResc_LpIref]              = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_LpBuffer_Act]        = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_LpBuffer_Lp]         = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
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
    
    [kResc_Flash]               = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Lp},
    [kResc_Lpcac]               = {kResc_Status_On,     kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
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

    /*! Peripheral clocks */
    [kResc_RamA_Clk]            = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_RamB_Clk]            = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_RamX_Clk]            = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Peri_clk_all]        = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
};

/* LDO voltage level and drive strength control table for all power modes. */
static const app_core_ldo_ctrl_t g_core_ldo_ctrl_table[APP_LOW_POWER_MODE_COUNT] = {
    /* Sleep */
    {
        .valid                 = true,
        .useActiveModeConfig   = true,
        .coreLDOVoltage        = kSPC_CoreLDO_OverDriveVoltage,
        .coreLDODriveStrength  = kSPC_CoreLDO_NormalDriveStrength,
    },
    /* DeepSleep */
    {
        .valid                 = true,
        .useActiveModeConfig   = false,
        .coreLDOVoltage        = kSPC_CoreLDO_MidDriveVoltage,
        .coreLDODriveStrength  = kSPC_CoreLDO_LowDriveStrength,
    },
    /* PowerDown */
    {
        .valid                 = true,
        .useActiveModeConfig   = false,
        .coreLDOVoltage        = kSPC_Core_LDO_RetentionVoltage,
        .coreLDODriveStrength  = kSPC_CoreLDO_LowDriveStrength,
    },
    /* DeepPowerDown */
    {
        .valid                 = true,
        .useActiveModeConfig   = false,
        .coreLDOVoltage        = kSPC_Core_LDO_RetentionVoltage,
        .coreLDODriveStrength  = kSPC_CoreLDO_LowDriveStrength,
    },
};

/* CPU clock control table for Active/Sleep mode. Other modes are intentionally left empty. */
static const app_cpu_clock_cfg_t g_cpu_clock_cfg_table[] = {
    /* Active */
    {
        .valid  = true,
        .source = kAPP_CpuClockSrcPll,
        .freqHz = BOARD_BOOTCLOCKPLL240M_CORE_CLOCK,
    },
    /* Sleep */
    {
        .valid  = true,
        .source = kAPP_CpuClockSrcPll,
        .freqHz = BOARD_BOOTCLOCKPLL240M_CORE_CLOCK,
    },
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

        case kResc_LpIref:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableLowPowerModeLowPowerIREF(APP_SPC, false);
            }
            else
            {
                SPC_EnableLowPowerModeLowPowerIREF(APP_SPC, true);
            }
            break;

        case kResc_LpBuffer_Act:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableActiveModeCMPBandgapBuffer(APP_SPC, false);
            }
            else
            {
                SPC_EnableActiveModeCMPBandgapBuffer(APP_SPC, true);
            }
            break;
            
        case kResc_LpBuffer_Lp:
            if (resc_status == kResc_Status_Off)
            {
                SPC_EnableLowPowerModeCMPBandgapBufferMode(APP_SPC, false);
            }
            else
            {
                SPC_EnableLowPowerModeCMPBandgapBufferMode(APP_SPC, true);
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

static void SetMemoryPowerStatus(resc_status_t resc_status, resc_name_t resc_name)
{
    uint32_t sramMask = 0U;
    uint8_t vbatSramMask = 0U;

    if (resc_name == kResc_Flash)
    {
        if (resc_status == kResc_Status_Off)
        {
            /* Flash memory is placed in low power state. */
            CMC_ConfigFlashMode(APP_CMC, false, false, true);
        }
        else if (resc_status == kResc_Status_Lp)
        {
            /* Flash is disabled while core is sleeping, wake on access. */
            CMC_ConfigFlashMode(APP_CMC, true, true, false);
        }
        else
        {
            /* Flash is always on. */
            CMC_ConfigFlashMode(APP_CMC, false, false, false);
        }
    }
    else if (resc_name == kResc_Lpcac)
    {
        if (resc_status == kResc_Status_Off)
        {
            /* Disable LPCAC. */
            SYSCON->LPCAC_CTRL |= SYSCON_LPCAC_CTRL_DIS_LPCAC_MASK;
        }
        else
        {
            /* Enable LPCAC. */
            SYSCON->LPCAC_CTRL &= ~SYSCON_LPCAC_CTRL_DIS_LPCAC_MASK;
        }
    }
    else if (resc_name >= kResc_RamX)
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
        assert(sramMask <= UINT8_MAX);
        vbatSramMask = (uint8_t)sramMask;

        if (resc_status == kResc_Status_Off)
        {
            VBAT_PowerOffSRAMsInLowPowerModes(APP_VBAT, vbatSramMask);
        }
        else
        {
            VBAT_RetainSRAMsInLowPowerModes(APP_VBAT, vbatSramMask);
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

/* List of peripheral clocks to control for kResc_Peri_clk_all. */
static const clock_ip_name_t g_peri_clk_list[] = {
    kCLOCK_GateCTIMER0,
    kCLOCK_GateCTIMER1,
    kCLOCK_GateCTIMER2,
    kCLOCK_GateCTIMER3,
    kCLOCK_GateCTIMER4,
    kCLOCK_GateDMA0,
    kCLOCK_GateDMA1,
    kCLOCK_GateCRC0,
    kCLOCK_GateFLEXIO0,
    kCLOCK_GateLPI2C0,
    kCLOCK_GateLPI2C1,
    kCLOCK_GateLPI2C2,
    kCLOCK_GateLPI2C3,
    kCLOCK_GateLPI2C4,
    kCLOCK_GateLPSPI0,
    kCLOCK_GateLPSPI1,
    kCLOCK_GateLPSPI2,
    kCLOCK_GateLPSPI3,
    kCLOCK_GateLPSPI4,
    kCLOCK_GateLPSPI5,
    kCLOCK_GateADC0,
    kCLOCK_GateADC1,
    kCLOCK_GateI3C0,
    kCLOCK_GateI3C1,
    kCLOCK_GateI3C2,
    kCLOCK_GateI3C3,
    kCLOCK_GateFLEXCAN0,
    kCLOCK_GateFLEXCAN1,
};

static void SetPeripheralClockStatus(resc_status_t resc_status, resc_name_t resc_name)
{
    switch (resc_name)
    {
        case kResc_RamA_Clk:
            if (resc_status == kResc_Status_Off)
            {
                /* Enable RAMA auto clock gating. */
                SYSCON->RAM_CTRL &= ~SYSCON_RAM_CTRL_RAMA_CG_OVERRIDE_MASK;
            }
            else
            {
                /* Disable RAMA auto clock gating (clock always on). */
                SYSCON->RAM_CTRL |= SYSCON_RAM_CTRL_RAMA_CG_OVERRIDE_MASK;
            }
            break;

        case kResc_RamB_Clk:
            if (resc_status == kResc_Status_Off)
            {
                /* Enable RAMB auto clock gating. */
                SYSCON->RAM_CTRL &= ~SYSCON_RAM_CTRL_RAMB_CG_OVERRIDE_MASK;
            }
            else
            {
                /* Disable RAMB auto clock gating (clock always on). */
                SYSCON->RAM_CTRL |= SYSCON_RAM_CTRL_RAMB_CG_OVERRIDE_MASK;
            }
            break;

        case kResc_RamX_Clk:
            if (resc_status == kResc_Status_Off)
            {
                /* Enable RAMX auto clock gating. */
                SYSCON->RAM_CTRL &= ~SYSCON_RAM_CTRL_RAMX_CG_OVERRIDE_MASK;
            }
            else
            {
                /* Disable RAMX auto clock gating (clock always on). */
                SYSCON->RAM_CTRL |= SYSCON_RAM_CTRL_RAMX_CG_OVERRIDE_MASK;
            }
            break;

        case kResc_Peri_clk_all:
            if (resc_status == kResc_Status_Off)
            {
                for (uint32_t i = 0U; i < ARRAY_SIZE(g_peri_clk_list); i++)
                {
                    CLOCK_DisableClock(g_peri_clk_list[i]);
                }
            }
            else
            {
                for (uint32_t i = 0U; i < ARRAY_SIZE(g_peri_clk_list); i++)
                {
                    CLOCK_EnableClock(g_peri_clk_list[i]);
                }
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
            SetMemoryPowerStatus(status, resc);
        }
        else if (resc <= kResc_Cmp0_Dac)
        {
            SetAnalogPeripheralPowerStatus(status, resc);
        }
        else
        {
            SetPeripheralClockStatus(status, resc);
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
        spc_active_mode_core_ldo_option_t option;

        option.CoreLDOVoltage = ctrl->coreLDOVoltage;
        option.CoreLDODriveStrength = ctrl->coreLDODriveStrength;

        (void)SPC_SetActiveModeCoreLDORegulatorConfig(APP_SPC, &option);
    }
    else
    {
        spc_lowpower_mode_core_ldo_option_t option;

        option.CoreLDOVoltage       = ctrl->coreLDOVoltage;
        option.CoreLDODriveStrength = ctrl->coreLDODriveStrength;

        (void)SPC_SetLowPowerModeCoreLDORegulatorConfig(APP_SPC, &option);
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
    else if ((cfg->source == kAPP_CpuClockSrcFroHf) && (cfg->freqHz == BOARD_BOOTCLOCKFROHF192M_CORE_CLOCK))
    {
        BOARD_BootClockFROHF192M();
    }
    else if ((cfg->source == kAPP_CpuClockSrcPll) && (cfg->freqHz == BOARD_BOOTCLOCKPLL200M_CORE_CLOCK))
    {
        BOARD_BootClockPLL200M();
    }
    else if ((cfg->source == kAPP_CpuClockSrcPll) && (cfg->freqHz == BOARD_BOOTCLOCKPLL240M_CORE_CLOCK))
    {
        BOARD_BootClockPLL240M();
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
    SPC_SetLowPowerWakeUpDelay(APP_SPC, APP_LDO_LPWKUP_DELAY);
}

void APP_PowerPostSwitchHook(void)
{ 
    BOARD_InitHardware();
}
