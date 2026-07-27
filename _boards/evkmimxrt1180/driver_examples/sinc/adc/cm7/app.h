/*
 * Copyright 2022, 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_SINC                          SINC2
#define DEMO_SINC_IRQn                     SINC2_CH3_IRQn
#define DEMO_SINC_IRQ_HANDLER              SINC2_CH3_IRQHandler
#define DEMO_SINC_CHANNEL                  (3U)
#define DEMO_SINC_CONV_COMPLETE_INT_STATUS kSINC_CH3ConvCompleteIntStatus
#define DEMO_SINC_CHANNEL_ID               kSINC_Channel3
#define DEMO_SINC_INPUT_CLK_SOURCE         kSINC_InputClk_SourceExternalModulatorClk
#define DEMO_SINC_CONV_COMPLETE_INT_ENABLE kSINC_CH3ConvCompleteIntEnable
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
