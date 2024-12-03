/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
*/
#include "nt_setup.h"
#include "board.h"

/* ************************** */
/* Crosstalk reduction Matrix */
/* ************************** */
int16_t actMat[] = {
 32767,     0,     0,     0,     0,     0,
  7836, 32767, 13712, 13159,  9339,  1758,
  3760, 16178, 32767, 10470, 16523,  2151,
  6452, 18958, 13281, 32767, 13860,  1214,
   724, 12995, 16586, 11000, 32767,   368,
    17,    13,    23,    19,    23, 32767};

/* ************************* */
/* Keydetector Configuration */
/* ************************* */
#define DEFAULTMBW \
    .signal_filter.coef1 = 0, \
    .base_avrg.n2_order = 12, \
    .non_activity_avrg.n2_order = 15, \
    .entry_event_cnt = 0, \
    .deadband_cnt = 0, \
    .baseline_track_window = 4000, \
    .baseline_track_window_touch = 16000, \
    .touch_limit = 3, \
    .tau_smooth_baseline = 5000, \
    .debounce_length = 5, \
    .baseline_debounce_length = 20 \

const struct nt_keydetector_mbw nt_keydetector_mbw_El_1 = {
    DEFAULTMBW,
    .tau_smooth_signal = 0,
    .min_noise_limit = 7,
    .signal_to_noise_ratio = 114
};

const struct nt_keydetector_mbw nt_keydetector_mbw_El_2 = {
    DEFAULTMBW,
    .tau_smooth_signal = 0,
    .min_noise_limit = 9,
    .signal_to_noise_ratio = 89
};

const struct nt_keydetector_mbw nt_keydetector_mbw_El_3 = {
    DEFAULTMBW,
    .tau_smooth_signal = 42,
    .min_noise_limit = 160,
    .signal_to_noise_ratio = 5
};
const struct nt_keydetector_mbw nt_keydetector_mbw_El_4 = {
    DEFAULTMBW,
    .tau_smooth_signal = 69,
    .min_noise_limit = 160,
    .signal_to_noise_ratio = 5
};
const struct nt_keydetector_mbw nt_keydetector_mbw_El_5 = {
    DEFAULTMBW,
    .tau_smooth_signal = 42,
    .min_noise_limit = 200,
    .signal_to_noise_ratio = 4
};
const struct nt_keydetector_mbw nt_keydetector_mbw_El_6 = {
    DEFAULTMBW,
    .tau_smooth_signal = 65,
    .min_noise_limit = 160,
    .signal_to_noise_ratio = 5
};

