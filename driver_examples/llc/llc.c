/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Portable Last Level Cache (LLC) driver example.
 *
 * This source is memory-type agnostic: it never references PSRAM, SDRAM,
 * HyperRAM or any specific external-memory technology, and it never hard-codes
 * an LLC instance or a memory address. Everything platform-specific is supplied
 * at run time by the board port through the contract in app.h
 * (llc_example_platform_t + LLC_ExampleGetPlatform / LLC_ExamplePlatformInit).
 *
 * The example demonstrates, using only public LLC driver APIs:
 *   - reading and printing the LLC version, capabilities and geometry
 *   - ensuring the LLC is enabled before the cacheable region is accessed
 *   - generating cacheable traffic against a board-provided memory region
 *   - clean/invalidate maintenance over an address range and the whole cache
 *   - configuring, running and reading the LLC performance monitor
 *   - checking every API return value and printing a final PASS / FAIL.
 */

#include <string.h>

#include "app.h"
#include "board.h"
#include "llc_example_platform.h"
#include "fsl_llc.h"
#include "fsl_debug_console.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Number of cache lines the example actively exercises. Kept small so the
 * working set stays well inside any board's region. */
#define LLC_EXAMPLE_WORK_LINES 32U

/* Performance monitor window, in LLC cycles (must be a multiple of 256). */
#define LLC_EXAMPLE_PERF_DURATION 1024U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static uint16_t LLC_ExampleLineBytes(const llc_feature_capability_t *cap);
static void LLC_ExamplePrintInfo(const llc_example_platform_t *plat, uint16_t lineBytes);
static bool LLC_ExampleEnsureEnabled(const llc_example_platform_t *plat);
static uint32_t LLC_ExampleGenerateTraffic(const llc_example_platform_t *plat, uint16_t lineBytes, uint32_t lines);
static bool LLC_ExampleDemonstrateMaintenance(const llc_example_platform_t *plat, uint16_t lineBytes);
static bool LLC_ExampleRunPerformanceMonitor(const llc_example_platform_t *plat, uint16_t lineBytes);

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Translate the capability enum into a byte count. */
static uint16_t LLC_ExampleLineBytes(const llc_feature_capability_t *cap)
{
    uint16_t bytes;

    switch (cap->cacheLineSize)
    {
        case kLLC_CacheLineSize128B:
            bytes = 128U;
            break;
        default:
            bytes = 0U;
            break;
    }

    return bytes;
}

/* Print LLC version, capabilities and geometry. */
static void LLC_ExamplePrintInfo(const llc_example_platform_t *plat, uint16_t lineBytes)
{
    llc_version_info_t version;
    llc_feature_capability_t cap;
    uint32_t ways     = (uint32_t)LLC_WAY_COUNT;
    uint32_t sets     = (uint32_t)LLC_SET_COUNT;
    uint32_t capacity = ways * sets * (uint32_t)lineBytes;

    LLC_GetVersion(plat->instance, &version);
    LLC_GetCapabilities(plat->instance, &cap);

    PRINTF("LLC driver version : %d.%d.%d\r\n", version.major, version.minor, version.patch);
    PRINTF("Cache line size    : %d bytes\r\n", lineBytes);
    PRINTF("Ways               : %d\r\n", ways);
    PRINTF("Sets               : %d\r\n", sets);
    PRINTF("Total capacity     : %d bytes\r\n", capacity);
    PRINTF("Free-run capable   : %s\r\n", cap.freeRun ? "yes" : "no");
    PRINTF("Perf counter width : %d bits\r\n", cap.counterWidth);
    PRINTF("Test region base   : 0x%08X\r\n", plat->regionBase);
    PRINTF("Test region size   : %d bytes\r\n", plat->regionSize);
}

/*
 * Ensure the LLC is enabled before the cacheable region is accessed.
 *
 * If the boot code already initialized the LLC (plat->bootInitedLlc == true),
 * the driver must NOT be re-initialized: re-running LLC_Init on a live cache
 * would disturb valid tags. In that case the cache is already enabled and there
 * is nothing to do. Otherwise the example brings the LLC up with the default
 * configuration and enables it.
 */
