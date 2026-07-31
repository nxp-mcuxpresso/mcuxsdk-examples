/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
/* The test window (on the 0x8800_0000 LLC-cached path) is not covered by
 * BOARD_ConfigMPU: on flash/SRAM-linked targets the whole PSRAM range is
 * unmapped, and on PSRAM-resident targets the carved window sits above the
 * (shrunk) NCACHE region; either way it falls back to the ARMv8-M default
 * map. Cover it with the same inner-non-cacheable / outer-write-back
 * attributes the board uses for PSRAM NCACHE: inner NC keeps the L1 cache
 * out, so the AHB memcpy path stays coherent with the IP-command path
 * without cache maintenance; outer WB keeps the LLC in, which turns every
 * CPU burst into aligned line fills (required - unaligned AHB bursts on the
 * direct path corrupt at XSPI buffer-line boundaries). Region 10 and
 * attribute index 3 are free/installed by BOARD_ConfigMPU. */
static void BOARD_SetPsRamWindowInnerNonCacheable(void)
{
    uint32_t windowBase = BOARD_PsramTestWindowBase();
    uint32_t windowEnd  = BOARD_IsPsramResident() ? (windowBase + DRAM_SIZE - 1U) : (windowBase + 0x000FFFFFU);

    __DMB();
    ARM_MPU_SetRegion(10U, ARM_MPU_RBAR(windowBase, ARM_MPU_SH_NON, 0U, 1U, 1U), ARM_MPU_RLAR(windowEnd, 3U));
    __DSB();
    __ISB();
}

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitXSPI1PsRamPins();
    BOARD_SetPsRamWindowInnerNonCacheable();
}
/*${function:end}*/
