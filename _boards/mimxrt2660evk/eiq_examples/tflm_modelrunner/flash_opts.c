/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * flash_opts.c -- modelrunner flash abstraction for RT2660 PSRAM.
 *
 * The "flash" target on RT2660 is the 32 MB APS256XXN PSRAM mapped at
 * EXAMPLE_XSPI_AMBA_BASE (0x88000000).  The PSRAM is mapped by the boot
 * configuration and is used as plain memory -- there is no example-side
 * controller init and no dedicated read/write API (same approach as
 * tflm_cifar10 on this board).  PSRAM does not require erase before write,
 * so FlashErase() is a no-op and FlashProgram() is a plain memcpy.
 */

#include "flash_opts.h"
#include "fsl_debug_console.h"
#include "fsl_llc.h"
#include "timer.h"

#include <string.h>
#include <stdint.h>

/* Satisfy the armclang thread-pointer ABI stub when building with MDK. */
#if defined(__ARMCC_VERSION)
__attribute__((weak)) size_t __aeabi_read_tp(void)
{
    return 0;
}
#endif

/*
 * FlashInit -- PSRAM is mapped by the boot configuration; nothing to do.
 */
status_t FlashInit(FlashConfig *config)
{
    (void)config;
    return kStatus_Success;
}

/*
 * FlashErase -- PSRAM has no erase granularity; always succeeds.
 * Prints the range so the modelrunner log shows progress.
 */
status_t FlashErase(FlashConfig *config, uint32_t start, uint32_t length)
{
    (void)config;
    /* Reject writes beyond the dedicated model storage range. */
    if (start + length > FLASH_MODEL_STORAGE_SIZE) {
        PRINTF("ERROR: model too large for PSRAM storage (%u > %u)\r\n",
               start + length, FLASH_MODEL_STORAGE_SIZE);
        return kStatus_Fail;
    }
    PRINTF("PSRAM erase (no-op) @ 0x%08X len %u\r\n",
           FLASH_BASE_ADDR + start, length);
    return kStatus_Success;
}

/*
 * FlashProgram -- write 'length' bytes from 'src' to PSRAM offset 'start'.
 * The model storage range is cacheable, so the written range is cleaned
 * from L1 and the LLC before the NPU reads it.
 */
status_t FlashProgram(FlashConfig *config, uint32_t start, uint32_t *src, uint32_t length)
{
    (void)config;
    memcpy((void *)(uintptr_t)(FLASH_BASE_ADDR + start), src, length);
    cleanCache_by_Addr(FLASH_BASE_ADDR + start, length);
    (void)LLC_CleanInvalidateCache(CMPT__LLC);
    /* Trailing "==" is the per-chunk ack expected by the host script. */
    PRINTF("PSRAM program @ 0x%08X: %u bytes  ==\r\n",
           FLASH_BASE_ADDR + start, length);
    return kStatus_Success;
}