const struct nt_keydetector_mbw nt_keydetector_mbw_El_7 = {
    .signal_filter.coef1        = 0,
    .base_avrg.n2_order         = 14,
    .non_activity_avrg.n2_order = 15,
    .entry_event_cnt            = 4,
    .deadband_cnt               = 4,
    .signal_to_noise_ratio      = 3,
    .min_noise_limit            = 60,
    .baseline_track_window      = 2000,
    .baseline_track_window_touch= 8000,
    .touch_limit                = 0,      
    .tau_smooth_signal          = 10,
    .tau_smooth_baseline        = 5000,
    .debounce_length            = 5,
};
/* ********************** */
/* Hardware Configuration */
/* ********************** */
#define DEFAULTHWCONFIG \
    .configSelfCap.commonConfig.mainClock     = kTSI_MainClockSlection_0, \
    .configSelfCap.commonConfig.ssc_prescaler = kTSI_ssc_div_by_1, \
    .configSelfCap.commonConfig.ssc_mode      = kTSI_ssc_prbs_method, \
    .configSelfCap.commonConfig.noChargeNum   = kTSI_SscNoChargeNumValue_1, \
    .configSelfCap.commonConfig.prbsOutsel    = kTSI_SscPrbsOutsel_2, \
    .configSelfCap.commonConfig.chargeNum     = kTSI_SscChargeNumValue_3, \
    .configSelfCap.commonConfig.mode          = kTSI_SensingModeSlection_Self, \
    .configSelfCap.commonConfig.dvolt         = kTSI_DvoltOption_3, \
    .configSelfCap.commonConfig.cutoff        = kTSI_SincCutoffDiv_1, \
    .configSelfCap.commonConfig.order         = kTSI_SincFilterOrder_1, \
    .configSelfCap.enableSensitivity          = false, \
    .configSelfCap.inputCurrent               = kTSI_CurrentMultipleInputValue_0, \
    .configSelfCap.xdn                        = kTSI_SensitivityXdnOption_0, \
    .configSelfCap.chargeCurrent              = kTSI_CurrentMultipleChargeValue_0, \
    .configMutual.commonConfig.mainClock      = kTSI_MainClockSlection_0, \
    .configMutual.commonConfig.ssc_prescaler  = kTSI_ssc_div_by_1, \
    .configMutual.commonConfig.ssc_mode       = kTSI_ssc_prbs_method, \
    .configMutual.commonConfig.noChargeNum    = kTSI_SscNoChargeNumValue_1, \
    .configMutual.commonConfig.prbsOutsel     = kTSI_SscPrbsOutsel_2, \
    .configMutual.commonConfig.chargeNum      = kTSI_SscChargeNumValue_3, \
    .configMutual.commonConfig.mode           = kTSI_SensingModeSlection_Mutual, \
    .configMutual.commonConfig.dvolt          = kTSI_DvoltOption_3, \
    .configMutual.commonConfig.cutoff         = kTSI_SincCutoffDiv_1, \
    .configMutual.commonConfig.order          = kTSI_SincFilterOrder_1, \
    .configMutual.preCurrent                  = kTSI_MutualPreCurrent_4uA, \
    .configMutual.preResistor                 = kTSI_MutualPreResistor_4k, \
    .configMutual.senseResistor               = kTSI_MutualSenseResistor_10k, \
    .configMutual.boostCurrent                = kTSI_MutualSenseBoostCurrent_0uA, \
    .configMutual.txDriveMode                 = kTSI_MutualTxDriveModeOption_0, \
    .configMutual.pmosLeftCurrent             = kTSI_MutualPmosCurrentMirrorLeft_32, \
    .configMutual.pmosRightCurrent            = kTSI_MutualPmosCurrentMirrorRight_1, \
    .configMutual.enableNmosMirror            = true, \
    .configMutual.nmosCurrent                 = kTSI_MutualNmosCurrentMirror_1, \
    .newCalc = true

const tsi_config_t tsi_hw_config_El_1 = {
    DEFAULTHWCONFIG,
    .configSelfCap.commonConfig.decimation = kTSI_SincDecimationValue_1, \
    .configSelfCap.enableShield = kTSI_shieldAllOff \
};
const tsi_config_t tsi_hw_config_El_2 = {
    DEFAULTHWCONFIG,
    .configSelfCap.commonConfig.decimation = kTSI_SincDecimationValue_1, \
    .configSelfCap.enableShield = kTSI_shieldAllOff \
};
const tsi_config_t tsi_hw_config_El_3 = {
    DEFAULTHWCONFIG,
    .configMutual.commonConfig.decimation = kTSI_SincDecimationValue_1, \
    .configSelfCap.enableShield = kTSI_shieldAllOff \
};
const tsi_config_t tsi_hw_config_El_4 = {
    DEFAULTHWCONFIG,
    .configMutual.commonConfig.decimation = kTSI_SincDecimationValue_1, \
    .configSelfCap.enableShield = kTSI_shieldAllOff \
};
const tsi_config_t tsi_hw_config_El_5 = {
    DEFAULTHWCONFIG,
    .configMutual.commonConfig.decimation = kTSI_SincDecimationValue_1, \
    .configSelfCap.enableShield = kTSI_shieldAllOff \
};
const tsi_config_t tsi_hw_config_El_6 = {
    DEFAULTHWCONFIG,
    .configMutual.commonConfig.decimation = kTSI_SincDecimationValue_1, \
    .configSelfCap.enableShield = kTSI_shieldAllOff \
};

