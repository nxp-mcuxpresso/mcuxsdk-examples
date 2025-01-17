/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
Static configuration c file for the NXP Touch Library
generated by the NXP Touch GUI Tool
*/
#include "nt_setup.h"
#include "board.h"

int16_t actMat[NT_XTALK_NSENSORS * NT_XTALK_NSENSORS] = {
          32767, 15342, 19509, 12501,
          5383,  32767, 17086, 16383,
          12873, 11702, 32767, 11702 ,
          3692,  16153, 15922, 32767,
};

const struct nt_keydetector_mbw nt_keydetector_mbw_El_1 = {
    .signal_filter.coef1        = 2,
    .base_avrg.n2_order         = 14,
    .non_activity_avrg.n2_order = 15,
    .entry_event_cnt            = 4,
    .deadband_cnt               = 4,
    .signal_to_noise_ratio      = 5,
    .min_noise_limit            = 90,
    .baseline_track_window      = 6000,
    .baseline_track_window_touch= 0,
    .touch_limit                = 0,      
    .tau_smooth_signal          = 100,
    .tau_smooth_baseline        = 15000,
    .debounce_length            = 5,
};
const struct nt_keydetector_mbw nt_keydetector_mbw_El_3 = {
    .signal_filter.coef1        = 1,
    .base_avrg.n2_order         = 12,
    .non_activity_avrg.n2_order = 15,
    .entry_event_cnt            = 4,
    .deadband_cnt               = 5,
    .signal_to_noise_ratio      = 5,
    .min_noise_limit            = 40,
    .baseline_track_window      = 2000,
    .baseline_track_window_touch= 8000,
    .touch_limit                = 3,      
    .tau_smooth_signal          = 10,
    .tau_smooth_baseline        = 5000,
    .debounce_length            = 5,
};
const struct nt_keydetector_mbw nt_keydetector_mbw_El_7 = {
    .signal_filter.coef1        = 2,
    .base_avrg.n2_order         = 14,
    .non_activity_avrg.n2_order = 15,
    .entry_event_cnt            = 4,
    .deadband_cnt               = 4,
    .signal_to_noise_ratio      = 4,
    .min_noise_limit            = 80,
    .baseline_track_window      = 6000,
    .baseline_track_window_touch= 0,
    .touch_limit                = 0,      
    .tau_smooth_signal          = 100,
    .tau_smooth_baseline        = 15000,
    .debounce_length            = 5,
};
const struct nt_electrode El_1 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 1500,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .tsicnt_shift           = 5,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_1,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_SLIDER_ELECTRODE_1,
};
const struct nt_electrode El_2 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 1500,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .tsicnt_shift           = 5,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_1,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_SLIDER_ELECTRODE_2,
};
const struct nt_electrode El_3 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 500,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_3,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_1,
};
const struct nt_electrode El_4 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 500,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_3,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_2,
};
const struct nt_electrode El_5 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 500,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_3,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_3,
};
const struct nt_electrode El_6 = {
    .shielding_electrode    = NULL,
    .multiplier             = 0,
    .divider                = 0,
    .delta_limit            = 500,
    .shield_threshold       = 5,
    .shield_gain            = 30,
    .shield_attn            = 1,
    .shield_sens            = 800,
    .keydetector_params.mbw = &nt_keydetector_mbw_El_3,
    .keydetector_interface  = &nt_keydetector_mbw_interface,
    .pin_input              = FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_4,
};
const struct nt_electrode El_7 = {
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
};
const struct nt_electrode El_8 = {
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
    .pin_input              = FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_3,
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
    .pin_input              = FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_4,
};


