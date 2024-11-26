/*
 * Copyright 2024 NXP
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
#define DEMO_LPSPI_MASTER_BASEADDR   (LPSPI3)
#define DEMO_LPSPI_MASTER_IRQN       (LPSPI3_IRQn)
#define DEMO_LPSPI_MASTER_IRQHandler (LPSPI3_IRQHandler)

#define DEMO_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define DEMO_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define DEMO_LPSPI_DEFAULT_VALUE           (0x7FU)

#define DEMO_LPSPI_CLOCK hal_clock_lpspi3
#define LPSPI_MASTER_CLK_FREQ HAL_ClockGetRate(DEMO_LPSPI_CLOCK)

#define DEMO_SINC                 (SINC2)
#define DEMO_SINC_IRQn            (SINC2_CH0_IRQn)
#define DEMO_SINC_IRQ_HANDLER     SINC2_CH0_IRQHandler
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
