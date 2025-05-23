/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_trdc.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_TRDC_INSTANCE         MBC0
#define EXAMPLE_TRDC_MBC_SLAVE_INDEX  0U /* 0=Flash, 1=IFR0, 2=IFR1 */

/*
 * Pick two global access registers. They must NOT have set LK bit.
 * When policy with LK bit set is asigned it canot be changed any more so
 * "access resolution" part of this example will not work.
 */
#define EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX           4
#define EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX_NO_ACCESS 5
   
/* 
 * Following must correspord to vlaue set in MBC0_MEM0_GLBCFG_SIZE_LOG2 register.
 * It is used to align MBC protected buffer to the block size.
 */
#define EXAMPLE_TRDC_MBC0_MEM0_GLBCFG_SIZE_LOG2 0xdU
   
/* Use block at the end of FLASH. This is the end CMPA region. Nothing should be there. */
#define EXAMPLE_TRDC_MBC_BLK_ADDR 0x80000U - (1U << EXAMPLE_TRDC_MBC0_MEM0_GLBCFG_SIZE_LOG2)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_SetTrdcGlobalConfig(void);
void APP_SetMbcUnaccessible(void);
void APP_TouchMbcMemory(void);
void APP_ResolveMbcAccessError(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
