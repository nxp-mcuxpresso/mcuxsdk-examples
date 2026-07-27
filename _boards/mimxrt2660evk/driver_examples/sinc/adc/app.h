/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_SINC                          HSP__SINC_1
#define DEMO_SINC_IRQn                     HSP_SINC1_CH0_IRQn
#define DEMO_SINC_IRQ_HANDLER              HSP_SINC1_CH0_IRQHandler
#define DEMO_SINC_CHANNEL                  (0U)
#define DEMO_SINC_CONV_COMPLETE_INT_STATUS kSINC_CH0ConvCompleteIntStatus
#define DEMO_SINC_CHANNEL_ID               kSINC_Channel0
#define DEMO_SINC_INPUT_CLK_SOURCE         kSINC_InputClk_SourceMclkOut0
#define DEMO_SINC_CONV_COMPLETE_INT_ENABLE kSINC_CH0ConvCompleteIntEnable
#define DEMO_SINC_DISABLE_MOD_CLK0_OUTPUT  (false)
#define DEMO_SINC_DISABLE_MOD_CLK1_OUTPUT  (true)
#define DEMO_SINC_DISABLE_MOD_CLK2_OUTPUT  (true)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
