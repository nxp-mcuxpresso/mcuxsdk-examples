/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_power.h"
#include "fsl_nand_flash.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_XSPI           XSPI1
#define EXAMPLE_XSPI_NAND_PORT kXSPI_TargetGroup0
#define EXAMPLE_XSPI_AMBA_BASE (0x08000000U)

#define FLASH_PAGE_SIZE 2112U /* W25N01KW: 2048B page + 64B spare. */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
