/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_xspi.h"
#include "llc_example_platform.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
static bool s_platformInited = false;

/* RT2660 platform description handed to the portable LLC example. The example
 * exercises the LLC-cached window (0x8800_0000) so every CPU burst becomes an
 * aligned LLC line fill. board.c BOARD_EarlyConfigLLC() already ran LLC init +
 * enable during boot (SystemInit -> BOARD_EarlyInit), so the LLC is live in
 * main and must not be re-initialized: bootInitedLlc = true. */
static const llc_example_platform_t s_llcPlatform = {
    .instance      = BOARD_LLC_INSTANCE,
    .regionBase    = BOARD_LLC_CACHED_BASE,
    .regionSize    = BOARD_LLC_REGION_SIZE,
    .bootInitedLlc = true,
};
/*${variable:end}*/

/*${function:start}*/
/* Cover the LLC-cached test window (0x8800_0000) with inner-non-cacheable /
 * outer-write-back attributes: inner NC keeps the CM33 L1 D-cache out of the
 * window (so the CPU bursts the example issues reach the LLC directly), while
 * outer WB keeps the LLC in - it turns every CPU burst into aligned line fills
 * (required; unaligned AHB bursts on the direct 0x8000_0000 path corrupt at
 * XSPI buffer-line boundaries). Region 10 / attribute index 3 are free after
 * BOARD_ConfigMPU, mirroring the psram polling_transfer example. */
static void BOARD_SetLlcTestWindowInnerNonCacheable(void)
{
    uint32_t windowBase = BOARD_LLC_CACHED_BASE;
    uint32_t windowEnd  = BOARD_LLC_CACHED_BASE + BOARD_LLC_REGION_SIZE - 1U;

    __DMB();
    ARM_MPU_SetRegion(10U, ARM_MPU_RBAR(windowBase, ARM_MPU_SH_NON, 0U, 1U, 1U), ARM_MPU_RLAR(windowEnd, 3U));
    __DSB();
    __ISB();
}

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console
     * init. This also runs BOARD_EarlyConfigLLC() which brings the LLC online. */
    BOARD_CommonSetting();

    /* XSPI1 pin mux for the APS512XXN PSRAM (the LLC test memory). */
    BOARD_InitXSPI1PsRamPins();

    /* Cover the LLC-cached test window with inner-NC / outer-WB. */
    BOARD_SetLlcTestWindowInnerNonCacheable();
}

const llc_example_platform_t *LLC_ExampleGetPlatform(void)
{
    return &s_llcPlatform;
}

void LLC_ExamplePlatformInit(void)
{
    if (s_platformInited)
    {
        return;
    }

    /* Bring the XSPI1 APS512XXN PSRAM up in software: this example links no
     * Boot ROM XMCD, so the PSRAM that backs the LLC test region is NOT live
     * out of reset. This reuses the silicon-validated board bring-up from the
     * xspi/psram polling_transfer driver example: reprogram XSPI1, run the
     * APS512XXN mode-register sequence, switch to X16 @ 250 MHz and re-arm the
     * read DLL. After it returns, the LLC-cached window (0x8800_0000) addresses
     * valid PSRAM cells. */
    xspi_hyper_ram_init(EXAMPLE_XSPI);

    s_platformInited = true;
}
/*${function:end}*/
