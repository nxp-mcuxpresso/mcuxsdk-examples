/*
 * Copyright 2025 NXP
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
#define BOARD_EEPROM_LPSPI_BASEADDR  (LPSPI0)
#define BOARD_LPSPI_CLK_FREQ         CLOCK_GetLpspiClkFreq(0u)
#define BOARD_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define BOARD_LPSPI_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