const tsi_config_t tsi_hw_config = {
    .configSelfCap.commonConfig.mainClock     = kTSI_MainClockSlection_0,
    .configSelfCap.commonConfig.ssc_mode      = kTSI_ssc_prbs_method,
    .configSelfCap.commonConfig.mode          = kTSI_SensingModeSlection_Self,
    .configSelfCap.commonConfig.dvolt         = kTSI_DvoltOption_3,
    .configSelfCap.commonConfig.cutoff        = kTSI_SincCutoffDiv_1,
    .configSelfCap.commonConfig.order         = kTSI_SincFilterOrder_2,
    .configSelfCap.commonConfig.decimation    = kTSI_SincDecimationValue_28,
    .configSelfCap.commonConfig.chargeNum     = kTSI_SscChargeNumValue_4,
    .configSelfCap.commonConfig.prbsOutsel    = kTSI_SscPrbsOutsel_2,
    .configSelfCap.commonConfig.noChargeNum   = kTSI_SscNoChargeNumValue_2,
    .configSelfCap.commonConfig.ssc_prescaler = kTSI_ssc_div_by_2,
    .configSelfCap.enableSensitivity          = true,
    .configSelfCap.enableShield               = kTSI_shieldAllOff,
    .configSelfCap.xdn                        = kTSI_SensitivityXdnOption_1,
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
};

const struct nt_system_xtalk_params my_xtalk_params= {
    .actMat =(uint16_t*)actMat,
    .nt_xtalk_neighbours = 4,
    .nt_xtalk_adapt_touch_time = 5,
};

const struct nt_electrode *const ASlider_2_controls[]        = {&El_1, &El_2, NULL};
const struct nt_electrode *const Keypad_1_controls[]         = {&El_3, &El_4, &El_5, &El_6, NULL};
const struct nt_electrode *const ARotary_3_controls[]        = {&El_7, &El_8, &El_9, &El_10, NULL};
const struct nt_control_arotary nt_control_arotary_ARotary_3 = {
    .range = 200,
};
const struct nt_control_aslider nt_control_aslider_ASlider_2 = {
    .range         = 100,
    .insensitivity = 3,
};

const struct nt_control_keypad nt_control_keypad_Keypad_1 = {
    .groups           = NULL,
    .groups_size      = 0,
    .multi_touch      = (uint32_t[]){0x06, 0x0C, 0x0A, 0x0E},
    .multi_touch_size = 4,
};

const struct nt_control Keypad_1 = {
    .electrodes            = &Keypad_1_controls[0],
    .control_params.keypad = &nt_control_keypad_Keypad_1,
    .interface             = &nt_control_keypad_interface,
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

const struct nt_electrode *const nt_tsi_module_electrodes_1[] = {&El_1, &El_2,
                                                                 &El_3, &El_4,  &El_5, &El_6, 
                                                                 &El_7, &El_8, &El_9, &El_10, NULL};

const struct nt_electrode *const nt_tsi_module_xtalk_electrodes[] = {&El_3,  &El_4, &El_5,  &El_6, NULL};
const struct nt_module nt_tsi_module_1                       = {
    .interface          = &nt_module_tsi_interface,
    .wtrmark_hi         = 65535,
    .wtrmark_lo         = 0,
    .config             = (void *)&tsi_hw_config,
    .instance           = 0,
    .electrodes         = &nt_tsi_module_electrodes_1[0],
};

const struct nt_control *const System_0_controls[] = {&Keypad_1, &ASlider_2, &ARotary_3, NULL};
const struct nt_module *const System_0_modules[]   = {&nt_tsi_module_1, NULL};
const struct nt_system System_0                    = {
    .time_period            = 60,
    .init_time              = 400,
    .controls               = &System_0_controls[0],
    .modules                = &System_0_modules[0],
    .xtalk_interface        = &nt_system_xtalk_interface,
    .xtalk_params           = (void *)&my_xtalk_params,
    .xtalk_electrodes       = &nt_tsi_module_xtalk_electrodes[0],
};
void nt_enable(void)
{
    nt_electrode_enable(&El_1, 0); 
    nt_electrode_enable(&El_2, 0); 
    nt_electrode_enable(&El_3, 0); 
    nt_electrode_enable(&El_4, 0); 
    nt_electrode_enable(&El_5, 0); 
    nt_electrode_enable(&El_6, 0);  
    nt_electrode_enable(&El_7, 0); 
    nt_electrode_enable(&El_8, 0);
    nt_electrode_enable(&El_9, 0);
    nt_electrode_enable(&El_10, 0);
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
