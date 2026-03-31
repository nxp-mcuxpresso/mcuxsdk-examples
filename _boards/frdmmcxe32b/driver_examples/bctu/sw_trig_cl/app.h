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
#define DEMO_ADC_BASE_0                 ADC_0
#define DEMO_ADC_BASE_1                 ADC_1
#define DEMO_ADC_BASE_2                 ADC_2
#define DEMO_ADC_CLOCK_FREQ             kADC_ModuleClockFreqDivide2
#define DEMO_ADC_BCTU_MODE              kADC_BctuTrig
#define DEMO_ADC_CONV_AVERAGE           kADC_ConvAvg32
#define DEMO_ADC_CAL_AVERAGE_ENABLE     true
#define DEMO_ADC_CAL_SAMPLE_TIME        kADC_SampleTime22
#define DEMO_ADC_CAL_AVERAGE_SAMPLE_NUM kADC_AverageSampleNumbers32
   
#define DEMO_BCTU_BASE                  BCTU
#define DEMO_BCTU_REG_PROTECT           kBCTU_ProtectDis_Permanent
#define DEMO_BCTU_TRIG_CHAN_ADDR        0U
#define DEMO_BCTU_DATA_DEST             kBCTU_DataDest_Fifo1
#define DEMO_BCTU_LOOP_ENABLE           false
#define DEMO_BCTU_TRIG_ADC_INSTANCE     ((uint32_t)kBCTU_TrigAdc_0 | (uint32_t)kBCTU_TrigAdc_1 | (uint32_t)kBCTU_TrigAdc_2)
#define DEMO_BCTU_TRIG_RESOLUTION       kBCTU_TrigRes_ConvList
#define DEMO_BCTU_TRIGGER_INDEX         0U
#define DEMO_BCTU_FIFO_INDEX            kBCTU_Fifo_1
#define DEMO_BCTU_INT_MASK              (uint32_t)kBCTU_Fifo_1_Int
#define DEMO_BCTU_IRQn                  BCTU_IRQn
#define DEMO_BCTU_IRQ_HANDLER_FUNC      BCTU_IRQHandler
#define DEMO_BCTU_ADC_CHANNEL_0         48U
#define DEMO_BCTU_ADC_CHANNEL_1         54U
#define DEMO_BCTU_ADC_CHANNEL_2         55U
#define DEMO_BCTU_SW_TRIG_GROUP         kBCTU_TrigGroup_0
#define DEMO_BCTU_SW_TRIG_MASK          kBCTU_TrigMask_0
/*${macro:end}*/
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
