/*
 * Copyright 2021 NXP
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
/*Master related*/
#define EXAMPLE_LPSPI_MASTER_BASEADDR         LPSPI0
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE     (kCLOCK_Pcc0BusIpSrcCm33Bus)
#define EXAMPLE_LPSPI_MASTER_IRQN             LPSPI0_IRQn
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER kLPSPI_MasterPcs0

#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ (CLOCK_GetIpFreq(kCLOCK_Lpspi0))

/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         LPSPI1
#define EXAMPLE_LPSPI_SLAVE_IRQN             LPSPI1_IRQn
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs0

#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_Pcc0BusIpSrcCm33Bus)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
