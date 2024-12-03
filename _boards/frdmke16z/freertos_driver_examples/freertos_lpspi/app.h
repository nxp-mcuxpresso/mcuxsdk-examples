/*
 * Copyright 2018 NXP
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
/* LPSPI FreeRTOS board to board example */
#define EXAMPLE_CONNECT_SPI BOARD_TO_BOARD
#define SPI_MASTER_SLAVE    isMASTER /* Change to isSLAVE to run as slave. */

#define EXAMPLE_LPSPI_MASTER_BASEADDR         LPSPI0
#define EXAMPLE_LPSPI_MASTER_IRQN             LPSPI0_IRQn
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     kLPSPI_Pcs3
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER kLPSPI_MasterPcs3

#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME   (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ   (CLOCK_GetIpFreq(EXAMPLE_LPSPI_MASTER_CLOCK_NAME))

/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         LPSPI0
#define EXAMPLE_LPSPI_SLAVE_IRQN             LPSPI0_IRQn
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     kLPSPI_Pcs3
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs3
#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME       (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE     (kCLOCK_IpSrcFircAsync)
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
