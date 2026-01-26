/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PM_DEVICE_H_
#define PM_DEVICE_H_

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
    
    kResc_Max_Num,
} resc_name_t;

#endif /*PM_DEVICE_H_*/
