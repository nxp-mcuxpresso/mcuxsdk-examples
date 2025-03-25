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
#define DEMO_CMU                         CMU_3
#define CMU_FC_REF_CLK_FREQ              CLOCK_GetFxoscFreq()       /* CMU FC reference clock */
#define CMU_FC_MONITORED_CLK_FREQ        CLOCK_GetCoreClkFreq()     /* CMU FC monitored clock */
#define CMU_FC_BUS_CLK_FREQ              CLOCK_GetAipsSlowClkFreq() /* CMU FC bus clock */
#define CMU_FM_REF_CLK_DEVIATION         0.00002f /* Reference clock deviation from crystal oscillator datasheet */
#define CMU_FM_MONITORED_CLK_DEVIATION   0.00002f /* Monitored clock is from PLL, use crystal oscillator deviation*/
#define IS_DESTRUCTIVE_RESET             (0U != (MC_RGM->DES & MC_RGM_DES_CORE_CLK_FAIL_MASK))
#define CLR_DESTRUCTIVE_RESET_FLAG       (MC_RGM->DES &= MC_RGM_DES_CORE_CLK_FAIL_MASK)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