/* *********************** */
/* Electrode Configuration */
/* *********************** */
const struct nt_electrode El_1 = {
    .delta_limit = 20000,
    .multiplier = 11,
    .divider = 69,
    .offset = 7258,
    .delta_multiplier = 19,
    .delta_divider = 66,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_1,
    .keydetector_interface= &nt_keydetector_mbw_interface,
    .pin_input = FRDM_TOUCH_BOARD_TSI_1,
    .gpio_input = {GPIO_PTD16},   
    .tsicnt_shift = 3,
    .tsi_hw_config = ( void* )&tsi_hw_config_El_1,
    .shield_mask = 1U<<FRDM_TOUCH_BOARD_TSI_2,
};
const struct nt_electrode El_2 = {
    .delta_limit = 20000,
    .multiplier = 12,
    .divider = 68,
    .offset = 8144,
    .delta_multiplier = 19,
    .delta_divider = 66,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_2,
    .keydetector_interface= &nt_keydetector_mbw_interface,
    .pin_input = FRDM_TOUCH_BOARD_TSI_2,
    .gpio_input = {GPIO_PTA1},
    .tsicnt_shift = 3,
    .tsi_hw_config = ( void* )&tsi_hw_config_El_2,
    .shield_mask = 1U<<FRDM_TOUCH_BOARD_TSI_1,
};
const struct nt_electrode El_3 = {
    .delta_limit = 3000,
    .multiplier = 112,
    .divider = 1,
    .offset = 507,
    .delta_multiplier = 19,
    .delta_divider = 66,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_3,
    .keydetector_interface= &nt_keydetector_mbw_interface,
    .pin_input = FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_1,
    .gpio_input = {GPIO_PTE11, GPIO_PTA10},        
    .tsicnt_shift = 0,
    .tsi_hw_config = ( void* )&tsi_hw_config_El_3,
};
const struct nt_electrode El_4 = {
    .delta_limit = 3000,
    .multiplier = 144,
    .divider = 1,
    .offset = 474,
    .delta_multiplier = 19,
    .delta_divider = 66,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_4,
    .keydetector_interface= &nt_keydetector_mbw_interface,
    .pin_input = FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_2,
    .gpio_input = {GPIO_PTC5, GPIO_PTA10},
    .tsicnt_shift = 0,
    .tsi_hw_config = ( void* )&tsi_hw_config_El_4,
};
const struct nt_electrode El_5 = {
    .delta_limit = 3000,
    .multiplier = 69,
    .divider = 1,
    .offset = 572,
    .delta_multiplier = 19,
    .delta_divider = 66,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_5,
    .keydetector_interface= &nt_keydetector_mbw_interface,
    .pin_input = FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_3,
    .gpio_input = {GPIO_PTE11, GPIO_PTA11},
    .tsicnt_shift = 0,
    .tsi_hw_config = ( void* )&tsi_hw_config_El_5,
};
const struct nt_electrode El_6 = {
    .delta_limit = 3000,
    .multiplier = 128,
    .divider = 1,
    .offset = 489,
    .delta_multiplier = 19,
    .delta_divider = 66,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_6,
    .keydetector_interface= &nt_keydetector_mbw_interface,
    .pin_input = FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_4,
    .gpio_input = {GPIO_PTC5, GPIO_PTA11},     
    .tsicnt_shift = 0,
    .tsi_hw_config = ( void* )&tsi_hw_config_El_6,
};

