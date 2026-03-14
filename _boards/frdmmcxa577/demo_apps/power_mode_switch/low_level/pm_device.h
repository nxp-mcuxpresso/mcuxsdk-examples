/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PM_DEVICE_H_
#define PM_DEVICE_H_

#include "app.h"
#include "board.h"
#include "fsl_cmc.h"
#include "fsl_spc.h"
#include "fsl_vbat.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "fsl_lpuart.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "power_mode_switch.h"

typedef enum _resc_status
{
    kResc_Status_On = 0U,
    kResc_Status_Lp,
    kResc_Status_Off,
} resc_status_t;

typedef enum _resc_name
{
    /*! CLock modules */
    kResc_Fro_192M,
    kResc_Fro_12M,
    kResc_Fro_16K,
    kResc_Osc_Rtc,
    kResc_Osc_Sys,
    kResc_Spll,

    /*! Power domain modules */
    kResc_LdoCore,
    kResc_RamRetentionLdo,
    kResc_LpIref,
    kResc_LpBuffer_Act,
    kResc_LpBuffer_Lp,

    /* When LVD/HVD is enabled, BGMODE should kept enabled. */
    kResc_Core_Vdd_Lvd_Act,
    kResc_Core_Vdd_Hvd_Act,
    kResc_Sys_Vdd_Lvd_Act,
    kResc_Sys_Vdd_Hvd_Act,
    kResc_Core_Vdd_Lvd_Lp,
    kResc_Core_Vdd_Hvd_Lp,
    kResc_Sys_Vdd_Lvd_Lp,
    kResc_Sys_Vdd_Hvd_Lp,
    kResc_GlitchDetector_Act,
    kResc_GlitchDetector_Lp,

    /*! Memory modules */
    kResc_Flash,
    kResc_Lpcac,
    kResc_RamA0,
    kResc_RamA3,
    kResc_RamX,
    kResc_RamA,
    kResc_RamB,

    /*! Analog modules */
    kResc_Vref,
    kResc_Usb,
    kResc_Vbat,
    kResc_Dac0,
    kResc_Dac1,
    kResc_Tsi0,
    kResc_Cmp0,
    kResc_Cmp0_Dac,

    /*! Peripheral clocks, This is for demonstration purposes only.
     * In actual projects, the peripheral clock can be controlled
     * according to specific needs.
     */
    kResc_RamA_Clk,
    kResc_RamB_Clk,
    kResc_RamX_Clk,
    kResc_Peri_clk_all,

    kResc_Max_Num,
} resc_name_t;

typedef struct _app_core_ldo_ctrl
{
    bool valid;
    bool useActiveModeConfig;   /* true: SPC_SetActiveModeRegulatorsConfig, false: SPC_SetLowPowerModeRegulatorsConfig */
    bool lpIREF;                /* Only used for low power modes. */
    bool lpBuff;
    spc_bandgap_mode_t bandgapMode;
    spc_core_ldo_voltage_level_t coreLDOVoltage;
    spc_core_ldo_drive_strength_t coreLDODriveStrength;
} app_core_ldo_ctrl_t;

typedef enum _app_cpu_clock_source
{
    kAPP_CpuClockSrcNone = 0,
    kAPP_CpuClockSrcFro12M,
    kAPP_CpuClockSrcFroHf,
    kAPP_CpuClockSrcPll,
} app_cpu_clock_source_t;

typedef struct _app_cpu_clock_cfg
{
    bool valid;
    app_cpu_clock_source_t source;
    uint32_t freqHz;
} app_cpu_clock_cfg_t;

#endif /*PM_DEVICE_H_*/
