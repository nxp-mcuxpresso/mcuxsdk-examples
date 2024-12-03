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
#define EXAMPLE_LPSPI_MASTER_BASEADDR     (LPSPI1)
#define EXAMPLE_LPSPI_MASTER_IRQN         (LPSPI1_IRQn)
#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ   (CLOCK_GetIpFreq(kCLOCK_Lpspi1))
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT (kLPSPI_Pcs1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
