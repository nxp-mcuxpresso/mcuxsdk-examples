/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pm_device.h"
#include "fsl_lptmr.h"
#include "pin_mux.h"

#ifndef APP_POWERDOWN_WAKE_RESTORE_TIMEOUT
#define APP_POWERDOWN_WAKE_RESTORE_TIMEOUT 1000000U
#endif

static const resc_status_t g_resc_ctrl_table[kResc_Max_Num][APP_LOW_POWER_MODE_COUNT] = {
    /*! Clock modules */
    [kResc_Fro_192M]            = {kResc_Status_Lp,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_Fro_12M]             = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Fro_16K]             = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_Osc_Rtc]             = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Osc_Sys]             = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_Off},
    [kResc_Spll]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},

    /*! Power domain modules */
    [kResc_LdoCore]             = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_ScgLdo]              = {kResc_Status_Lp,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Lp},
    [kResc_RamRetentionLdo]     = {kResc_Status_Off,     kResc_Status_On,        kResc_Status_On,        kResc_Status_Off},
    [kResc_VbatBandgap]         = {kResc_Status_Off,    kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_VbatBandgapRefresh]  = {kResc_Status_Off,    kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_LpIref]              = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_LpBuffer_Act]        = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_LpBuffer_Lp]         = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Core_Vdd_Lvd_Act]    = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_Core_Vdd_Hvd_Act]    = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_Sys_Vdd_Lvd_Act]     = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_Sys_Vdd_Hvd_Act]     = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_Core_Vdd_Lvd_Lp]     = {kResc_Status_Off,    kResc_Status_On,        kResc_Status_Off,       kResc_Status_Off},
    [kResc_Core_Vdd_Hvd_Lp]     = {kResc_Status_Off,    kResc_Status_On,        kResc_Status_Off,       kResc_Status_Off},
    [kResc_Sys_Vdd_Lvd_Lp]      = {kResc_Status_Off,    kResc_Status_On,        kResc_Status_Off,       kResc_Status_Off},
    [kResc_Sys_Vdd_Hvd_Lp]      = {kResc_Status_Off,    kResc_Status_On,        kResc_Status_Off,       kResc_Status_Off},
    [kResc_Glitch_Detector_Act] = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_Glitch_Detector_Lp]  = {kResc_Status_Off,    kResc_Status_On,        kResc_Status_Off,       kResc_Status_Off},
    [kResc_SpcSocCtrl_Act]      = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_On},
    [kResc_SpcSocCtrl_Lp]       = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_Off},
    
    [kResc_Flash]               = {kResc_Status_Lp,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Lp},
    [kResc_Lpcac]               = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Lp},
    [kResc_SysconRamCtrl]       = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_Lp},
    /*! SRAM modules */
    [kResc_RamA0]               = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Off},
    [kResc_RamA3]               = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Off},
    
    /*! In certain power modes, the hardware will power off SRAM. Even if you select power on or retain here, it will have no effect. */
    [kResc_RamX]                = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Off},
    [kResc_RamA]                = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Off},
    [kResc_RamB]                = {kResc_Status_On,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Off},
    [kResc_Ram_Lpcac]           = {kResc_Status_Lp,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Off},
    [kResc_Ram_Dma_Can_Enet]    = {kResc_Status_Lp,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Off},
    [kResc_Ram_Usb_FlexSpi]     = {kResc_Status_Lp,     kResc_Status_Lp,        kResc_Status_Lp,        kResc_Status_Off},

    /*! Analog modules */
    [kResc_Vref]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Usb]                 = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Vbat]                = {kResc_Status_On,     kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Dac0]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Dac1]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Tsi0]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Cmp0]                = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Cmp0_Dac]            = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Vbat_Lp]             = {kResc_Status_On,     kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_VbatDpdPower]        = {kResc_Status_On,     kResc_Status_On,        kResc_Status_On,        kResc_Status_Off},
    [kResc_Clkmon]              = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},

    /*! Peripheral clocks */
    [kResc_RamA_Clk]            = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_RamB_Clk]            = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_RamX_Clk]            = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
    [kResc_Peri_clk_all]        = {kResc_Status_Off,    kResc_Status_Off,       kResc_Status_Off,       kResc_Status_Off},
};

