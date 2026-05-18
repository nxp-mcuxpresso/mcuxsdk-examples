/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "fsl_sramctl.h"

#define SRAMCTL_MAGIC_NUMBER              (0xA5A55A5AU)
#define SRAMCTL_INJECT_MULTI_DATABIT_MASK (0x00000003u) /* flip 2 bits -> uncorrectable */

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


static bool SRAMCTL_RunMultiBitUncorrectableTest(void)
{
    bool errorFlag = false;

    uint32_t *const ramAddr = (uint32_t *)(uintptr_t)APP_SRAMCTL_TEST_ADDR;

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
    dataCfg.ramErrorInjectionData0         = SRAMCTL_INJECT_MULTI_DATABIT_MASK;
    dataCfg.ramErrorInjectionData1         = 0u;
    SRAMCTL_ConfigErrorInjectionData(SRAMCTL_BASE, &dataCfg);

    PRINTF("\r\nSRAMCTL multi-bit injection test\r\n");
    PRINTF("Target address : 0x%08X\r\n", (uint32_t)(uintptr_t)ramAddr);
    PRINTF("Inject mask    : 0x%08X (flip 2 data bits)\r\n", (unsigned int)SRAMCTL_INJECT_MULTI_DATABIT_MASK);
    PRINTF("NOTE: multi-bit ECC is uncorrectable; some targets may reset/halt here.\r\n");

    /* Trigger read: may set MLTERR or may escalate to fault/reset depending on SoC integration. */
    volatile uint32_t readValue = *ramAddr;
    (void)readValue;

#if defined(SRAMCTL_USE_INTERRUPT) && (SRAMCTL_USE_INTERRUPT)
    while (!s_sramctlEventHappened)
    {
        __NOP();
    }

    DisableIRQ(SRAMCTL_IRQ_ID);
#else
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

    uint32_t ramsr = s_sramctlLatchedRamsr;
    if (ramsr == 0U)
    {
        ramsr = SRAMCTL_GetStatusFlags(SRAMCTL_BASE);
    }
    sramctl_error_info_t info;
    SRAMCTL_GetErrorInfo(SRAMCTL_BASE, &info);

    if (!s_sramctlEventHappened)
    {
        errorFlag = true;
        PRINTF("FAIL: timeout waiting for SRAMCTL event\r\n");
    }

    PRINTF("SRAMCTL status: 0x%08X | SYND=0x%02X EINFO=0x%02X ECCNT=%u\r\n", (unsigned int)ramsr,
           info.eccSyndromeValue, info.ramStatusEventInfo, info.ramCorrectableErrorCount);

    if ((ramsr & (uint32_t)kSRAMCTL_EccMultiBitError) != 0U)
    {
        PRINTF("PASS: uncorrectable multi-bit error was detected (MLTERR=1)\r\n");
    }
    else
    {
        errorFlag = true;
        PRINTF("FAIL: expect uncorrectable multi-bit event (MLTERR=1)\r\n");
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

    bool fail = SRAMCTL_RunMultiBitUncorrectableTest();

    if (fail)
    {
        PRINTF("\r\nSRAMCTL multi-bit example finished with error.\r\n");
    }
    else
    {
        PRINTF("\r\nSRAMCTL multi-bit example finished successfully.\r\n");
    }

    while (1)
    {
        __NOP();
    }
}
