/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_SEMA42           SEMA42_1
#define APP_SEMA42_GATE      0
/*
 * Master ID assignment for the SoC,
 * 4'b0000-4'b0011 -- For Core0 to Core3 Exclusive Access & Device Transactions
 * 4'b0100 -- Cortex-M7 core1
 * 4'b0101 -- Cortex-M33 core1(CM33_Sync), eDMA4(AXI)
 * 4'b0110 -- PCIe2
 * 4'b0111 -- Cortex-A DSU L3 evicted/ACP transactions(including EDMA3)
 * 4'b1000 -- Cortex-M33 core0
 * 4'b1001 -- Cortex-M7 core0
 * 4'b1010 -- Edgelock Enclave
 * 4'b1011 -- eDMA1(AHB)
 * 4'b1100 -- eDMA2(AXI)
 * 4'b1101 -- NETC
 * 4'b1110 -- V2X-FH SOC_OUT
 * 4'b1111 -- All other bus masters.
 */
/* For Cortex-M7 core0 */
#define APP_CORTEX_M7_DID     9
/* For Cortex-M33 core0(System Manager is running on the core) */
#define APP_CORTEX_M33_DID     8
#define APP_SEMA42_GATE_ADDR 0x44260003 /* Address of the SEMA42 gate. */

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InitDomainConfig(void);
void APP_DeinitDomainConfig(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