/*! LDO voltage level and drive strength control table for all power modes. */
static const app_core_ldo_ctrl_t g_core_ldo_ctrl_table[APP_LOW_POWER_MODE_COUNT] = {
    /*! Sleep */
    {
        .valid                 = true,
        .useActiveModeConfig   = true,
        .bandgapMode           = kSPC_BandgapEnabledBufferDisabled,
        .coreLDOVoltage        = kSPC_CoreLDO_OverDriveVoltage,
        .coreLDODriveStrength  = kSPC_CoreLDO_NormalDriveStrength,
    },
    /*! DeepSleep */
    {
        .valid                 = true,
        .useActiveModeConfig   = false,
        .bandgapMode           = kSPC_BandgapEnabledBufferDisabled,
        .coreLDOVoltage        = kSPC_CoreLDO_OverDriveVoltage,
        .coreLDODriveStrength  = kSPC_CoreLDO_NormalDriveStrength,
    },
    /*! PowerDown */
    {
        .valid                       = true,
        .useActiveModeConfig         = false,
        .bandgapMode                 = kSPC_BandgapDisabled,
        .coreLDOVoltage              = kSPC_Core_LDO_RetentionVoltage,
        .coreLDODriveStrength        = kSPC_CoreLDO_LowDriveStrength,
        .updateActiveModeConfig      = true,
        .activeBandgapMode           = kSPC_BandgapEnabledBufferDisabled,
        .activeCoreLDOVoltage        = kSPC_CoreLDO_MidDriveVoltage,
        .activeCoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength,
    },
    /*! DeepPowerDown */
    {
        .valid                       = true,
        .useActiveModeConfig         = false,
        .bandgapMode                 = kSPC_BandgapDisabled,
        .coreLDOVoltage              = kSPC_CoreLDO_MidDriveVoltage,
        .coreLDODriveStrength        = kSPC_CoreLDO_LowDriveStrength,
        .updateActiveModeConfig      = true,
        .activeBandgapMode           = kSPC_BandgapEnabledBufferDisabled,
        .activeCoreLDOVoltage        = kSPC_CoreLDO_MidDriveVoltage,
        .activeCoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength,
    },
};

/*! CPU clock control table for active and low power mode. */
static const app_cpu_clock_cfg_t g_cpu_clock_cfg_table[] = {
    /*! Active */
    {
        .valid  = true,
        .source = kAPP_CpuClockSrcPll,
        .freqHz = BOARD_BOOTCLOCKPLL240M_CORE_CLOCK,
    },
    /*! Sleep */
    {
        .valid  = true,
        .source = kAPP_CpuClockSrcFroHf,
        .freqHz = BOARD_BOOTCLOCKFROHF192M_CORE_CLOCK,
    },
    /*! DeepSleep */
    {
        .valid  = true,
        .source = kAPP_CpuClockSrcFroHf,
        .freqHz = BOARD_BOOTCLOCKFROHF192M_CORE_CLOCK,
    },
    /*! PowerDown */
    {
        .valid  = true,
        .source = kAPP_CpuClockSrcFroHf,
        .freqHz = BOARD_BOOTCLOCKFROHF48M_CORE_CLOCK,
    },
    /*! DeepPowerDown */
    {
        .valid  = true,
        .source = kAPP_CpuClockSrcFroHf,
        .freqHz = BOARD_BOOTCLOCKFROHF48M_CORE_CLOCK,
    },
};

#define APP_SCG_LDO_VOUT_SEL_1V1             4U
#define APP_SPC_SOC_CNTRL_WAKE_TIMER_CLOCK    (1UL << 2U)

static const uint8_t sram_ret_map[] =
{
    [kResc_RamA0] = 0U, /* RET0 -> RAMA0 */
    [kResc_RamA3] = 3U, /* RET3 -> RAMA3 */
};

static const uint8_t sram_map[] =
{
    [kResc_RamX] = 0U,              /* Bit0 -> RAMX0~RAMX3 */
    [kResc_RamA] = 1U,              /* Bit1 -> RAMA1/A2/A4~A11 */
    [kResc_RamB] = 2U,              /* Bit2 -> RAMB0~RAMB7 */
    [kResc_Ram_Lpcac] = 24U,        /* Bit24 -> LPCAC */
    [kResc_Ram_Dma_Can_Enet] = 25U, /* Bit25 -> DMA/CAN/ENET */
    [kResc_Ram_Usb_FlexSpi] = 26U,  /* Bit26 -> USB/FlexSPI */
};

