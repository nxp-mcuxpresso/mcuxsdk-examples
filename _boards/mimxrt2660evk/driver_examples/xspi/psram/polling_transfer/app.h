/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*${macro:start}*/

#define EXAMPLE_XSPI (MAIN__XSPI_1)

/* XSPI1 AHB window base on the LLC-cached path (0x8800_0000). All CPU (AHB)
 * PSRAM accesses use this path: the LLC turns any CPU burst into aligned
 * cache-line fills, which the XSPI AHB read buffering requires - unaligned
 * bursts on the direct (LLC-bypass) 0x8000_0000 path lose/insert stale bytes
 * at internal buffer-line boundaries (measured on silicon; hello_world psram
 * data placement uses the same path for the same reason). */
#define BOARD_XSPI1_AMBA_BASE 0x88000000U

/* Device offset 0 as seen by the IP-command engine. SFAR addresses do not
 * traverse the interconnect (no LLC/direct distinction) and must fall inside
 * the SFA1AD/SFA2AD map, which starts at the direct window base. Used by the
 * mode-register / reset / IP data sequences. */
#define BOARD_XSPI1_AMBA_BASE_DIRECT 0x80000000U

/* Test window swept by the example, resolved at run time:
 *  - flash/SRAM-linked targets: PSRAM is entirely unused by the linker, test
 *    from device offset 0 of the LLC-cached window;
 *  - PSRAM-resident targets (psram/psram_txt/xspi_nor_psram): the private
 *    linker scripts carve the last 32 KB of the mapped PSRAM out of NCACHE
 *    (__PSRAM_TEST_START__, a 0x8800_0000-based address) so the sweep cannot
 *    touch the linked image.
 * BOARD_InitHardware covers the active window with an inner-non-cacheable /
 * outer-write-back MPU region: the L1 cache stays out (keeps the AHB memcpy
 * path coherent with the IP-command path) while the LLC stays in. */
#define EXAMPLE_XSPI_AMBA_BASE (BOARD_PsramTestWindowBase())

/* The same window on the IP-command (SFAR) address map, used by the IP data
 * transfer functions; the two bases differ by the fixed 0x0800_0000 path
 * offset. */
#define EXAMPLE_XSPI_AMBA_BASE_DIRECT (BOARD_PsramTestWindowBase() - 0x08000000U)

/* Byte range swept by the example in 1 KB steps; must not exceed the linker
 * carve-out (__PSRAM_TEST_SIZE__ = 32 KB) on PSRAM-resident targets. All
 * accesses stay 1 KB aligned - mandatory at 250 MHz, where the device does
 * not support row-boundary-crossing reads. */
#define DRAM_SIZE 0x8000U

/* LUT sequence indices (APS512XXN Xccela command set, LUT defined in the
 * board xspi_psram_ops.c). */
#define HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ  0
#define HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE 1
#define HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ    2
#define HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE   3
#define HYPERRAM_CMD_LUT_SEQ_IDX_RESET       4

#define CUSTOM_LUT_LENGTH 80
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

#if defined(__ICCARM__)
extern __weak uint8_t __NCACHE_REGION_START[];
extern __weak uint8_t __PSRAM_TEST_START__[];
#else
extern uint8_t __NCACHE_REGION_START[];
extern uint8_t __PSRAM_TEST_START__[] __attribute__((weak));
#endif

/* The PSRAM-resident linker scripts place NCACHE inside the 0x8800_0000
 * (LLC-cached) PSRAM window; every other target keeps it in SRAM (or, on the
 * IAR flash/SRAM targets, does not define the symbol at all - a weak
 * reference of 0 reads as non-resident). */
static inline bool BOARD_IsPsramResident(void)
{
    return ((uint32_t)__NCACHE_REGION_START >= 0x88000000U);
}

static inline uint32_t BOARD_PsramTestWindowBase(void)
{
    return BOARD_IsPsramResident() ? (uint32_t)__PSRAM_TEST_START__ : BOARD_XSPI1_AMBA_BASE;
}
/*${prototype:end}*/

#endif /* _APP_H_ */
