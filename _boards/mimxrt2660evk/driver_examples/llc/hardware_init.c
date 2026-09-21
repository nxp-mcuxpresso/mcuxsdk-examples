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
#include "fsl_debug_console.h"
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

    /* XSPI1 pin mux for the PSRAM (the LLC test memory). */
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

    /* The PSRAM that backs the LLC test region must be live before the example
     * touches the LLC-cached window (0x8800_0000). reconfig.cmake defines
     * USE_PSRAM, so the boot header carries the board's XMCD and the Boot ROM
     * has already configured XSPI1 and the device by the time this runs. Every
     * target of this example is XIP from XSPI0 NOR, so that header is always
     * present, and the XMCD exists for both board PSRAM variants - so there is
     * no software bring-up here to fall back on.
     *
     * Check the result instead of assuming it: X16_EN reports a live DDR PSRAM
     * (the same runtime test clock_config.c uses for livePsram, and the reason
     * it is a runtime test and not a build macro). The only way it can be clear
     * on a working board is a build whose XMCD does not match the part
     * populated, so say exactly that rather than letting the example fault on
     * an absent test region. */
    if ((EXAMPLE_XSPI->MCR & XSPI_MCR_X16_EN_MASK) == 0U)
    {
        PRINTF("Boot ROM left XSPI1 without a live DDR PSRAM (MCR=0x%08x).\r\n", (unsigned int)EXAMPLE_XSPI->MCR);
        PRINTF("The LLC test region is not usable: set USE_PSRAM_W958D6 to match\r\n");
        PRINTF("the PSRAM populated on this board (1 = Winbond W958D6NMYA4I,\r\n");
        PRINTF("0 = AP Memory Xccela) and rebuild.\r\n");
    }

    s_platformInited = true;
}
/*${function:end}*/