/* List of peripheral clocks to control for kResc_Peri_clk_all. */
static const clock_ip_name_t g_peri_clk_list[] = {
    kCLOCK_GateINPUTMUX0,
    kCLOCK_GateFREQME,
    kCLOCK_GateCTIMER0,
    kCLOCK_GateCTIMER1,
    kCLOCK_GateCTIMER2,
    kCLOCK_GateCTIMER3,
    kCLOCK_GateCTIMER4,
    kCLOCK_GateUTICK0,
    kCLOCK_GateWWDT0,
    kCLOCK_GateWWDT1,
    kCLOCK_GateDMA0,
    kCLOCK_GateDMA1,
    kCLOCK_GateAOI0,
    kCLOCK_GateCRC0,
    kCLOCK_GateEIM0,
    kCLOCK_GateERM0,
    kCLOCK_GateFLEXIO0,
    kCLOCK_GateLPI2C0,
    kCLOCK_GateLPI2C1,
    kCLOCK_GateLPI2C2,
    kCLOCK_GateLPI2C3,
    kCLOCK_GateLPI2C4,
    kCLOCK_GateLPUART0,
    kCLOCK_GateLPUART1,
    kCLOCK_GateLPUART2,
    kCLOCK_GateLPUART3,
    kCLOCK_GateLPUART4,
    kCLOCK_GateLPUART5,
    kCLOCK_GateOSTIMER0,
    kCLOCK_GateLPSPI0,
    kCLOCK_GateLPSPI1,
    kCLOCK_GateLPSPI2,
    kCLOCK_GateLPSPI3,
    kCLOCK_GateLPSPI4,
    kCLOCK_GateLPSPI5,
    kCLOCK_GatePORT0,
    kCLOCK_GatePORT1,
    kCLOCK_GatePORT2,
    kCLOCK_GatePORT3,
    kCLOCK_GatePORT4,
    kCLOCK_GatePORT5,
    kCLOCK_GateADC0,
    kCLOCK_GateADC1,
    kCLOCK_GateCMP0,
    kCLOCK_GateDAC0,
    kCLOCK_GateDAC1,
    kCLOCK_GateVREF0,
    kCLOCK_GateTSI0,
    kCLOCK_GateI3C0,
    kCLOCK_GateI3C1,
    kCLOCK_GateI3C2,
    kCLOCK_GateI3C3,
    kCLOCK_GateFLEXCAN0,
    kCLOCK_GateFLEXCAN1,
    kCLOCK_GateE1588,
    kCLOCK_GateRMII,
    kCLOCK_GateENET0,
    kCLOCK_GateTENBASET_PHY0,
    kCLOCK_GateFLEXSPI0,
    kCLOCK_GateSPIFILTER0,
    kCLOCK_GateESPI0,
    kCLOCK_GateUSBHS,
    kCLOCK_GateUSBHS_PHY,
    kCLOCK_GateEWM0,
    kCLOCK_GateRAMA,
    kCLOCK_GateRAMB,
    kCLOCK_GateGPIO0,
    kCLOCK_GateGPIO1,
    kCLOCK_GateGPIO2,
    kCLOCK_GateGPIO3,
    kCLOCK_GateGPIO4,
    kCLOCK_GateGPIO5,
    kCLOCK_GateROMC,
    kCLOCK_GateSMARTDMA0,
    kCLOCK_GateSECCON,
    kCLOCK_GateGLIKEY0,
    kCLOCK_GateTDET0,
    kCLOCK_GatePKC0,
    kCLOCK_GateSGI0,
    kCLOCK_GateTRNG0,
    kCLOCK_GateUDF0,
    kCLOCK_GateDGDET0,
    kCLOCK_GateITRC0,
};

