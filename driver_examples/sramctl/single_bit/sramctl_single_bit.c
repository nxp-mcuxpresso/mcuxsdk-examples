/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "fsl_sramctl.h"

#define SRAMCTL_MAGIC_NUMBER               (0xA5A55A5AU)
#define SRAMCTL_INJECT_SINGLE_DATABIT_MASK (0x00000001u)

#define SRAMCTL_EVENT_TIMEOUT_LOOP (10000000u)

static volatile bool s_sramctlEventHappened;
static volatile uint32_t s_sramctlLatchedRamsr;

#if defined(SRAMCTL_USE_INTERRUPT) && (SRAMCTL_USE_INTERRUPT)
void SRAMCTL_IRQ_HANDLER(void)
{
    uint32_t status = SRAMCTL_GetStatusFlags(SRAMCTL_BASE);

    s_sramctlLatchedRamsr   = status;
    s_sramctlEventHappened  = true;

    /* Clear W1C bits to allow subsequent events. */
    (void)SRAMCTL_ClearStatusFlags(SRAMCTL_BASE, status & (uint32_t)kSRAMCTL_AllW1CFlags);

    SDK_ISR_EXIT_BARRIER;
}
#endif

static bool SRAMCTL_RunSingleBitCorrectionTest(void)
{
    bool errorFlag = false;

    uint32_t *const ramAddr = (uint32_t *)(uintptr_t)APP_SRAMCTL_TEST_ADDR;
    uint32_t readValue;
    sramctl_error_info_t info;

    *ramAddr = SRAMCTL_MAGIC_NUMBER;

    s_sramctlEventHappened = false;
    s_sramctlLatchedRamsr  = 0U;

    (void)SRAMCTL_ClearStatusFlags(SRAMCTL_BASE, (uint32_t)kSRAMCTL_AllW1CFlags);

#if defined(SRAMCTL_USE_INTERRUPT) && (SRAMCTL_USE_INTERRUPT)
    EnableIRQ(SRAMCTL_IRQ_ID);
#endif

    sramctl_error_injection_address_t addrCfg;
    addrCfg.ramErrorInjectionAddressEnable = true;
    addrCfg.ramErrorInjectionBaseAddress   = (uint32_t)(uintptr_t)ramAddr;
    addrCfg.ramErrorInjectionAddressMask   = 0xFFFFFFFFu;
    SRAMCTL_ConfigErrorInjectionAddress(SRAMCTL_BASE, &addrCfg);

    sramctl_error_injection_data_t dataCfg;
    dataCfg.ramErrorInjectionDataEnable    = true;
    dataCfg.ramErrorInjectionDataCheckbits = 0u;
    dataCfg.ramErrorInjectionData0         = SRAMCTL_INJECT_SINGLE_DATABIT_MASK;
    dataCfg.ramErrorInjectionData1         = 0u;
    SRAMCTL_ConfigErrorInjectionData(SRAMCTL_BASE, &dataCfg);

    PRINTF("\r\nSRAMCTL single-bit injection test\r\n");
    PRINTF("Target address : 0x%08X\r\n", (uint32_t)(uintptr_t)ramAddr);
    PRINTF("Inject mask    : 0x%08X (flip 1 data bit)\r\n", (unsigned int)SRAMCTL_INJECT_SINGLE_DATABIT_MASK);

    /* Trigger a read to hit the injection address. */
    readValue = *ramAddr;

#if defined(SRAMCTL_USE_INTERRUPT) && (SRAMCTL_USE_INTERRUPT)
    while (!s_sramctlEventHappened)
    {
        __NOP();
    }

    DisableIRQ(SRAMCTL_IRQ_ID);
#else
    /* No NVIC IRQ on this target: wait until status becomes valid (simple timeout loop). */
    for (uint32_t i = 0U; i < SRAMCTL_EVENT_TIMEOUT_LOOP; i++)
    {
        uint32_t ramsr = SRAMCTL_GetStatusFlags(SRAMCTL_BASE);
        if ((ramsr & ((uint32_t)kSRAMCTL_EccSingleBitError | (uint32_t)kSRAMCTL_EccMultiBitError | (uint32_t)kSRAMCTL_BusError | (uint32_t)kSRAMCTL_EccAddressError)) != 0U)
        {
            s_sramctlLatchedRamsr  = ramsr;
            s_sramctlEventHappened = true;
            break;
        }
    }
#endif

    /* Single-bit error is correctable, value returned to CPU may be corrected back to original. */
    if (readValue == SRAMCTL_MAGIC_NUMBER)
    {
        PRINTF("Read value     : 0x%08X (corrected by ECC)\r\n", readValue);
    }
    else
    {
        errorFlag = true;
        PRINTF("Read value     : 0x%08X (unexpected, expect 0x%08X)\r\n", readValue, SRAMCTL_MAGIC_NUMBER);
    }

    if (!s_sramctlEventHappened)
    {
        errorFlag = true;
        PRINTF("FAIL: timeout waiting for SRAMCTL event\r\n");
    }

    uint32_t ramsr = s_sramctlLatchedRamsr;
    if (ramsr == 0U)
    {
        ramsr = SRAMCTL_GetStatusFlags(SRAMCTL_BASE);
    }

    SRAMCTL_GetErrorInfo(SRAMCTL_BASE, &info);

    PRINTF("SRAMCTL status: 0x%08X | SYND=0x%02X EINFO=0x%02X ECCNT=%u\r\n", (unsigned int)ramsr,
           info.eccSyndromeValue, info.ramStatusEventInfo, info.ramCorrectableErrorCount);

    if (((ramsr & (uint32_t)kSRAMCTL_EccSingleBitError) == 0U) || (info.ramCorrectableErrorCount == 0U))
    {
        errorFlag = true;
        PRINTF("FAIL: expect correctable single-bit event (SGLERR=1 and ECCNT>0)\r\n");
    }
    else
    {
        PRINTF("PASS: correctable single-bit error was detected and corrected (ECCNT=%u)\r\n",
               info.ramCorrectableErrorCount);
    }

    /* Disable injection. */
    dataCfg.ramErrorInjectionDataEnable = false;
    SRAMCTL_ConfigErrorInjectionData(SRAMCTL_BASE, &dataCfg);
    addrCfg.ramErrorInjectionAddressEnable = false;
    SRAMCTL_ConfigErrorInjectionAddress(SRAMCTL_BASE, &addrCfg);

    (void)SRAMCTL_ClearStatusFlags(SRAMCTL_BASE, (uint32_t)kSRAMCTL_AllW1CFlags);

    return errorFlag;
}

