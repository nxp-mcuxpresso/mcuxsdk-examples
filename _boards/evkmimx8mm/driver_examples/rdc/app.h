/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_RDC             RDC
#define APP_CUR_MASTER      kRDC_Master_M4
#define APP_CUR_MASTER_DID  BOARD_DOMAIN_ID /* Current master domain ID. */
#define APP_RDC_PERIPH      kRDC_Periph_GPIO1
#define APP_RDC_SEMA42      RDC_SEMAPHORE1  /* Current master domain ID. */
#define APP_RDC_SEMA42_GATE (((uint8_t)APP_RDC_PERIPH) & 0x3F)

/* OCRAM is used for demonstration here. */
#define APP_RDC_MEM           kRDC_Mem_MRC3_0
#define APP_RDC_MEM_BASE_ADDR 0x900000
#define APP_RDC_MEM_END_ADDR  0x920000

/*
 * Master index:
 * All masters excluding ARM core: 0
 * A53 core: 1
 * M4 core: 6
 * SDMA 3
 */
#define APP_MASTER_INDEX 6

/*
 * If cache is enabled, this example should maintain the cache to make sure
 * CPU core accesses the memory, not cache only.
 */
#define APP_USING_CACHE 1

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_TouchPeriph(void);
void APP_TouchMem(void);
/*${prototype:end}*/
