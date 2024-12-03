/*
 * Copyright 2019 NXP
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
#define EXAMPLE_LPSPI_MASTER_BASEADDR (LPSPI0)
#define EXAMPLE_LPSPI_MASTER_IRQN     (LPSPI0_IRQn)

#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME   (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ   (CLOCK_GetIpFreq(EXAMPLE_LPSPI_MASTER_CLOCK_NAME))

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_BASEADDR (LPSPI2)
#define EXAMPLE_LPSPI_SLAVE_IRQN     (LPSPI2_IRQn)

#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME   (kCLOCK_Lpspi2)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

/*INTMUX setting*/
#define EXAMPLE_LPSPI_INTMUX_CHANNLE (0U)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