int main(void)
{
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    PRINTF("SRAMCTL base address: 0x%08X\r\n", (uint32_t)SRAMCTL_BASE);

    sramctl_config_t config;
    config.addressType              = (uint8_t)kSRAMCTL_AddressTypeSystem;
    config.initializationWaitStates = kSRAMCTL_ZeroWaitCycle;
    config.addressStart             = APP_SRAMCTL_INIT_ADDR_START;
    config.addressEnd               = APP_SRAMCTL_INIT_ADDR_END;

    PRINTF("Init range: 0x%08X - 0x%08X\r\n", config.addressStart, config.addressEnd);

    status_t st = SRAMCTL_Init(SRAMCTL_BASE, &config);
    if (st != kStatus_Success)
    {
        PRINTF("SRAMCTL_Init failed: %d\r\n", (int)st);
        while (1)
        {
        }
    }

    (void)SRAMCTL_ClearStatusFlags(SRAMCTL_BASE, (uint32_t)kSRAMCTL_InitializationDone);

    bool fail = SRAMCTL_RunSingleBitCorrectionTest();

    if (fail)
    {
        PRINTF("\r\nSRAMCTL single-bit example finished with error.\r\n");
    }
    else
    {
        PRINTF("\r\nSRAMCTL single-bit example finished successfully.\r\n");
    }

    while (1)
    {
        __NOP();
    }
}
