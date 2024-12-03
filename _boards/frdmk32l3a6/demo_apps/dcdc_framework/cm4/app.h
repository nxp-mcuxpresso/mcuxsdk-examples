/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/* LPTMRx. */
#define LPTMRx_BASE         LPTMR0
#define LPTMRx_INTERVAL_MS  1000U /* 50s. */
#define LPTMRx_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)
#define LPTMRx_IRQn         LPTMR0_IRQn
#define LPTMRx_IRQHandler   LPTMR0_IRQHandler

/* LPADCx. */
#define LPADCx_BASE                         LPADC0
#define LPADCx_USER_CMD_IDX                 1U
#define LPADCx_USER_TRIGGER_IDX             0U
#define LPADCx_MEASURE_VBANDGAP_CHANNEL_NUM 27U /* CH27B, Bandgap. */
#define LPADCx_MEASURE_VBATT_CHANNEL_NUM    31U /* CH31B, DCDC. */

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
