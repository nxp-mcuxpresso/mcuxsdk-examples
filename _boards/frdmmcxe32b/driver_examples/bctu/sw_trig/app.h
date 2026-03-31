/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_ADC_BASE                   ADC_0
#define DEMO_ADC_CLOCK_FREQ             kADC_ModuleClockFreqDivide2
#define DEMO_ADC_BCTU_MODE              kADC_BctuTrig
#define DEMO_ADC_CONV_AVERAGE           kADC_ConvAvg32
#define DEMO_ADC_CAL_AVERAGE_ENABLE     true
#define DEMO_ADC_CAL_SAMPLE_TIME        kADC_SampleTime22
#define DEMO_ADC_CAL_AVERAGE_SAMPLE_NUM kADC_AverageSampleNumbers32

#define DEMO_BCTU_BASE                  BCTU
#define DEMO_BCTU_TRIG_ADC_ADDR         48U
#define DEMO_BCTU_DATA_DEST             kBCTU_DataDest_AdcReg
#define DEMO_BCTU_LOOP_ENABLE           false
#define DEMO_BCTU_TRIG_ADC_INSTANCE     kBCTU_TrigAdc_0
#define DEMO_BCTU_TRIG_RESOLUTION       kBCTU_TrigRes_SingleConv
#define DEMO_BCTU_TRIGGER_INDEX         0U
#define DEMO_BCTU_INT_MASK              (uint32_t)kBCTU_NewData_0_Ready
#define DEMO_BCTU_SW_TRIG_GROUP         kBCTU_TrigGroup_0
#define DEMO_BCTU_SW_TRIG_MASK          (uint32_t)kBCTU_TrigMask_0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