static void SetClockModulePowerStatus(resc_status_t resc_status, resc_name_t resc_name)
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
                SCG0->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;
                SCG0->FIRCCSR |= SCG_FIRCCSR_LK_MASK;
            }
            else if(resc_status == kResc_Status_Lp)
            {
                if ((SCG0->FIRCCSR & SCG_FIRCCSR_LK_MASK) == SCG_FIRCCSR_LK_MASK)
                {
                    SCG0->FIRCCSR &= ~SCG_FIRCCSR_LK_MASK;
                }

                SCG0->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;
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
                
                SCG0->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;
                SCG0->FIRCCSR |= SCG_FIRCCSR_FIRC_FCLK_PERIPH_EN_MASK;
                SCG0->FIRCCSR |= SCG_FIRCCSR_FIRC_SCLK_PERIPH_EN_MASK;

                while ((SCG0->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK) == 0U)
                {
                }

                SCG0->FIRCCSR |= SCG_FIRCCSR_LK_MASK;
            }
            break;
            
        case kResc_Fro_12M:
            if (resc_status == kResc_Status_Off || resc_status == kResc_Status_Lp)
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
                 /*! The application should modify the configuration here according to actual needs. */
                 VBAT_GateFRO16k(APP_VBAT, kVBAT_EnableClockToDomain0);
                 VBAT_GateFRO16k(APP_VBAT, kVBAT_EnableClockToDomain1);
                 VBAT_GateFRO16k(APP_VBAT, kVBAT_EnableClockToDomain2);
            }
            else if (resc_status == kResc_Status_Lp)
            {
                 VBAT_EnableFRO16k(APP_VBAT, true);
                 VBAT_GateFRO16k(APP_VBAT, kVBAT_EnableClockToDomain0);
                 VBAT_GateFRO16k(APP_VBAT, kVBAT_EnableClockToDomain1);
                 VBAT_GateFRO16k(APP_VBAT, kVBAT_EnableClockToDomain2);
            }
            else
            {
                 VBAT_EnableFRO16k(APP_VBAT, true);
                 VBAT_UngateFRO16k(APP_VBAT, kVBAT_EnableClockToDomain0);
                 VBAT_UngateFRO16k(APP_VBAT, kVBAT_EnableClockToDomain1);
                 VBAT_UngateFRO16k(APP_VBAT, kVBAT_EnableClockToDomain2);
            }
            break;

        case kResc_Osc_Rtc:
            if (resc_status == kResc_Status_Off)
            {
                 VBAT_EnableCrystalOsc32k(APP_VBAT, false);
            }
            else if (resc_status == kResc_Status_Lp)
            {
                 osc_32k_config_t config;
                 CLOCK_GetDefaultOsc32KConfig(&config);
                 config.mode = kVBAT_OscLowpowerModeEnable;
                 config.updateTrim = false; 
                 CLOCK_SetupOsc32KClockingConfig(config);
                 VBAT_GateOsc32k(APP_VBAT, kVBAT_EnableClockToDomain0 | kVBAT_EnableClockToDomain1 | kVBAT_EnableClockToDomain2);
            }
            else
            {
                 osc_32k_config_t config;
                 CLOCK_GetDefaultOsc32KConfig(&config);
                 config.mode = kVBAT_OscLowpowerModeEnable;
                 config.updateTrim = false; 
                 config.id = (osc32k_clk_gate_id_t) (kCLOCK_Osc32kToVbat | kCLOCK_Osc32kToCore);
                 CLOCK_SetupOsc32KClockingConfig(config);
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
                SCG0->SPLLCTRL = 0U;
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

static void SetVoltageModulePowerStatus(resc_status_t resc_status, resc_name_t resc_name)
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

        case kResc_ScgLdo:
            if (resc_status == kResc_Status_Lp)
            {
                SCG0->LDOCSR = SCG_LDOCSR_VOUT_SEL(APP_SCG_LDO_VOUT_SEL_1V1);
            }
            else if (resc_status == kResc_Status_Off)
            {
                SCG0->LDOCSR &= ~SCG_LDOCSR_LDOEN_MASK;
            }
            else
            {
                /* Keep the boot/default SCG LDO configuration. */
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
                /*! Disable SRAM LDO, use SoC to supply SRAM. */
                VBAT_SwitchSRAMPowerBySocSupply(APP_VBAT);
            }
            else
            {
                /*! FRO16K must be enabled before enabling the Bandgap: set kResc_Fro_16K to kResc_Status_On. */
                VBAT_EnableBandgap(APP_VBAT, true);
                VBAT_EnableBandgapRefreshMode(APP_VBAT, true);
                if (kStatus_Success != VBAT_EnableBackupSRAMRegulator(APP_VBAT, true))
                {
                    assert(false);
                }
                /*! Enable SRAM LDO, use SRAMLDO to supply SRAM. */
                VBAT_SwitchSRAMPowerByLDOSRAM(APP_VBAT);
            }
            break;

        case kResc_VbatBandgap:
            (void)VBAT_EnableBandgap(APP_VBAT, (resc_status != kResc_Status_Off));
            break;

        case kResc_VbatBandgapRefresh:
            VBAT_EnableBandgapRefreshMode(APP_VBAT, (resc_status != kResc_Status_Off));
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
            
        case kResc_Glitch_Detector_Act:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableActiveModeVddCoreGlitchDetect(APP_SPC, true);
            }
            else
            {
                SPC_DisableActiveModeVddCoreGlitchDetect(APP_SPC, false);
            }
            break;
            
        case kResc_Glitch_Detector_Lp:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableLowPowerModeVddCoreGlitchDetect(APP_SPC, true);
            }
            else
            {
                SPC_DisableLowPowerModeVddCoreGlitchDetect(APP_SPC, false);
            }
            break;

        case kResc_SpcSocCtrl_Act:
            APP_SPC->ACTIVE_CFG1 = SPC_ACTIVE_CFG1_SOC_CNTRL(
                (resc_status == kResc_Status_Off) ? 0U : APP_SPC_SOC_CNTRL_WAKE_TIMER_CLOCK);
            break;

        case kResc_SpcSocCtrl_Lp:
            APP_SPC->LP_CFG1 = SPC_LP_CFG1_SOC_CNTRL(
                (resc_status == kResc_Status_Off) ? 0U : APP_SPC_SOC_CNTRL_WAKE_TIMER_CLOCK);
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

    if (resc_name == kResc_Flash)
    {
        if (resc_status == kResc_Status_Off)
        {
            /* Flash in low-power state, no wake up */
            CMC_ConfigFlashMode(APP_CMC, false, true, true);
        }
        else if (resc_status == kResc_Status_Lp)
        {
            /* Flash in low-power state, wake on access. */
            CMC_ConfigFlashMode(APP_CMC, true, true, true);
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
        else if (resc_status == kResc_Status_Lp)
        {
            SYSCON->LPCAC_CTRL = SYSCON_LPCAC_CTRL_DIS_LPCAC_WTBF(1U) |
                                 SYSCON_LPCAC_CTRL_LIM_LPCAC_WTBF(1U);
        }
        else
        {
            /* Enable LPCAC. */
            SYSCON->LPCAC_CTRL &= ~SYSCON_LPCAC_CTRL_DIS_LPCAC_MASK;
        }
    }
    else if (resc_name == kResc_SysconRamCtrl)
    {
        if (resc_status == kResc_Status_Lp)
        {
            SYSCON->RAM_CTRL = SYSCON_RAM_CTRL_RAMA_ECC_ENABLE(1U);
        }
        else if (resc_status == kResc_Status_Off)
        {
            SYSCON->RAM_CTRL = 0U;
        }
        else
        {
            /* Keep the current RAM_CTRL configuration. */
        }
    }
    else if (resc_name >= kResc_RamX)
    {
        sramMask = (1UL << (uint32_t)sram_map[resc_name]);        

        if (resc_status == kResc_Status_Off)
        {
            /* This API will clear SRAMRET0 first, so need to or SRAMRET0 */
            CMC_PowerOffSRAMLowPowerOnly(APP_CMC, sramMask | CMC->SRAMRET[0]);
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
        sramMask = (1UL << (uint32_t)sram_ret_map[resc_name]);

        if (resc_status == kResc_Status_Off)
        {
            VBAT_PowerOffSRAMsInLowPowerModes(APP_VBAT, (uint8_t)sramMask);
        }
        else
        {
            VBAT_RetainSRAMsInLowPowerModes(APP_VBAT, (uint8_t)sramMask);
        }
    }
}

static void SetAnalogModulePowerStatus(resc_status_t resc_status, resc_name_t resc_name)
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

        case kResc_Vbat_Lp:
            if (resc_status == kResc_Status_Off)
            {
                SPC_DisableLowPowerModeAnalogModules(APP_SPC, kSPC_controlVbat);
            }
            else
            {
                SPC_EnableLowPowerModeAnalogModules(APP_SPC, kSPC_controlVbat);
            }
            break;

        case kResc_VbatDpdPower:
            if (resc_status == kResc_Status_Off)
            {
                APP_VBAT->FROCTLA = 0U;
                APP_VBAT->FROCLKE = 0U;
                APP_VBAT->LDORAMC = 0U;
                APP_VBAT->OSCCTLA = 0U;
                APP_VBAT->OSCCLKE = 0U;
                APP_VBAT->MONCTLA = 0U;
            }
            break;

        case kResc_Clkmon:
            if (resc_status == kResc_Status_Off)
            {
                VBAT0->MONCTLA &= ~VBAT_MONCTLA_MON_EN_MASK;
            }
            else
            {
                VBAT0->MONCTLA |= VBAT_MONCTLA_MON_EN_MASK;
            }
            break;

        default:
        {
            assert(false);
            break;
        }
    }
}

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

        if (resc == kResc_Fro_16K)
        {
            continue;
        }

        if (resc <= kResc_Spll)
        {
            SetClockModulePowerStatus(status, resc);
        }
        else if (resc <= kResc_SpcSocCtrl_Lp)
        {
            SetVoltageModulePowerStatus(status, resc);
        }
        else if (resc <= kResc_Ram_Usb_FlexSpi)
        {
            SetMemoryPowerStatus(status, resc);
        }
        else if (resc <= kResc_Clkmon)
        {
            SetAnalogModulePowerStatus(status, resc);
        }
        else
        {
            SetPeripheralClockStatus(status, resc);
        }
    }
}

