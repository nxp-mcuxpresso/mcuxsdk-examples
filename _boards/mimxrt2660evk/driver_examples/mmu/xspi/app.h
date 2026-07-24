/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_MMU_INSTANCE    (MAIN__MMU)
/* Upper 16 bits of XSPI1/PSRAM range: 0x8000_0000 ~ 0x8FFF_FFFF */
#define DEMO_MMU_BASE_ADDR   (0x8000UL)
#define DEMO_MMU_MAX_ADDR    (0x8FFFUL)
#define DEMO_MMU_UNITE_SIZE  (kMMU_UnitSize8KB)
#define DEMO_UNIT_BYTE_SIZE  (0x2000UL)
#define DEMO_IRQN            (MAIN_MMU_IRQn)
#define DEMO_MMU_IRQ_HANDLER MAIN_MMU_IRQHandler

/* XSPI1 (PSRAM, 0x8000_0000, read-write) is used for MMU test */
#define DEMO_MMU_ADDR_ARRAY_SIZE  (4U)
#define DEMO_MMU_PHY_ADDR_ARRAY     {0x8000000UL, 0x8000000UL, 0x8000000UL, 0x8000000UL}
#define DEMO_MMU_PHY_ADDR_STR       "{0x8000000UL, 0x8000000UL, 0x8000000UL, 0x8000000UL}"
#define DEMO_MMU_VIRTUAL_ADDR_ARRAY {0x8000000UL, 0x8004000UL, 0x8006000UL, 0x8008000UL}
#define DEMO_MMU_VIRTUAL_ADDR_STR   "{0x8000000UL, 0x8004000UL, 0x8006000UL, 0x8008000UL}"

#define DEMO_TEST_BUFFER_LEN      (256UL)
#define DEMO_SIMPLE_MAP_VIRT_ADDR (0x8002000UL)
#define DEMO_SIMPLE_MAP_PHYS_ADDR (0x8000000UL)

#define DEMO_REGION_MAP_VIRT_ADDR (0x8100000UL)
#define DEMO_REGION_MAP_PHYS_ADDR (0x8000000UL)
#define DEMO_REGION_SIZE          (0x100000UL)

/* RT2660 CM85: L1 D-cache (SCB) + LLC L2 cache clean+invalidate for full coherency.
 * LLC call is via forward declaration in hardware_init.c to avoid fsl_llc.h ODR issue. */
#define DEMO_CLEAN_INVALIDA_CACHE do { SCB_CleanInvalidateDCache(); DEMO_CleanInvalidateL2Cache(); } while(0)
/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/


/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DEMO_CleanInvalidateL2Cache(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
