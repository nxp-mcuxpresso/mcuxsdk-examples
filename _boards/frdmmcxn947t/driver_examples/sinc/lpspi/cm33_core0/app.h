/*
 * Copyright 2023 NXP
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
/* Master related */
#define DEMO_LPSPI_MASTER_BASEADDR   (LPSPI1)
#define DEMO_LPSPI_MASTER_IRQN       (LP_FLEXCOMM1_IRQn)
#define DEMO_LPSPI_MASTER_IRQHandler (LP_FLEXCOMM1_IRQHandler)

#define DEMO_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define DEMO_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define DEMO_LPSPI_DEFAULT_VALUE           (0x7FU)

#define LPSPI_MASTER_CLK_FREQ CLOCK_GetLPFlexCommClkFreq(1U)

#define DEMO_SINC                 (SINC0)
#define DEMO_SINC_IRQn            (SINC_FILTER_IRQn)
#define DEMO_SINC_IRQ_HANDLER     SINC_FILTER_IRQHandler
#define DEMO_SINC_MOD_CLK_DIVIDER (4UL)
#define DEMO_SINC_OverSampleRatio (139U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