static void ApplySpcSocCtrlForMode(uint8_t modeIndex)
{
    SetVoltageModulePowerStatus(g_resc_ctrl_table[kResc_SpcSocCtrl_Act][modeIndex], kResc_SpcSocCtrl_Act);
    SetVoltageModulePowerStatus(g_resc_ctrl_table[kResc_SpcSocCtrl_Lp][modeIndex], kResc_SpcSocCtrl_Lp);
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

    if (ctrl->updateActiveModeConfig)
    {
        spc_active_mode_core_ldo_option_t activeOption;

        activeOption.CoreLDOVoltage       = ctrl->activeCoreLDOVoltage;
        activeOption.CoreLDODriveStrength = ctrl->activeCoreLDODriveStrength;

        (void)SPC_SetActiveModeBandgapModeConfig(APP_SPC, ctrl->activeBandgapMode);
        (void)SPC_SetActiveModeCoreLDORegulatorConfig(APP_SPC, &activeOption);
    }

    if (ctrl->useActiveModeConfig)
    {
        spc_active_mode_core_ldo_option_t option;

        option.CoreLDOVoltage = ctrl->coreLDOVoltage;
        option.CoreLDODriveStrength = ctrl->coreLDODriveStrength;

        SPC_SetActiveModeBandgapModeConfig(APP_SPC, ctrl->bandgapMode);

        (void)SPC_SetActiveModeCoreLDORegulatorConfig(APP_SPC, &option);
    }
    else
    {
        spc_lowpower_mode_core_ldo_option_t option;

        option.CoreLDOVoltage       = ctrl->coreLDOVoltage;
        option.CoreLDODriveStrength = ctrl->coreLDODriveStrength;

        if (ctrl->coreLDODriveStrength == kSPC_CoreLDO_LowDriveStrength)
        {
            (void)SPC_SetLowPowerModeBandgapmodeConfig(APP_SPC, kSPC_BandgapEnabledBufferDisabled);
            (void)SPC_SetLowPowerModeCoreLDORegulatorDriveStrength(APP_SPC, kSPC_CoreLDO_NormalDriveStrength);
            (void)SPC_SetLowPowerModeCoreLDORegulatorVoltageLevel(APP_SPC, ctrl->coreLDOVoltage);
            (void)SPC_SetLowPowerModeCoreLDORegulatorDriveStrength(APP_SPC, kSPC_CoreLDO_LowDriveStrength);
            (void)SPC_SetLowPowerModeBandgapmodeConfig(APP_SPC, ctrl->bandgapMode);
        }
        else
        {
            (void)SPC_SetLowPowerModeBandgapmodeConfig(APP_SPC, ctrl->bandgapMode);
            (void)SPC_SetLowPowerModeCoreLDORegulatorConfig(APP_SPC, &option);
        }
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
        case kAPP_PowerModeDeepSleep:
            return &g_cpu_clock_cfg_table[2];
        case kAPP_PowerModePowerDown:
            return &g_cpu_clock_cfg_table[3];
        case kAPP_PowerModeDeepPowerDown:
            return &g_cpu_clock_cfg_table[4];
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
    else if ((cfg->source == kAPP_CpuClockSrcFroHf) && (cfg->freqHz == BOARD_BOOTCLOCKFROHF48M_CORE_CLOCK))
    {
        BOARD_BootClockFROHF48M();
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

static bool IsWakeTimerRunning(void)
{
    return (APP_WUU_WAKEUP_TIMER->CSR & LPTMR_CSR_TEN_MASK) != 0UL;
}

static void KeepWakeTimerClock(void)
{
    CLOCK_SetClockDiv(kCLOCK_DivLPTMR0, 1U);
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPTMR0);
}

static void KeepFro16kForWakeTimer(void)
{
    uint32_t clockMask = kVBAT_EnableClockToVddWake;

    if (IsWakeTimerRunning())
    {
        clockMask |= kVBAT_EnableClockToVddBat;
    }

    VBAT_EnableFRO16k(APP_VBAT, true);
    VBAT_UngateFRO16k(APP_VBAT, clockMask);
    VBAT_GateFRO16k(APP_VBAT, (kVBAT_EnableClockToVddBat | kVBAT_EnableClockToVddSys |
                               kVBAT_EnableClockToVddWake | kVBAT_EnableClockToVddMain) &
                                  ~clockMask);
}

static bool APP_WaitPowerDownWakeSpcNotBusy(void)
{
    uint32_t timeout = APP_POWERDOWN_WAKE_RESTORE_TIMEOUT;

    while ((APP_SPC->SC & SPC_SC_BUSY_MASK) != 0UL)
    {
        if (timeout == 0U)
        {
            return false;
        }
        timeout--;
    }

    return true;
}

static bool APP_SetPowerDownWakeSramOperateVoltage(spc_sram_operate_voltage_t operateVoltage)
{
    uint32_t timeout = APP_POWERDOWN_WAKE_RESTORE_TIMEOUT;

    APP_SPC->SRAMCTL = SPC_SRAMCTL_VSM(operateVoltage);
    APP_SPC->SRAMCTL |= SPC_SRAMCTL_REQ_MASK;

    while ((APP_SPC->SRAMCTL & SPC_SRAMCTL_ACK_MASK) == 0UL)
    {
        if (timeout == 0U)
        {
            APP_SPC->SRAMCTL &= ~SPC_SRAMCTL_REQ_MASK;
            return false;
        }
        timeout--;
    }

    APP_SPC->SRAMCTL &= ~SPC_SRAMCTL_REQ_MASK;

    return true;
}

static bool APP_PreparePowerDownWakeActiveVoltage(void)
{
    uint32_t reg;

    if (kStatus_Success != SPC_SetActiveModeBandgapModeConfig(APP_SPC, kSPC_BandgapEnabledBufferDisabled))
    {
        return false;
    }

    reg = APP_SPC->ACTIVE_CFG;
    reg &= ~SPC_ACTIVE_CFG_CORELDO_VDD_DS_MASK;
    reg |= SPC_ACTIVE_CFG_CORELDO_VDD_DS(kSPC_CoreLDO_NormalDriveStrength);
    APP_SPC->ACTIVE_CFG = reg;
    if (!APP_WaitPowerDownWakeSpcNotBusy())
    {
        return false;
    }

    reg = APP_SPC->ACTIVE_CFG;
    reg &= ~SPC_ACTIVE_CFG_CORELDO_VDD_LVL_MASK;
    reg |= SPC_ACTIVE_CFG_CORELDO_VDD_LVL(kSPC_CoreLDO_OverDriveVoltage);
    APP_SPC->ACTIVE_CFG = reg;
    if (!APP_WaitPowerDownWakeSpcNotBusy())
    {
        return false;
    }

    CLOCK_SetFLASHAccessCyclesForFreq(BOARD_BOOTCLOCKPLL240M_CORE_CLOCK, kOD_Mode);

    if (!APP_SetPowerDownWakeSramOperateVoltage(kSPC_sramOperateAt1P2V))
    {
        return false;
    }

    return true;
}

static bool APP_RestoreActiveClockAfterPowerDownWake(void)
{
    const app_cpu_clock_cfg_t *powerDownClockCfg = GetCpuClockCfg(kAPP_PowerModePowerDown);

    ApplyCpuClockCfg(powerDownClockCfg);
    __DSB();
    __ISB();

    if (!APP_PreparePowerDownWakeActiveVoltage())
    {
        return false;
    }

    BOARD_BootClockPLL240M_InitClockModule(kClockModule_SOSC);
    BOARD_BootClockPLL240M_InitClockModule(kClockModule_SIRC);
    BOARD_BootClockPLL240M_InitClockModule(kClockModule_FIRC);
    BOARD_BootClockPLL240M_InitClockModule(kClockModule_VBAT);
    BOARD_BootClockPLL240M_InitClockModule(kClockModule_PLL);
    BOARD_BootClockPLL240M_InitClockModule(kClockModule_SystemClk);

    __DSB();
    __ISB();

    SystemCoreClock = BOARD_BOOTCLOCKPLL240M_CORE_CLOCK;

    return true;
}

void APP_PowerPreSwitchHook(app_power_mode_t targetPowerMode)
{
    uint8_t modeIndex = 0U;
    bool modeIndexValid = GetLowPowerModeIndex(targetPowerMode, &modeIndex);
    bool wakeTimerRunning = IsWakeTimerRunning();

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
    APP_DEBUG_CONSOLE_RX_PORT->PCR[APP_DEBUG_CONSOLE_RX_PIN] &= ~PORT_PCR_IBE_MASK;
    APP_DEBUG_CONSOLE_TX_PORT->PCR[APP_DEBUG_CONSOLE_TX_PIN] &= ~PORT_PCR_IBE_MASK;

    if ((targetPowerMode != kAPP_PowerModeDeepPowerDown) || wakeTimerRunning)
    {
        KeepFro16kForWakeTimer();
    }
    ApplyCpuClockCfg(GetCpuClockCfg(targetPowerMode));
    if (wakeTimerRunning)
    {
        KeepWakeTimerClock();
    }
    ApplyRescTableForMode(modeIndex);
    SetRegulatorsConfig(targetPowerMode);
    ApplySpcSocCtrlForMode(modeIndex);

    /* Keep SPC wakeup delay in sync for low power entry. */
    SPC_SetLowPowerWakeUpDelay(APP_SPC, APP_LDO_LPWKUP_DELAY);

    if ((targetPowerMode != kAPP_PowerModeDeepPowerDown) || wakeTimerRunning)
    {
        KeepFro16kForWakeTimer();
    }
    if (wakeTimerRunning)
    {
        KeepWakeTimerClock();
    }
}

void APP_PowerPostSwitchHook(app_power_mode_t targetPowerMode)
{
    /*
     * After waking from DeepSleep/PowerDown/DeepPowerDown, clear the SPC
     * external-voltage-domain isolation state that was configured before entry.
     *
     * SPC lives in the SYSTEM power domain and is not reset by a warm reset
     * (including debugger-triggered reset when downloading the next image).
     * If the LPISO/ISO bits and the PERIPH_IO_ISO latch are left set, the
     * next application will see VDD_USB / VDD_P2 / VDD_P3 / VDD_P4 held in
     * the isolated state.
     */
    SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, kSPC_PowerDomain0);
    SPC_ClearLowPowerRequest(APP_SPC);

    if (kCMC_CoreClockGated == CMC_GetCoreClockGatedStatus(APP_CMC))
    {
        CMC_ClearCoreClockGatedStatus(APP_CMC);
    }

    SPC_ClearPeriphIOIsolationFlag(APP_SPC);
    SPC_SetExternalVoltageDomainsConfig(APP_SPC, 0x0U, 0x0U);

    if (targetPowerMode == kAPP_PowerModePowerDown)
    {
        (void)APP_RestoreActiveClockAfterPowerDownWake();
    }
    else
    {
        BOARD_InitBootClocks();
    }

    BOARD_InitBUTTONsPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
}
