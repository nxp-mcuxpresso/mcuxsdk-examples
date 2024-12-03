/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v10.0
processor: MKE15Z256xxx7
package_id: MKE15Z256VLL7
mcu_data: ksdk2_0
processor_version: 0.10.11
board: FRDM-KE15Z
functionalGroups:
- name: BOARD_InitPeripherals
  UUID: 9522edab-3968-41a4-8a34-5905f79f26da
  called_from_default_init: true
  id_prefix: DEMO_
  selectedCore: core0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'system'
- type_id: 'system'
- global_system_definitions:
  - user_definitions: ''
  - user_includes: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'uart_cmsis_common'
- type_id: 'uart_cmsis_common_9cb8e302497aa696fdbb5a4fd622c2a8'
- global_USART_CMSIS_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "peripherals.h"

/***********************************************************************************************************************
 * BOARD_InitPeripherals functional group
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * PDB initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'PDB'
- type: 'pdb'
- mode: 'general'
- custom_name_enabled: 'true'
- type_id: 'pdb_32eb8756416d042c4a30e2cf9bd8fca4'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'PDB0'
- config_sets:
  - fsl_pdb:
    - clockConfig:
      - clockSource: 'BusInterfaceClock'
      - clockSourceFreq: 'BOARD_BootClockRUN'
    - pdb_config:
      - loadValueMode: 'kPDB_LoadValueImmediately'
      - firstDivider: 'kPDB_PrescalerDivider1'
      - secondDivider: 'kPDB_DividerMultiplicationFactor1'
      - moduloValue_str: '2000'
      - triggerInputSource: 'kPDB_TriggerInput0'
      - initSWtrigger: 'false'
      - enableContinuousMode: 'false'
    - pdb_adc_triggering_config:
      - 0:
        - pdb_adc_triggered_device: 'ADC0'
        - pdb_adc_pretriggers_config:
          - 0:
            - enable: 'true'
            - outputSource: 'delayedTrigger'
            - delayValue_str: '500'
    - pdb_pulse_out_config: []
    - pdb_delay_interrupt_config:
      - actionAfterDelay: 'noAction'
      - delayValue_str: ''
    - pdb_interrupts_config:
      - interrupt_sel: ''
      - enable_irq: 'false'
      - interrupt:
        - IRQn: 'PDB0_IRQn'
        - enable_interrrupt: 'enabled'
        - enable_priority: 'false'
        - priority: '0'
        - enable_custom_name: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
/* PDB counter configuration */
const pdb_config_t DEMO_PDB_config = {
  .loadValueMode = kPDB_LoadValueImmediately,
  .prescalerDivider = kPDB_PrescalerDivider1,
  .dividerMultiplicationFactor = kPDB_DividerMultiplicationFactor1,
  .triggerInputSource = kPDB_TriggerInput0,
  .enableContinuousMode = false
};
/* PDB ADC0 pre-triggers configuration */
pdb_adc_pretrigger_config_t DEMO_PDB_ADC0_pretriggers_config = {
  .enablePreTriggerMask = 0U | (1U << kPDB_ADCPreTrigger0),
  .enableOutputMask = 0U | (1U << kPDB_ADCPreTrigger0),
  .enableBackToBackOperationMask = 0U
};
const uint32_t DEMO_PDB_ADC0_pretriggers_value[DEMO_PDB_ADC0_PRETRIGGERS_COUNT] = {
  500U
};

static void DEMO_PDB_init(void) {
  /* PDB counter initialization */
  PDB_Init(DEMO_PDB_PERIPHERAL, &DEMO_PDB_config);
  /* PDB counter modulo initialization */
  PDB_SetModulusValue(DEMO_PDB_PERIPHERAL, 2000U);
  /* PDB ADC0 pre-triggers initialization */
  PDB_SetADCPreTriggerConfig(DEMO_PDB_PERIPHERAL, kPDB_ADCTriggerChannel0, &DEMO_PDB_ADC0_pretriggers_config);
  PDB_SetADCPreTriggerDelayValue(DEMO_PDB_PERIPHERAL, kPDB_ADCTriggerChannel0, kPDB_ADCPreTrigger0, DEMO_PDB_ADC0_pretriggers_value[DEMO_PDB_ADC0_PT0]);
  /* Load buffered registers values into the working register for the first time */
  PDB_DoLoadValues(DEMO_PDB_PERIPHERAL);
}