static bool LLC_ExampleEnsureEnabled(const llc_example_platform_t *plat)
{
    llc_config_t config;
    status_t status;

    if (plat->bootInitedLlc)
    {
        PRINTF("LLC already initialized by boot code; leaving it untouched.\r\n");
        return true;
    }

    LLC_GetDefaultConfig(&config);

    status = LLC_Init(plat->instance, &config);
    if (status != kStatus_Success)
    {
        PRINTF("LLC_Init failed: %d\r\n", status);
        return false;
    }

    LLC_EnableCache(plat->instance);
    PRINTF("LLC initialized and enabled by the example.\r\n");

    return true;
}

/*
 * Generate cacheable traffic against the board-provided region through its
 * LLC-routed view. Writes then reads back a walking pattern so the LLC sees
 * both cacheable write and cacheable read requests. Returns an accumulator so
 * the compiler cannot optimize the loads away.
 */
static uint32_t LLC_ExampleGenerateTraffic(const llc_example_platform_t *plat, uint16_t lineBytes, uint32_t lines)
{
    volatile uint8_t *region = (volatile uint8_t *)plat->regionBase;
    uint32_t span            = lines * (uint32_t)lineBytes;
    uint32_t acc             = 0U;
    uint32_t i;

    for (i = 0U; i < span; i++)
    {
        region[i] = (uint8_t)(i & 0xFFU);
    }

    for (i = 0U; i < span; i++)
    {
        acc += region[i];
    }

    return acc;
}

/*
 * Demonstrate LLC maintenance. The example writes a known pattern through the
 * cached view, then exercises address-range, by-range and whole-cache
 * clean/invalidate. A correct maintenance op writes any dirty lines back to
 * memory and drops them from the cache, so a subsequent read refills from
 * memory and observes the same pattern. Every return value is checked.
 */
static bool LLC_ExampleDemonstrateMaintenance(const llc_example_platform_t *plat, uint16_t lineBytes)
{
    volatile uint8_t *region = (volatile uint8_t *)plat->regionBase;
    uint32_t lines           = LLC_EXAMPLE_WORK_LINES;
    uint32_t span            = lines * (uint32_t)lineBytes;
    status_t status;
    uint32_t i;
    bool ok = true;

    for (i = 0U; i < span; i++)
    {
        region[i] = (uint8_t)(0xA5U ^ (i & 0xFFU));
    }

    /* Clean+invalidate an exact line range starting at the region base. */
    status = LLC_CleanInvalidateCacheAtAddressRange(plat->instance, plat->regionBase, (uint16_t)lines);
    if (status != kStatus_Success)
    {
        PRINTF("LLC_CleanInvalidateCacheAtAddressRange failed: %d\r\n", status);
        ok = false;
    }

    /* Clean+invalidate the same span expressed in bytes. */
    status = LLC_CleanInvalidateCacheByRange(plat->regionBase, span);
    if (status != kStatus_Success)
    {
        PRINTF("LLC_CleanInvalidateCacheByRange failed: %d\r\n", status);
        ok = false;
    }

    /* Clean+invalidate the whole cache. */
    status = LLC_CleanInvalidateCache(plat->instance);
    if (status != kStatus_Success)
    {
        PRINTF("LLC_CleanInvalidateCache failed: %d\r\n", status);
        ok = false;
    }

    /* After maintenance the data must survive the write-back / refill round
     * trip: read it back through the cached view and confirm the pattern. */
    for (i = 0U; i < span; i++)
    {
        if (region[i] != (uint8_t)(0xA5U ^ (i & 0xFFU)))
        {
            PRINTF("Data mismatch after maintenance at offset %d\r\n", i);
            ok = false;
            break;
        }
    }

    if (ok)
    {
        PRINTF("Range and whole-cache maintenance completed; data preserved.\r\n");
    }

    return ok;
}

