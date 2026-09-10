/*
 * Copyright 2026 NXP
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
#define LPSPI_TRANSFER_SIZE          (128U)
#define EXAMPLE_LPSPI_BASEADDR       (LPSPI0)
#define EXAMPLE_LPSPI_CLOCK_FREQ     CLOCK_GetLpspiClkFreq(0u)
#define EXAMPLE_LPSPI_PCS_FOR_INT    (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_PCS_FOR_TRANS  (kLPSPI_MasterPcs0)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