/***********************************************************************************************************************
 * FTM initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'FTM'
- type: 'ftm'
- mode: 'EdgeAligned'
- custom_name_enabled: 'true'
- type_id: 'ftm_a206ca22312775f3c8a462078188c129'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'FTM0'
- config_sets:
  - ftm_main_config:
    - ftm_config:
      - clockSource: 'kFTM_SystemClock'
      - clockSourceFreq: 'BOARD_BootClockRUN'
      - timerPrescaler: '1'
      - timerOutputFrequency: '16000Hz'
      - systemClockSource: 'BusInterfaceClock'
      - systemClockSourceFreq: 'mirrored_value'
      - faultMode: 'kFTM_Fault_Disable'
      - inputFilterPeriod: '1'
      - faultInputs:
        - 0:
          - enableFaultInput: 'false'
          - faultLevelVal: 'low'
          - useFaultFilter: 'false'
        - 1:
          - enableFaultInput: 'false'
          - faultLevelVal: 'low'
          - useFaultFilter: 'false'
        - 2:
          - enableFaultInput: 'false'
          - faultLevelVal: 'low'
          - useFaultFilter: 'false'
        - 3:
          - enableFaultInput: 'false'
          - faultLevelVal: 'low'
          - useFaultFilter: 'false'
      - deadTimePrescale: 'kFTM_Deadtime_Prescale_1'
      - deadTimePeriod: '0'
      - pwmSyncMode: 'kFTM_SoftwareTrigger'
      - reloadPoints: ''
      - extTriggers: 'kFTM_Chnl0Trigger'
      - chnlInitState: ''
      - chnlPolarity: ''
      - bdmMode: 'kFTM_BdmMode_0'
      - useGlobalTimeBase: 'false'
    - timer_interrupts: ''
    - enable_irq: 'false'
    - ftm_interrupt:
      - IRQn: 'FTM0_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - EnableTimerInInit: 'false'
  - ftm_edge_aligned_mode:
    - ftm_edge_aligned_channels_config:
      - 0:
        - channelId: 'pwm'
        - edge_aligned_mode: 'kFTM_AsymmetricalCombinedPwm'
        - combined_pwm:
          - chnlNumber: 'kFTM_Chnl_0'
          - level: 'kFTM_HighTrue'
          - combinedMode: 'asymmetrical'
          - firstEdgeValueStr: '20%'
          - dutyValueStr: '50%'
          - enableComplementary: 'false'
          - enableDeadtime: 'false'
          - enable_chan_irq: 'false'
          - enable_chan1_irq: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const ftm_config_t DEMO_FTM_config = {
  .prescale = kFTM_Prescale_Divide_1,
  .faultMode = kFTM_Fault_Disable,
  .faultFilterValue = 0,
  .deadTimePrescale = kFTM_Deadtime_Prescale_1,
  .deadTimeValue = 0,
  .pwmSyncMode = kFTM_SoftwareTrigger,
  .reloadPoints = 0,
  .extTriggers = kFTM_Chnl0Trigger,
  .chnlInitState = 0,
  .chnlPolarity = 0,
  .bdmMode = kFTM_BdmMode_0,
  .useGlobalTimeBase = false
};

const ftm_chnl_pwm_config_param_t DEMO_FTM_asymmetricalCombinedPwmSignalParams[] = { 
  {
    .chnlNumber = kFTM_Chnl_0,
    .level = kFTM_HighTrue,
    .firstEdgeValue = 900,
    .dutyValue = 2250,
    .enableComplementary = false,
    .enableDeadtime = false,
  }
};

static void DEMO_FTM_init(void) {
  FTM_Init(DEMO_FTM_PERIPHERAL, &DEMO_FTM_config);
  FTM_SetTimerPeriod(DEMO_FTM_PERIPHERAL, DEMO_FTM_TIMER_MODULO_VALUE);
  FTM_SetupPwmMode(DEMO_FTM_PERIPHERAL, DEMO_FTM_asymmetricalCombinedPwmSignalParams, sizeof(DEMO_FTM_asymmetricalCombinedPwmSignalParams) / sizeof(ftm_chnl_pwm_config_param_t), kFTM_AsymmetricalCombinedPwm);
}

/***********************************************************************************************************************
 * ADC initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'ADC'
- type: 'adc12'
- mode: 'ADC12'
- custom_name_enabled: 'true'
- type_id: 'adc12_5324d28dd0212c08055a9d9cd4317082'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'ADC0'
- config_sets:
  - fsl_adc12:
    - enable_irq: 'true'
    - adc_interrupt:
      - IRQn: 'ADC0_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - adc12_config:
      - referenceVoltageSource: 'kADC12_ReferenceVoltageSourceVref'
      - clockSource: 'kADC12_ClockSourceAlt0'
      - clockSourceFreq: 'BOARD_BootClockRUN'
      - clockDivider: 'kADC12_ClockDivider1'
      - resolution: 'kADC12_Resolution8Bit'
      - sampleClockCount: '13'
      - enableContinuousConversion: 'false'
    - adc12HardwareCompareConfig:
      - hardwareCompareModeEnable: 'false'
    - adc12_hardware_average_mode: 'kADC12_HardwareAverageDisabled'
    - hardwareTrigger: 'true'
    - enableDMA: 'false'
    - doAutoCalibration: 'false'
    - offset: '0'
    - gain: '0'
    - adc12_channels_config:
      - 0:
        - channelName: ''
        - channelNumber: 'SE.1'
        - enableInterruptOnConversionCompleted: 'true'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const adc12_config_t DEMO_ADC_config = {
  .referenceVoltageSource = kADC12_ReferenceVoltageSourceVref,
  .clockSource = kADC12_ClockSourceAlt0,
  .clockDivider = kADC12_ClockDivider1,
  .resolution = kADC12_Resolution8Bit,
  .sampleClockCount = 13UL,
  .enableContinuousConversion = false
};
adc12_channel_config_t DEMO_ADC_channelsConfig[1] = {
  {
    .channelNumber = 1U,
    .enableInterruptOnConversionCompleted = true
  }
};
const adc12_hardware_average_mode_t DEMO_ADC_hardwareAverageConfig = kADC12_HardwareAverageDisabled;

static void DEMO_ADC_init(void) {
  /* Initialize ADC12 converter */
  ADC12_Init(DEMO_ADC_PERIPHERAL, &DEMO_ADC_config);
  /* Set to hardware trigger mode */
  ADC12_EnableHardwareTrigger(DEMO_ADC_PERIPHERAL, true);
  /* Configure hardware average mode */
  ADC12_SetHardwareAverage(DEMO_ADC_PERIPHERAL, DEMO_ADC_hardwareAverageConfig);
  /* Set the offset value for the conversion result */
  ADC12_SetOffsetValue(DEMO_ADC_PERIPHERAL, (uint32_t) 0);
  /* Set the gain value for the conversion result */
  ADC12_SetGainValue(DEMO_ADC_PERIPHERAL, 0);
  /* Enable generating the DMA trigger when conversion is completed */
  ADC12_EnableDMA(DEMO_ADC_PERIPHERAL, false);
  /* Enable interrupt ADC0_IRQn request in the NVIC. */
  EnableIRQ(DEMO_ADC_IRQN);
}

/***********************************************************************************************************************
 * NVIC initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'NVIC'
- type: 'nvic'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'nvic_57b5eef3774cc60acaede6f5b8bddc67'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'NVIC'
- config_sets:
  - nvic:
    - interrupt_table:
      - 0: []
    - interrupts: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/* Empty initialization function (commented out)
static void DEMO_NVIC_init(void) {
} */

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void)
{
  /* Initialize components */
  DEMO_PDB_init();
  DEMO_FTM_init();
  DEMO_ADC_init();
}

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void)
{
  BOARD_InitPeripherals();
}