const struct nt_electrode El_7 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 1500,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .tsicnt_shift           = 6,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_7,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_SLIDER_ELECTRODE_1,
    .gpio_input             = {GPIO_PTE13},
};
const struct nt_electrode El_8 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 1500,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .tsicnt_shift           = 6,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_7,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_SLIDER_ELECTRODE_2,
    .gpio_input             = {GPIO_PTE5},
};
const struct nt_electrode El_9 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 1000,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .tsicnt_shift           = 6,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_7,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_1,
    .gpio_input             = {GPIO_PTE10},
};
const struct nt_electrode El_10 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 1000,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .tsicnt_shift           = 6,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_7,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_2,
    .gpio_input             = {GPIO_PTD1},
};
const struct nt_electrode El_11 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 1000,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .tsicnt_shift           = 6,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_7,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_3,
    .gpio_input             = {GPIO_PTE16},
};
const struct nt_electrode El_12 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 1000,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .tsicnt_shift           = 6,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_7,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_4,
    .gpio_input             = {GPIO_PTE15},
};


const tsi_config_t tsi_hw_config = {
    .configSelfCap.commonConfig.mainClock     = kTSI_MainClockSlection_0,
    .configSelfCap.commonConfig.ssc_mode      = kTSI_ssc_prbs_method,
    .configSelfCap.commonConfig.mode          = kTSI_SensingModeSlection_Self,
    .configSelfCap.commonConfig.dvolt         = kTSI_DvoltOption_0,
    .configSelfCap.commonConfig.cutoff        = kTSI_SincCutoffDiv_1,
    .configSelfCap.commonConfig.order         = kTSI_SincFilterOrder_2,
    .configSelfCap.commonConfig.decimation    = kTSI_SincDecimationValue_6,
    .configSelfCap.commonConfig.chargeNum     = kTSI_SscChargeNumValue_4,
    .configSelfCap.commonConfig.prbsOutsel    = kTSI_SscPrbsOutsel_2,
    .configSelfCap.commonConfig.noChargeNum   = kTSI_SscNoChargeNumValue_2,
    .configSelfCap.commonConfig.ssc_prescaler = kTSI_ssc_div_by_2,
    .configSelfCap.enableSensitivity          = false,
    .configSelfCap.enableShield               = kTSI_shieldAllOff,
    .configSelfCap.xdn                        = kTSI_SensitivityXdnOption_3,
    .configSelfCap.ctrim                      = kTSI_SensitivityCtrimOption_0,
    .configSelfCap.inputCurrent               = kTSI_CurrentMultipleInputValue_0,
    .configSelfCap.chargeCurrent              = kTSI_CurrentMultipleChargeValue_0,
    .configMutual.commonConfig.mainClock      = kTSI_MainClockSlection_0,
    .configMutual.commonConfig.ssc_mode       = kTSI_ssc_prbs_method,
    .configMutual.commonConfig.mode           = kTSI_SensingModeSlection_Mutual,
    .configMutual.commonConfig.dvolt          = kTSI_DvoltOption_0,
    .configMutual.commonConfig.cutoff         = kTSI_SincCutoffDiv_1,
    .configMutual.commonConfig.order          = kTSI_SincFilterOrder_2,
    .configMutual.commonConfig.decimation     = kTSI_SincDecimationValue_6,
    .configMutual.commonConfig.chargeNum      = kTSI_SscChargeNumValue_4,
    .configMutual.commonConfig.noChargeNum    = kTSI_SscNoChargeNumValue_2,
    .configMutual.commonConfig.prbsOutsel     = kTSI_SscPrbsOutsel_2,
    .configMutual.commonConfig.ssc_prescaler  = kTSI_ssc_div_by_2,
    .configMutual.preCurrent                  = kTSI_MutualPreCurrent_4uA,
    .configMutual.preResistor                 = kTSI_MutualPreResistor_4k,
    .configMutual.senseResistor               = kTSI_MutualSenseResistor_10k,
    .configMutual.boostCurrent                = kTSI_MutualSenseBoostCurrent_0uA,
    .configMutual.txDriveMode                 = kTSI_MutualTxDriveModeOption_0,
    .configMutual.pmosLeftCurrent             = kTSI_MutualPmosCurrentMirrorLeft_32,
    .configMutual.pmosRightCurrent            = kTSI_MutualPmosCurrentMirrorRight_1,
    .configMutual.enableNmosMirror            = true,
    .configMutual.nmosCurrent                 = kTSI_MutualNmosCurrentMirror_1,
    .thresl                                   = 0,
    .thresh                                   = 65535,
    .newCalc                                  = true,
};

