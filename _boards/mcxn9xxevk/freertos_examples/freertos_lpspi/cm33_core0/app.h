/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
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
#define EXAMPLE_LPSPI_MASTER_BASEADDR         (LPSPI2)
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE     (kCLOCK_IpSrcFircAsync)
#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ       (CLOCK_GetLPFlexCommClkFreq(2U))
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         (LPSPI1)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE     (kCLOCK_IpSrcFircAsync)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
