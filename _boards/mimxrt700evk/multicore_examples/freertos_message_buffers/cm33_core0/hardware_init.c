/*
 * Copyright 2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "mcmgr.h"
#include "fsl_cache.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* The board's BOARD_ConfigMPU() configures memory attributes such that attribute
 * index 1 is "non-cacheable".
 */
#define APP_RPMSG_SHMEM_MPU_REGION_INDEX (5U)
#define APP_MPU_ATTR_INDEX_NONCACHEABLE  (1U)

static bool APP_GetRpmsgShmemRange(uint32_t *start, uint32_t *end_inclusive)
{
#if defined(__ICCARM__)
    /* IAR: linker exports absolute symbols. */
    extern unsigned char rpmsg_sh_mem_start[];
    extern unsigned char rpmsg_sh_mem_end[];
    *start         = (uint32_t)(uintptr_t)rpmsg_sh_mem_start;
    *end_inclusive = (uint32_t)(uintptr_t)rpmsg_sh_mem_end;
    return (*end_inclusive > *start);

#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
    /* Keil/MDK: use Base/Limit symbols from the scatter file.
     *
     * IMPORTANT: Do not take the address of the $$Length symbol here. In the
     * Arm linker, $$Length is a value-like symbol and &$$Length is not the
     * length. Base/Limit are reliable.
     */
    extern unsigned char Image$$RPMSG_SH_MEM$$Base;
    extern unsigned char Image$$RPMSG_SH_MEM$$Limit;

    const uint32_t base  = (uint32_t)(uintptr_t)&Image$$RPMSG_SH_MEM$$Base;
    const uint32_t limit = (uint32_t)(uintptr_t)&Image$$RPMSG_SH_MEM$$Limit;

    if (limit <= base)
    {
        return false;
    }

    *start         = base;
    *end_inclusive = limit - 1U;
    return true;

#elif defined(__GNUC__)
    /* Arm GCC / MCUXpressoIDE:
     *
     * There are multiple linker scripts in use across SDK + IDE projects.
     * Try a few common symbol pairs in order.
     */
    extern uint32_t __RPMSG_SH_MEM_START__ __attribute__((weak));
    extern uint32_t __RPMSG_SH_MEM_END__ __attribute__((weak));

    /* MCUXpressoIDE generated linker script (NOINIT/BSS/DATA sections for rpmsg_sh_mem). */
    extern uint32_t __start_noinit_rpmsg_sh_mem __attribute__((weak));
    extern uint32_t __end_noinit_rpmsg_sh_mem __attribute__((weak));
    extern uint32_t __start_bss_rpmsg_sh_mem __attribute__((weak));
    extern uint32_t __end_bss_rpmsg_sh_mem __attribute__((weak));
    extern uint32_t __start_data_rpmsg_sh_mem __attribute__((weak));
    extern uint32_t __end_data_rpmsg_sh_mem __attribute__((weak));

    uint32_t s = 0U;
    uint32_t e = 0U;

    /* 1) SDK device linker scripts */
    s = (uint32_t)(uintptr_t)&__RPMSG_SH_MEM_START__;
    e = (uint32_t)(uintptr_t)&__RPMSG_SH_MEM_END__;
    if ((s != 0U) && (e > s))
    {
        *start         = s;
        *end_inclusive = e - 1U;
        return true;
    }

    /* 2) MCUXpressoIDE script: noinit section for rpmsg_sh_mem */
    s = (uint32_t)(uintptr_t)&__start_noinit_rpmsg_sh_mem;
    e = (uint32_t)(uintptr_t)&__end_noinit_rpmsg_sh_mem;
    if ((s != 0U) && (e > s))
    {
        *start         = s;
        *end_inclusive = e - 1U;
        return true;
    }

    /* 3) MCUXpressoIDE script: bss section for rpmsg_sh_mem */
    s = (uint32_t)(uintptr_t)&__start_bss_rpmsg_sh_mem;
    e = (uint32_t)(uintptr_t)&__end_bss_rpmsg_sh_mem;
    if ((s != 0U) && (e > s))
    {
        *start         = s;
        *end_inclusive = e - 1U;
        return true;
    }

    /* 4) MCUXpressoIDE script: data section for rpmsg_sh_mem */
    s = (uint32_t)(uintptr_t)&__start_data_rpmsg_sh_mem;
    e = (uint32_t)(uintptr_t)&__end_data_rpmsg_sh_mem;
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

static void APP_ConfigRpmsgShmemNonCacheableMpuRegion(void)
{
    uint32_t start;
    uint32_t end;

    if (!APP_GetRpmsgShmemRange(&start, &end))
    {
        return;
    }

    /* Make sure no dirty cache lines remain for this range before switching it
     * to non-cacheable.
     */
    XCACHE_CleanInvalidateCacheByRange(start, (end - start) + 1U);

    /* Add an MPU region for rpmsg shared memory (used by this example's
     * FreeRTOS message buffers) to make it non-cacheable.
     */
    ARM_MPU_Disable();

    /* Outer-shareable, RW, executable (same convention as other SRAM regions).
     * Attribute index 1 == non-cacheable.
     */
    ARM_MPU_SetRegion(APP_RPMSG_SHMEM_MPU_REGION_INDEX,
                      ARM_MPU_RBAR(start, ARM_MPU_SH_OUTER, 0U, 1U, 0U),
                      ARM_MPU_RLAR(end, APP_MPU_ATTR_INDEX_NONCACHEABLE));

    /* Re-enable MPU with the same control flags as BOARD_ConfigMPU(). */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);

    __DSB();
    __ISB();
}

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();

    /* Ensure rpmsg shared memory used by inter-core message buffers is
     * non-cacheable.
     */
    APP_ConfigRpmsgShmemNonCacheableMpuRegion();

    BOARD_InitPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();

    BOARD_InitAHBSC();

    /* Powerup all the SRAM partitions. */
    PMC0->PDRUNCFG2 &= ~0x3FFC0000;
    PMC0->PDRUNCFG3 &= ~0x3FFC0000;

    POWER_DisablePD(kPDRUNCFG_SHUT_SENSEP_MAINCLK);
    POWER_ApplyPD();

    RESET_ClearPeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio0);
}

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)__section_begin("__core1_image");
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif /* CORE1_IMAGE_COPY_TO_RAM */

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
void invalidate_cache_for_core1_image_memory(uint32_t address, uint32_t size_byte)
{
    XCACHE_CleanInvalidateCacheByRange(address, size_byte);
}
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY */

/*${function:end}*/
