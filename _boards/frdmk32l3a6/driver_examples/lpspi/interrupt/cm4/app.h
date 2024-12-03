/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define EXAMPLE_LPSPI_MASTER_BASEADDR (LPSPI0)
#define EXAMPLE_LPSPI_MASTER_IRQN     (LPSPI0_IRQn)

#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME   (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ   (CLOCK_GetIpFreq(EXAMPLE_LPSPI_MASTER_CLOCK_NAME))

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs2)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs2)

#define EXAMPLE_LPSPI_MASTER_IRQHandler (LPSPI0_IRQHandler)

/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_BASEADDR (LPSPI3)
#define EXAMPLE_LPSPI_SLAVE_IRQN     (LPSPI3_IRQn)

#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME   (kCLOCK_Lpspi3)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs1)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs1)

#define EXAMPLE_LPSPI_SLAVE_IRQHandler (LPSPI3_IRQHandler)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