/*
 * Configure, run and read the LLC performance monitor while cacheable traffic
 * is generated. Confirms the monitor observed activity (non-zero cycle and
 * read-request counts) and prints the collected counters.
 */
static bool LLC_ExampleRunPerformanceMonitor(const llc_example_platform_t *plat, uint16_t lineBytes)
{
    llc_performance_config_t config;
    llc_performance_counters_t counters;
    status_t status;
    uint32_t acc;
    bool ok = true;

    (void)memset(&config, 0, sizeof(config));
    config.enable          = true;
    config.hardwareTrigger = false;
    config.duration        = LLC_EXAMPLE_PERF_DURATION;
    config.rttThreshold    = 1U;
    config.wttThreshold    = 1U;

    status = LLC_InitPerformanceMonitor(plat->instance, &config);
    if (status != kStatus_Success)
    {
        PRINTF("LLC_InitPerformanceMonitor failed: %d\r\n", status);
        return false;
    }

    LLC_ResetPerformanceCounters(plat->instance);
    LLC_StartPerformanceMonitor(plat->instance);

    acc = LLC_ExampleGenerateTraffic(plat, lineBytes, LLC_EXAMPLE_WORK_LINES);

    LLC_StopPerformanceMonitor(plat->instance);

    status = LLC_GetPerformanceCounters(plat->instance, &counters);
    if (status != kStatus_Success)
    {
        PRINTF("LLC_GetPerformanceCounters failed: %d\r\n", status);
        return false;
    }

    PRINTF("Performance counters:\r\n");
    PRINTF("  cycles         : %d\r\n", counters.cycleCounter);
    PRINTF("  read requests  : %d\r\n", counters.readRequestCounter);
    PRINTF("  write requests : %d\r\n", counters.writeRequestCounter);
    PRINTF("  read hits      : %d\r\n", counters.readHitCounter);
    PRINTF("  write hits     : %d\r\n", counters.writeHitCounter);
    PRINTF("  evictions      : %d\r\n", counters.evictionCounter);

    if ((counters.cycleCounter == 0U) || (counters.readRequestCounter == 0U))
    {
        PRINTF("Performance monitor recorded no activity.\r\n");
        ok = false;
    }

    /* Consume acc so the traffic loop is not optimized out. */
    if (acc == 0xFFFFFFFFU)
    {
        PRINTF("");
    }

    return ok;
}

/*!
 * @brief Main function.
 */
int main(void)
{
    const llc_example_platform_t *plat;
    llc_feature_capability_t cap;
    uint16_t lineBytes;
    bool pass = true;

    BOARD_InitHardware();

    /* Bring up the board-specific memory that backs the LLC test region. */
    LLC_ExamplePlatformInit();
    plat = LLC_ExampleGetPlatform();

    PRINTF("\r\nLLC example start.\r\n");

    LLC_GetCapabilities(plat->instance, &cap);
    lineBytes = LLC_ExampleLineBytes(&cap);
    if (lineBytes == 0U)
    {
        PRINTF("Unsupported cache line size capability.\r\n");
        pass = false;
    }

    if (pass)
    {
        LLC_ExamplePrintInfo(plat, lineBytes);
    }

    if (pass && !LLC_ExampleEnsureEnabled(plat))
    {
        pass = false;
    }

    if (pass)
    {
        (void)LLC_ExampleGenerateTraffic(plat, lineBytes, LLC_EXAMPLE_WORK_LINES);
    }

    if (pass && !LLC_ExampleDemonstrateMaintenance(plat, lineBytes))
    {
        pass = false;
    }

    if (pass && !LLC_ExampleRunPerformanceMonitor(plat, lineBytes))
    {
        pass = false;
    }

    if (pass)
    {
        PRINTF("\r\nLLC example PASS.\r\n");
    }
    else
    {
        PRINTF("\r\nLLC example FAIL.\r\n");
    }

    while (1)
    {
    }
}