const struct nt_system_xtalk_params my_xtalk_params = {
    .actMat =(uint16_t*)actMat,
    .nt_xtalk_neighbours = 4,
    .nt_xtalk_adapt_touch_time = 25,
};

const struct nt_electrode *const Keypad_1_controls[]         = {&El_1, &El_2, &El_3, &El_4, &El_5, &El_6, NULL};
const struct nt_electrode *const ASlider_2_controls[]        = {&El_7, &El_8, NULL};
const struct nt_electrode *const ARotary_3_controls[]        = {&El_9, &El_10, &El_11, &El_12, NULL};
const struct nt_control_arotary nt_control_arotary_ARotary_3 = {
    .range = 72,
};
const struct nt_control_aslider nt_control_aslider_ASlider_2 = {
    .range         = 160,
    .insensitivity = 2,
};
const struct nt_control_keypad nt_control_keypad_Keypad_1 = {
    .groups           = NULL,
    .groups_size      = 0,
    .multi_touch      = (uint32_t[]){0x0C, 0x18, 0x30, 0x24, 0x3C, 0x03, 0},
    .multi_touch_size = 6,
};

const struct nt_control Keypad_1 = {
    .electrodes            = &Keypad_1_controls[0],
    .control_params.keypad = &nt_control_keypad_Keypad_1,
    .interface             = &nt_control_keypad_interface,
    .adjacent_electrodes   = kAdj_grounding,    
};
const struct nt_control ASlider_2 = {
    .electrodes             = &ASlider_2_controls[0],
    .control_params.aslider = &nt_control_aslider_ASlider_2,
    .interface              = &nt_control_aslider_interface,
};
const struct nt_control ARotary_3 = {
    .electrodes             = &ARotary_3_controls[0],
    .control_params.arotary = &nt_control_arotary_ARotary_3,
    .interface              = &nt_control_arotary_interface,
};

const struct nt_electrode *const nt_tsi_module_electrodes_1[] = {&El_1, &El_3, &El_4,  &El_5,  &El_6,  &El_7,
                                                                 &El_8, &El_9, &El_10, &El_11, &El_12, NULL};
const struct nt_electrode *const nt_tsi_module_electrodes_2[] = {&El_2, NULL};

const struct nt_electrode *const nt_tsi_module_xtalk_electrodes[] = {&El_3,  &El_4, &El_5,  &El_6, NULL};
const struct nt_module nt_tsi_module_1                       = {
    .interface          = &nt_module_tsi_interface,
    .wtrmark_hi         = 65535,
    .wtrmark_lo         = 0,
    .config             = (void *)&tsi_hw_config,
    .instance           = 0,
    .electrodes         = &nt_tsi_module_electrodes_1[0],
    .elec_random_order  = true,
};
const struct nt_module nt_tsi_module_2                       = {
    .interface          = &nt_module_tsi_interface,
    .wtrmark_hi         = 65535,
    .wtrmark_lo         = 0,
    .config             = (void *)&tsi_hw_config,
    .instance           = 1,
    .electrodes         = &nt_tsi_module_electrodes_2[0],
    .elec_random_order  = true,
};
const struct nt_control *const System_0_controls[] = {&Keypad_1, &ASlider_2, &ARotary_3, NULL};
const struct nt_module *const System_0_modules[]   = {&nt_tsi_module_1, &nt_tsi_module_2, NULL};
const struct nt_system System_0                    = {
    .time_period            = 40,
    .init_time              = 1000,
    .controls               = &System_0_controls[0],
    .modules                = &System_0_modules[0],
    .xtalk_interface        = &nt_system_xtalk_interface,
    .xtalk_params           = (void *)&my_xtalk_params,
    .xtalk_electrodes       = &nt_tsi_module_xtalk_electrodes[0],
};
void nt_enable(void)
{
    nt_electrode_enable(&El_1, 0);   /* TSI1 */
    nt_electrode_enable(&El_2, 0);   /* TSI2 */
    nt_electrode_enable(&El_3, 0);   /* TSI1 */
    nt_electrode_enable(&El_4, 0);   /* TSI1 */
    nt_electrode_enable(&El_5, 0);   /* TSI1 */
    nt_electrode_enable(&El_6, 0);   /* TSI1 */
    nt_electrode_enable(&El_7, 0);   /* TSI1 */
    nt_electrode_enable(&El_8, 0);   /* TSI1 */
    nt_electrode_enable(&El_9, 0);   /* TSI1 */
    nt_electrode_enable(&El_10, 0);  /* TSI1 */
    nt_electrode_enable(&El_11, 0);  /* TSI1 */
    nt_electrode_enable(&El_12, 0);  /* TSI1 */
    nt_control_enable(&Keypad_1);    
    nt_control_enable(&ASlider_2);   
    nt_control_enable(&ARotary_3);   
}

