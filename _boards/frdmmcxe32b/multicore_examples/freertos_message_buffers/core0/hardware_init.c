/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* MPU region index for the rpmsg shared memory non-cacheable override.
 * BOARD_ConfigMPU() programs regions 0-14; region 15 is free for the
 * application.  On ARMv7-M a higher-numbered region wins on overlap, so
 * region 15 overrides region 7 (cacheable 512 KB SRAM) for this 8 KB window.
 */
#define APP_RPMSG_SHMEM_MPU_REGION_INDEX (15U)

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Resolve the rpmsg shared-memory range from toolchain-specific linker symbols.
 * Returns true and fills start / end_inclusive (byte addresses) when the
 * section is present (i.e. __use_shmem__ was defined at link time).
 */
static bool APP_GetRpmsgShmemRange(uint32_t *start, uint32_t *end_inclusive)
{
#if defined(__ICCARM__)
    /* IAR linker exports absolute symbols. */
    extern unsigned char rpmsg_sh_mem_start[];
    extern unsigned char rpmsg_sh_mem_end[];
    *start         = (uint32_t)(uintptr_t)rpmsg_sh_mem_start;
    *end_inclusive = (uint32_t)(uintptr_t)rpmsg_sh_mem_end;
    return (*end_inclusive > *start);

#elif defined(__ARMCC_VERSION)
    /* Keil MDK / ARM Compiler: the scatter file places the shared section in
     * the RPMSG_SH_MEM execution region; the linker exports its base address
     * and byte length as Image$$RPMSG_SH_MEM$$Base / $$Length.
     */
    extern unsigned char Image$$RPMSG_SH_MEM$$Base[];
    extern unsigned char Image$$RPMSG_SH_MEM$$Length[];
    uint32_t s   = (uint32_t)(uintptr_t)Image$$RPMSG_SH_MEM$$Base;
    uint32_t len = (uint32_t)(uintptr_t)Image$$RPMSG_SH_MEM$$Length;
    if (len > 0U)
    {
        *start         = s;
        *end_inclusive = s + len - 1U;
        return true;
    }
    return false;

#elif defined(__GNUC__)
    /* GCC/armgcc: SDK device linker script exports these weak symbols from the
     * .noinit_rpmsg_sh_mem NOLOAD section (MCXE32B_cm7_core0_flash.ld).
     * __RPMSG_SH_MEM_END__ is the address one byte past the end of the section.
     */
    extern uint32_t __RPMSG_SH_MEM_START__ __attribute__((weak));
    extern uint32_t __RPMSG_SH_MEM_END__ __attribute__((weak));

    uint32_t s = (uint32_t)(uintptr_t)&__RPMSG_SH_MEM_START__;
    uint32_t e = (uint32_t)(uintptr_t)&__RPMSG_SH_MEM_END__;

    if ((s != 0U) && (e > s))
    {
        *start         = s;
        *end_inclusive = e - 1U;
        return true;
    }
    return false;

#else
    return false;
#endif
}

/* Make the rpmsg shared-memory region non-cacheable by programming a dedicated
 * MPU region that overrides the cacheable SRAM mapping set up by
 * BOARD_ConfigMPU().
 *
 * BOARD_ConfigMPU() maps 0x20400000 as Normal, outer/inner write-back cacheable
 * (region 7).  The FreeRTOS message-buffer demo passes data through this window
 * between the two Cortex-M7 cores but performs no cache maintenance; without a
 * non-cacheable mapping the reading core sees stale data sitting in the writing
 * core's D-cache.
 *
 * The rpmsg shared section is 0x1800 bytes; 8 KB is the smallest power-of-two
 * MPU region size that covers it, and 0x20400000 is naturally 8 KB-aligned.
 * Region 15 (highest priority on ARMv7-M) is used so the override takes
 * precedence over region 7 without altering the board-level MPU setup.
 */
static void APP_ConfigRpmsgShmemNonCacheableMpuRegion(void)
{
    uint32_t start;
    uint32_t end;

    if (!APP_GetRpmsgShmemRange(&start, &end))
    {
        /* Shared-memory section not present in this build (__use_shmem__ not set). */
        return;
    }

    /* Flush and invalidate any dirty D-cache lines for the range before the
     * memory-type attribute changes.
     */
    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)start, (int32_t)((end - start) + 1U));

    ARM_MPU_Disable();

    /* Normal memory, not shareable, outer/inner non-cacheable (TEX=1, S=0, C=0, B=0).
     * Full privileged/unprivileged RW access, instruction fetch enabled.
     * Region size 8 KB covers the entire 0x1800-byte shared section.
     */
    MPU->RBAR = ARM_MPU_RBAR(APP_RPMSG_SHMEM_MPU_REGION_INDEX, start);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_8KB);

    /* Re-enable MPU with the same control flags as BOARD_ConfigMPU(). */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);

    __DSB();
    __ISB();
}

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();

    /* Make the rpmsg shared memory non-cacheable so the FreeRTOS inter-core
     * message buffers remain coherent without explicit cache maintenance in
     * the demo code.
     */
    APP_ConfigRpmsgShmemNonCacheableMpuRegion();

    BOARD_InitBootClocks();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
