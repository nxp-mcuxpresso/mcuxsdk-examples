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
#define DEMO_CMU CMU_2
#define DEMO_CMU_IRQ CMU2_IRQn
   
#define CMU_FM_REF_CLK_FREQ CLOCK_GetFxoscFreq()        /* CMU FM reference clock */
#define CMU_FM_BUS_CLK_FREQ CLOCK_GetAipsSlowClkFreq()  /* CMU FM bus clock */
#define CMU_FM_MONITORED_CLK_FREQ CLOCK_SIRC_CLK_FREQ   /* CMU FM monitored clock */
#define CMU_FM_MONITORED_CLK_UP_DEVIATION 0.1f          /* Frequency up deviation from data sheet */
#define CMU_FM_MONITORED_CLK_DOWN_DEVIATION 0.1f        /* Frequency down deviation from data sheet */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
