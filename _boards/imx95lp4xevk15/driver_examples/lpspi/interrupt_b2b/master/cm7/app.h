/*
 * Copyright 2024 NXP
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
#define EXAMPLE_LPSPI_MASTER_BASEADDR         LPSPI1
#define LPSPI_MASTER_CLOCK_ROOT               hal_clock_lpspi1
#define LPSPI_MASTER_CLK_FREQ                 (HAL_ClockGetIpFreq(LPSPI_MASTER_CLOCK_ROOT))
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER kLPSPI_MasterPcs0
#define EXAMPLE_LPSPI_MASTER_IRQN             LPSPI1_IRQn
#define EXAMPLE_LPSPI_MASTER_IRQHandler       LPSPI1_IRQHandler

#define EXAMPLE_LPSPI_DEALY_COUNT 0xfffff
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