// Following functions are generate, copy them to your application and implement there behaviour of events
/* Call on the TSI CNTR overflow 16-bit range (65535) */
/*void system_0_callback_template(uint32_t event, union nt_system_event_context * context)
{
  switch(event)
  {
    case NT_SYSTEM_EVENT_OVERRUN:
        // your code
    break;
    case NT_SYSTEM_EVENT_DATA_READY:
        // your code
    break;
    case NT_SYSTEM_EVENT_MODULE_DATA_READY:
        // your code
    break;
    case NT_SYSTEM_EVENT_DATA_OVERFLOW:
        // your code
    break;
    case NT_SYSTEM_EVENT_SIGNAL_LOW:
        // your code
    break;
    case NT_SYSTEM_EVENT_SIGNAL_HIGH:
        // your code
    break;
    case NT_SYSTEM_EVENT_ELEC_SHORT_VDD:
        // your code
    break;
    case NT_SYSTEM_EVENT_ELEC_SHORT_GND:
        // your code
    break;
    case NT_SYSTEM_EVENT_ELEC_SHORT_ADJ:
        // your code
    break;
  }
}
*/

/*static void Keypad_1_callback_template(const struct nt_control *control,
                               enum nt_control_keypad_event event,
                               uint32_t index)
 {
   switch(event)
   {
   case NT_KEYPAD_RELEASE:
     switch (index) {
         case 0:
         // your code
     break;
         case 1:
         // your code
     break;
         case 2:
         // your code
     break;
         case 3:
         // your code
     break;
         default:
         // your code
     break;
     }
     break;
   case NT_KEYPAD_TOUCH:
     switch (index) {
         case 0:
         // your code
     break;
         case 1:
         // your code
     break;
         case 2:
         // your code
     break;
         case 3:
         // your code
     break;
         default:
         // your code
     }
     break;
   }
 }
 */

/*static void ASlider_2_callback_template(const struct nt_control *control,
                                enum nt_control_aslider_event event,
                                uint32_t position)
 {
 switch(event)
     {
     case NT_ASLIDER_MOVEMENT:
         // your code
     break;
     case NT_ASLIDER_ALL_RELEASE:
         // your code
     break;
     case NT_ASLIDER_INITIAL_TOUCH:
         // your code
     default:
         // your code
     break;
   }
 }
 */

/*static void ASlider_2_callback_template(const struct nt_control *control,
                                enum nt_control_aslider_event event,
                                uint32_t position)
 {
 switch(event)
     {
     case NT_ASLIDER_MOVEMENT:
         // your code
     break;
     case NT_ASLIDER_ALL_RELEASE:
         // your code
     break;
     case NT_ASLIDER_INITIAL_TOUCH:
         // your code
     default:
         // your code
     break;
   }
 }
 */
