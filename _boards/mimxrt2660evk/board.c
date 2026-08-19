/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_power.h"
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_lpi2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */
#include "fsl_iomuxc.h"
#include "pin_mux.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(__ARMCC_VERSION)
__attribute__((section("InRoot$$Sections"), __noinline__))
#elif defined(__GNUC__)
__attribute__((used, section(".text.startup")))
#elif defined(__ICCARM__)
__attribute__((section(".text.startup")))
#endif
void BOARD_EarlyConfigMisc(void)
{
    /* Enable TCM (bank 0) via SYSCON POWERCON. GPR_COLD[0] bit 0 is the
     * cold-reset-latched TCM enable strap read by the SoC power/reset logic;
     * set it before any code needs TCM behind the CPU. */
    SYSCON__POWERCON_SOC_CTRL->GPR_COLD[0] |= 1U;

    // Enable I cache to speed up startup
    __DSB();
    __ISB();
    SCB->ICIALLU = 0U;
    __DSB();
    __ISB();
    SCB->CCR |= SCB_CCR_IC_Msk;
    __DSB();
    __ISB();
}

#if defined(__ARMCC_VERSION)
__attribute__((section("InRoot$$Sections"), __noinline__))
#elif defined(__GNUC__)
__attribute__((used, section(".text.startup")))
#elif defined(__ICCARM__)
__attribute__((section(".text.startup")))
#endif
void BOARD_EarlyConfigLLC(void)
{
    LLC_Type *llc = CMPT__LLC;

    /* Skip if LLC is already up (LOOKUPEN or FILLEN already set). ROM /
     * BootROM / prior boot stage may have initialized LLC; re-running the
     * init sequence on a live LLC risks disturbing valid tags in-flight. */
    if ((llc->CCUCTCR & (LLC_CCUCTCR_LOOKUPEN_MASK | LLC_CCUCTCR_FILLEN_MASK)) != 0U) {
        return;
    }

    /* Step 1: "Initialize valid entries" on Tag Array (ARRAYID=0, MNTOP=0).
     *   1a. Select Tag Array (ARRAYID=0 in CCUCMCR).
     *   1b. Ensure every way is enabled in CCUCMWVR. The default is 0xFF
     *       (all 8 ways) per RM 22.7.17, but writing it explicitly is
     *       REQUIRED -- empirically the tag invalidate misses ways unless
     *       CCUCMWVR is written first, even though its value doesn't
     *       change. Without this write, LLC returns 0 on cached Flash reads. */
    llc->CCUCMWVR = 0xFFU;
    /*   1c. Initiate the op (MNTOP=0 = Initialize valid entries).
     *   1d. Poll until CCUCMAR[MNTOPACTV] clears. */
    llc->CCUCMCR = LLC_CCUCMCR_ARRAYID(0U) | LLC_CCUCMCR_MNTOP(0U);
    while ((llc->CCUCMAR & LLC_CCUCMAR_MNTOPACTV_MASK) != 0U) { }

    /* Step 2: "Initialize all entries" on Data Array (ARRAYID=1, MNTOP=0).
     * Data-array init ignores CCUCMWVR and clears all ways. */
    llc->CCUCMCR = LLC_CCUCMCR_ARRAYID(1U) | LLC_CCUCMCR_MNTOP(0U);
    while ((llc->CCUCMAR & LLC_CCUCMAR_MNTOPACTV_MASK) != 0U) { }

    /* Step 3: Skip (no scratchpad).
     * Step 4: Enable LLC lookups. */
    llc->CCUCTCR = LLC_CCUCTCR_LOOKUPEN(1U);
    /* Step 5: Enable LLC fills. */
    llc->CCUCTCR = LLC_CCUCTCR_LOOKUPEN(1U) | LLC_CCUCTCR_FILLEN(1U);

    /* Step 6: Enable LLC error detection (both bits per RM). */
    llc->CCUUEDR = LLC_CCUUEDR_PROTERRDETEN(1U) | LLC_CCUUEDR_MEMERRDETEN(1U);

    /* Step 7: Enable LLC partial write allocation. */
    llc->CCUCAOR = LLC_CCUCAOR_WRALLOCPARTIALEN(1U);
}

/*
 * Invoked from SystemInit before SystemInitHook. Overrides the weak
 * default in devices/RT/RT2660/MIMXRT266x/system_MIMXRT266x.c. */
#if defined(__ARMCC_VERSION)
__attribute__((section("InRoot$$Sections"), __noinline__))
#elif defined(__GNUC__)
__attribute__((used, section(".text.startup")))
#elif defined(__ICCARM__)
__attribute__((section(".text.startup")))
#endif
void BOARD_EarlyInit(void)
{
    BOARD_EarlyConfigMisc();
    BOARD_EarlyConfigLLC();
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, BOARD_DEBUG_UART_CLK_FREQ);
}

void BOARD_RequestTRDC(void)
{
    S3MU_Type *const mu = MAIN__SENTMU0_SENTMUA;
    uint32_t response[RELEASE_RDC_SIZE];

    /* Send Release-RDC message. Wait for TSR[i]=1 (TR[i] empty) before each
     * write. Payload codes come from board.h. */
    while ((mu->TSR & S3MU_TSR_TEn(1U << 0)) == 0U) { }
    mu->TR[0] = RELEASE_RDC;
    while ((mu->TSR & S3MU_TSR_TEn(1U << 1)) == 0U) { }
    mu->TR[1] = ((uint32_t)ALL_RDC << SHIFT_8) | 0x1U;

    /* Wait for RSR[i]=1 (RR[i] full) and drain the ELE response. */
    while ((mu->RSR & S3MU_RSR_RFn(1U << 0)) == 0U) { }
    response[0] = mu->RR[0];
    while ((mu->RSR & S3MU_RSR_RFn(1U << 1)) == 0U) { }
    response[1] = mu->RR[1];

    /* Verify the ELE accepted the request. A rejection here means the RDC
     * cannot be released and downstream masters would fault on access --
     * trap so the failure is visible on the debugger. */
    if ((response[0] != RELEASE_RDC_RESPONSE_HDR) ||
        ((response[1] != RESPONSE_SUCCESS) && (response[1] != RESPONSE_ALREADY_GRANTED)))
    {
        while (1) { __NOP(); }
    }
}

void BOARD_ConfigTRDC(void)
{
    BOARD_RequestTRDC();

#if defined(BOARD_TRDC_ALL_MASTER_TO_PREVELEGE_DOMAIN) && (BOARD_TRDC_ALL_MASTER_TO_PREVELEGE_DOMAIN == 1)
    /* Exhaustive: assign every TRDC master (MAIN / CMPT / WAKE / AUDIO /
     * COMM / MEDIA) to domain 0 so all initiator transfers see the same
     * access-control view as the CPU. Upper bounds are the last enumerator
     * in each block of trdc_master_t (fsl_trdc_soc.h); ranges are contiguous. */
    for (uint32_t i = 0U; i <= (uint32_t)kTRDC_MAIN_MasterTESTPORT; i++)
    {
        /* Skip reserved MAIN master slots (24, 25, 28, 29, 31). */
        if (i == (uint32_t)kTRDC_MAIN_MasterReserved0 ||
            i == (uint32_t)kTRDC_MAIN_MasterReserved1 ||
            i == (uint32_t)kTRDC_MAIN_MasterReserved2 ||
            i == (uint32_t)kTRDC_MAIN_MasterReserved3 ||
            i == (uint32_t)kTRDC_MAIN_MasterReserved4)
        {
            continue;
        }
        MAIN__TRDC->MDA_DFMT1[i].MDA_W_DFMT1[0] =
            TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    }
    /* Skip kTRDC_CMPT_MasterCPU0_AXIM (0), kTRDC_CMPT_MasterCPU0_AHBP (1)
     * (already in DID 0) and kTRDC_CMPT_MasterReserved0/1 (2, 3). */
    for (uint32_t i = (uint32_t)kTRDC_CMPT_MasterLLC_RD;
         i <= (uint32_t)kTRDC_CMPT_MasterNPU; i++)
    {
        CMPT__TRDC->MDA_DFMT1[i].MDA_W_DFMT1[0] =
            TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    }
    for (uint32_t i = 0U; i <= (uint32_t)kTRDC_WAKE_MasterWEDMA3Ch0_4; i++)
    {
        WAKE__TRDC->MDA_DFMT1[i].MDA_W_DFMT1[0] =
            TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    }
    for (uint32_t i = 0U; i <= (uint32_t)kTRDC_AUDIO_MasterAEDMA3Ch0_4; i++)
    {
        AUDIO__TRDC->MDA_DFMT1[i].MDA_W_DFMT1[0] =
            TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    }
    for (uint32_t i = 0U; i <= (uint32_t)kTRDC_COMM_MasterXSPI_RESP; i++)
    {
        COMM__TRDC->MDA_DFMT1[i].MDA_W_DFMT1[0] =
            TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    }
    for (uint32_t i = 0U; i <= (uint32_t)kTRDC_MEDIA_MasterJPEG; i++)
    {
        MEDIA__TRDC->MDA_DFMT1[i].MDA_W_DFMT1[0] =
            TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    }
#else
    /* CM85 bus ports: explicitly place both in domain 0 (processor
     * masters use the DFMT0 word layout: VLD | DID=0). */
    //CMPT__TRDC->MDA_DFMT1[kTRDC_CMPT_MasterCPU0_AXIM].MDA_W_DFMT1[0] =
    //    TRDC_MDA_W_DFMT0_VLD_MASK;
    //CMPT__TRDC->MDA_DFMT1[kTRDC_CMPT_MasterCPU0_AHBP].MDA_W_DFMT1[0] =
    //    TRDC_MDA_W_DFMT0_VLD_MASK;

    /* Assign MAIN non-processor masters to domain 0 so EDMA5 / MEDMA3
     * initiator transfers see the same access-control view as the CPU. */
    MAIN__TRDC->MDA_DFMT1[kTRDC_MAIN_MasterEDMA5Ch0_1].MDA_W_DFMT1[0]  =
        TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    MAIN__TRDC->MDA_DFMT1[kTRDC_MAIN_MasterMEDMA3Ch0_1].MDA_W_DFMT1[0] =
        TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;

    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterUSB0].MDA_W_DFMT1[0] =
        TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterUSB1].MDA_W_DFMT1[0] =
        TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;

    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterSDHC0].MDA_W_DFMT1[0] =
        TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
#endif
}

/* Disable cache and MPU and clear all MPU regions which is needed before whole MPU reconfiguration. */
void BOARD_ResetMPU(void)
{
    uint32_t i;
    uint32_t region_count = (MPU->TYPE & MPU_TYPE_DREGION_Msk) >> MPU_TYPE_DREGION_Pos;

    /* Disable branch predictor before invalidating the memory map; stale predictions
     * captured under the old MPU regions must not be used once regions change. The
     * caller is responsible for re-enabling SCB_CCR_BP_Msk after the new MPU layout
     * has been installed. */
    SCB->CCR &= ~SCB_CCR_BP_Msk;

    /* Disable I-cache and D-cache. CMSIS wrappers are idempotent so
     * unconditional calls are safe. */
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    SCB_DisableICache();
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    SCB_DisableDCache();
#endif

    /* Disable MPU */
    ARM_MPU_Disable();

    for (i = 0; i < region_count; i++)
    {
        ARM_MPU_ClrRegion(i);
    }
}

/* MPU + cache configuration.
 * Examples needing a different layout call BOARD_ResetMPU() first. */
void BOARD_ConfigMPU(void)
{
    uint32_t ncache_start;
    uint32_t ncache_size;
    uint32_t ncache_end;

#if defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    ncache_start = (uint32_t)__NCACHE_REGION_START;
    ncache_size  = (uint32_t)__NCACHE_REGION_SIZE;
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    extern uint32_t Image$$RW_m_ncache_aux$$Base[];
    ncache_start = (uint32_t)Image$$RW_m_ncache$$Base;
    ncache_size  = (uint32_t)Image$$RW_m_ncache_aux$$Base - ncache_start;
#else
    #error "Unsupported toolchain: need __NCACHE_REGION_START / __NCACHE_REGION_SIZE"
#endif
    ncache_end = ncache_start + ncache_size - 1U;

    /* Disable branch predictor and caches, then disable MPU while remapping.
     * CMSIS SCB_Disable{I,D}Cache() are idempotent so unconditional calls
     * are safe whether or not the caches were already off. */
    SCB->CCR &= ~SCB_CCR_BP_Msk;
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    SCB_DisableICache();
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    SCB_DisableDCache();
#endif
    ARM_MPU_Disable();

    /* -- Memory attribute indices -- */
    uint32_t attr_outer_wb = ARM_MPU_ATTR_MEMORY_(0U, 1U, 1U, 1U);
#if defined(CACHE_MODE_WRITE_THROUGH)
    uint32_t attr_inner_cacheable = ARM_MPU_ATTR_MEMORY_(0U, 0U, 1U, 0U);
#else
    uint32_t attr_inner_cacheable = ARM_MPU_ATTR_MEMORY_(0U, 1U, 1U, 1U);
#endif

    ARM_MPU_SetMemAttr(0U, ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE_nGnRnE));
    ARM_MPU_SetMemAttr(1U, ARM_MPU_ATTR(ARM_MPU_ATTR_NON_CACHEABLE, ARM_MPU_ATTR_NON_CACHEABLE));
    ARM_MPU_SetMemAttr(2U, ARM_MPU_ATTR(attr_outer_wb, attr_inner_cacheable));
    ARM_MPU_SetMemAttr(3U, ARM_MPU_ATTR(attr_outer_wb, ARM_MPU_ATTR_NON_CACHEABLE));

    /* -- Regions -- */
    /* ARM_MPU_RBAR(BASE, SH, RO, NP, XN)  --  RO=1 read-only, NP=1 allow-unpriv, XN=1 no-exec */
    /* ARM_MPU_RLAR(LIMIT, ATTR_IDX) */

    /* R0: Peripheral (AIPS) -- Device, XN. */
    ARM_MPU_SetRegion(0U,
        ARM_MPU_RBAR(0x40000000UL, ARM_MPU_SH_NON, 0U, 1U, 1U),
        ARM_MPU_RLAR(0x5FFFFFFFUL, 0U));

    /* R1: ITCM -- 256K, non-cacheable RW+exec. */
    ARM_MPU_SetRegion(1U,
        ARM_MPU_RBAR(0x00000000UL, ARM_MPU_SH_NON, 0U, 1U, 0U),
        ARM_MPU_RLAR(0x0003FFFFUL, 1U));

    /* R2: DTCM -- 256K, non-cacheable RW, XN. */
    ARM_MPU_SetRegion(2U,
        ARM_MPU_RBAR(0x20000000UL, ARM_MPU_SH_NON, 0U, 1U, 1U),
        ARM_MPU_RLAR(0x2003FFFFUL, 1U));

    /* R3: XSPI0 flash direct (LLC-bypass) window -- 64M, cacheable RO+exec.
     * Currently the active execution path (Flash base = 0x6000_0000). */
    ARM_MPU_SetRegion(3U,
        ARM_MPU_RBAR(0x60000000UL, ARM_MPU_SH_NON, 1U, 1U, 0U),
        ARM_MPU_RLAR(0x63FFFFFFUL, 2U));

    /* R4: XSPI0 flash LLC-cached window -- 64M, cacheable RO+exec.
     * Currently dead (same physical Flash cells as R3); kept for
     * defensive coverage in case future code reaches via this path. */
    ARM_MPU_SetRegion(4U,
        ARM_MPU_RBAR(0x68000000UL, ARM_MPU_SH_NON, 1U, 1U, 0U),
        ARM_MPU_RLAR(0x6BFFFFFFUL, 2U));

    if (ncache_start < 0x80000000UL)
    {
        /* NCACHE lives in SRAM2 tail (ram / xspi_nor targets). */

        /* R5: SRAM cacheable -- SRAM0+SRAM1+SRAM2 head, RW+exec. */
        ARM_MPU_SetRegion(5U,
            ARM_MPU_RBAR(0x22000000UL, ARM_MPU_SH_NON, 0U, 1U, 0U),
            ARM_MPU_RLAR(ncache_start - 1U, 2U));

        /* R6: SRAM NCACHE -- non-cacheable RW, XN. */
        ARM_MPU_SetRegion(6U,
            ARM_MPU_RBAR(ncache_start, ARM_MPU_SH_NON, 0U, 1U, 1U),
            ARM_MPU_RLAR(ncache_end, 1U));
    }
    else
    {
        /* NCACHE lives in PSRAM tail (psram / xspi_nor_psram / psram_txt).
         * SRAM2 is used entirely as heap so it is fully cacheable here.
         * 0x8000_0000 (direct, LLC-bypass) and 0x8800_0000 (LLC-cached) are
         * two independent HW paths -- NOT address aliases -- to the same 32
         * MB PSRAM cells. Linker symbols are 0x8800_0000-based; subtract
         * 0x0800_0000 to get the matching address on the direct path.
         * Applying the same cacheable / NCACHE split to both windows keeps
         * the physical byte's cache attribute consistent no matter which
         * path the CPU uses. */
        uint32_t ncache_start_direct = ncache_start - 0x08000000UL;
        uint32_t ncache_end_direct   = ncache_end   - 0x08000000UL;

        /* R5: SRAM cacheable -- all 768K (SRAM0+SRAM1+SRAM2), RW+exec. */
        ARM_MPU_SetRegion(5U,
            ARM_MPU_RBAR(0x22000000UL, ARM_MPU_SH_NON, 0U, 1U, 0U),
            ARM_MPU_RLAR(0x220BFFFFUL, 2U));

        /* R6: PSRAM direct path (0x80M, LLC-bypass) cacheable head
         * -- Attr2 inner WB / outer WB, RW+exec. */
        ARM_MPU_SetRegion(6U,
            ARM_MPU_RBAR(0x80000000UL, ARM_MPU_SH_NON, 0U, 1U, 0U),
            ARM_MPU_RLAR(ncache_start_direct - 1U, 2U));

        /* R7: PSRAM direct path NCACHE tail -- Attr3 inner NC / outer WB, RW, XN. */
        ARM_MPU_SetRegion(7U,
            ARM_MPU_RBAR(ncache_start_direct, ARM_MPU_SH_NON, 0U, 1U, 1U),
            ARM_MPU_RLAR(ncache_end_direct, 3U));

        /* R8: PSRAM LLC-cached path (0x88M) cacheable head
         * -- Attr2 inner WB / outer WB, RW+exec. */
        ARM_MPU_SetRegion(8U,
            ARM_MPU_RBAR(0x88000000UL, ARM_MPU_SH_NON, 0U, 1U, 0U),
            ARM_MPU_RLAR(ncache_start - 1U, 2U));

        /* R9: PSRAM LLC-cached path NCACHE tail -- Attr3 inner NC / outer WB, RW, XN. */
        ARM_MPU_SetRegion(9U,
            ARM_MPU_RBAR(ncache_start, ARM_MPU_SH_NON, 0U, 1U, 1U),
            ARM_MPU_RLAR(ncache_end, 3U));
    }

    /*
     * Enable MPU with PRIVDEFENA + HFNMIENA:
     *   PRIVDEFENA — privileged access to unmapped ranges falls back to the
     *                default memory map. Required for Cortex-M85 SCS at
     *                0xE000_0000+ and other system-space accesses.
     *   HFNMIENA   — MPU stays active during HardFault / NMI / FAULTMASK
     *                handlers (otherwise memory would be unprotected there).
     */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);

    /* Re-enable branch predictor and both caches now that the MPU map is
     * installed. */
    SCB->CCR |= SCB_CCR_BP_Msk;
    SCB_EnableICache();
    SCB_EnableDCache();
}

/* Common board settings: install the MPU map, apply the default power policy
 * (domain/active-clock-source baseline; low-power gating baseline), bring up the
 * boot clocks, release TRDC ownership from ELE and assign non-CPU masters to
 * domain 0, then mux the debug UART pins and bring up the debug console.
 * Called from BOARD_InitHardware() before user code. Examples that own the
 * debug UART peripheral (LPUART driver examples) or use a non-UART debug
 * console (hello_world_virtual_com over USB CDC) must not call this and
 * should instead inline BOARD_ConfigMPU / BOARD_InitBootClocks / BOARD_ConfigTRDC
 * plus their own console setup. */
void BOARD_CommonSetting(void)
{
    BOARD_ConfigMPU();
    power_policy_config_t powerPolicyCfg;
    POWER_GetDefaultPolicyConfig(&powerPolicyCfg);
    powerPolicyCfg.handshakeRouting = NULL;
    POWER_SetPolicy(&powerPolicyCfg);
    BOARD_InitBootClocks();
    BOARD_ConfigTRDC();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
}

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    lpi2c_master_config_t lpi2cConfig = {0};

    /*
     * lpi2cConfig.debugEnable = false;
     * lpi2cConfig.ignoreAck = false;
     * lpi2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * lpi2cConfig.baudRate_Hz = 100000U;
     * lpi2cConfig.busIdleTimeout_ns = 0;
     * lpi2cConfig.pinLowTimeout_ns = 0;
     * lpi2cConfig.sdaGlitchFilterWidth_ns = 0;
     * lpi2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&lpi2cConfig);
    lpi2cConfig.debugEnable = true;
    LPI2C_MasterInit(base, &lpi2cConfig, clkSrc_Hz);
}

status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subAddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subAddressSize,
                              uint8_t *txBuff,
                              uint8_t txBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 uint8_t *rxBuff,
                                 uint8_t rxBuffSize)
{
    status_t status;
    lpi2c_master_transfer_t xfer;

    xfer.flags          = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = NULL;
    xfer.dataSize       = 0;

    status = LPI2C_MasterTransferBlocking(base, &xfer);

    if (kStatus_Success == status)
    {
        xfer.subaddressSize = 0;
        xfer.direction      = kLPI2C_Read;
        xfer.data           = rxBuff;
        xfer.dataSize       = rxBuffSize;

        status = LPI2C_MasterTransferBlocking(base, &xfer);
    }

    return status;
}

void BOARD_Camera_I2C_Init(void)
{
    CLOCK_EnableClock(kCLOCK_MAIN_hsp_lpi2c0);
    BOARD_LPI2C_Init(BOARD_CAMERA_I2C_BASEADDR, BOARD_CAMERA_I2C_CLOCK_FREQ);
}

status_t BOARD_Camera_I2C_SendSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_SendSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                                txBuffSize);
}

status_t BOARD_Camera_I2C_ReceiveSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_ReceiveSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                                   rxBuffSize);
}

#if defined(BOARD_TOUCH_I2C_BASEADDR)
void BOARD_PanelTouch_I2C_Init(void)
{
    CLOCK_EnableClock(BOARD_TOUCH_I2C_CLOCK);
    BOARD_LPI2C_Init(BOARD_TOUCH_I2C_BASEADDR, BOARD_TOUCH_I2C_CLOCK_FREQ);
}

status_t BOARD_PanelTouch_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_TOUCH_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize,
                          (uint8_t *)txBuff, txBuffSize);
}

status_t BOARD_PanelTouch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_TOUCH_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                             rxBuffSize);
}
#endif /* BOARD_TOUCH_I2C_BASEADDR */

status_t BOARD_I2C_DeviceSend(void *base,
                               uint8_t deviceAddress,
                               uint32_t subAddress,
                               uint8_t subAddressSize,
                               const uint8_t *txBuff,
                               uint8_t txBuffSize,
                               uint32_t flags)
{
    return BOARD_LPI2C_Send((LPI2C_Type *)base, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_I2C_DeviceReceive(void *base,
                                  uint8_t deviceAddress,
                                  uint32_t subAddress,
                                  uint8_t subAddressSize,
                                  uint8_t *rxBuff,
                                  uint8_t rxBuffSize,
                                  uint32_t flags)
{
    return BOARD_LPI2C_Receive((LPI2C_Type *)base, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}
#endif


#if defined(BOARD_USE_PCAL6524) && BOARD_USE_PCAL6524
/* Board-level singleton: drivers/examples that don't manage their own handle
 * use BOARD_GetPCAL6524Handle(). ENET examples that pass their own handle to
 * BOARD_InitPCAL6524(handle) bypass this singleton. */
static pcal6524_handle_t s_pcal6524Handle;
static bool s_pcal6524Initialized = false;

/* IRQ-masking lock installed on every PCAL6524 handle initialized through
 * board.c. The PCAL6524 driver wraps every R-M-W (and its InterruptHandler
 * snapshot) with this hook so the MCU GPIO ISR for the PCAL6524 INT line
 * (see BOARD_PCAL6524_INT_IRQ_HANDLER below) cannot race task-context I/O on
 * prevPinStates. Masking the INT IRQ is sufficient — the LPI2C transport
 * itself is blocking-polling (BOARD_LPI2C_Send/Receive) so no other shared
 * resource is at risk. Safe to call before the IRQ has been enabled in NVIC:
 * DisableIRQ/EnableIRQ just twiddle NVIC enable bits and have no effect on
 * pending pin state. */
static status_t BOARD_PCAL6524_Lock(bool lock)
{
    if (lock)
    {
        DisableIRQ(BOARD_PCAL6524_INT_IRQ);
    }
    else
    {
        EnableIRQ(BOARD_PCAL6524_INT_IRQ);
    }
    return kStatus_Success;
}

void BOARD_PCAL6524_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_PCAL6524_I2C, BOARD_PCAL6524_I2C_CLOCK_FREQ);
    BOARD_Init6524Pins();
}

status_t BOARD_PCAL6524_I2C_Send(uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 const uint8_t *txBuff,
                                 uint8_t txBuffSize,
                                 uint32_t flags)
{
    return BOARD_LPI2C_Send(BOARD_PCAL6524_I2C, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_PCAL6524_I2C_Receive(uint8_t deviceAddress,
                                    uint32_t subAddress,
                                    uint8_t subAddressSize,
                                    uint8_t *rxBuff,
                                    uint8_t rxBuffSize,
                                    uint32_t flags)
{
    return BOARD_LPI2C_Receive(BOARD_PCAL6524_I2C, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}

void BOARD_InitPCAL6524(pcal6524_handle_t *handle)
{
    BOARD_PCAL6524_I2C_Init();

    static const pcal6524_config_t config = {
        .i2cBase         = BOARD_PCAL6524_I2C,
        .i2cAddr         = BOARD_PCAL6524_I2C_ADDR,
        .I2C_SendFunc    = BOARD_I2C_DeviceSend,
        .I2C_ReceiveFunc = BOARD_I2C_DeviceReceive,
    };

    PCAL6524_Init(handle, &config);

    /* PCAL6524 takes its lock hook on the handle (not the config). NULL after
     * Init; install our IRQ-masking lock so callers that don't care can ignore
     * the hook entirely. Existing ENET examples that pass their own handle
     * get the same lock — harmless: they never EnableIRQ(BOARD_PCAL6524_INT_IRQ),
     * so the DisableIRQ/EnableIRQ inside the lock are no-ops on their flow. */
    handle->lock = BOARD_PCAL6524_Lock;
}

pcal6524_handle_t *BOARD_GetPCAL6524Handle(void)
{
    if (!s_pcal6524Initialized)
    {
        BOARD_InitPCAL6524(&s_pcal6524Handle);
        s_pcal6524Initialized = true;
    }
    return &s_pcal6524Handle;
}

void BOARD_EnablePCAL6524Interrupt(void)
{
    /* PCAL6524 INT is active-LOW open-drain → falling edge on the MCU side.
     * The INT pad mux (BOARD_Init6524Pins in pin_mux.c) must be invoked from
     * the example's BOARD_InitHardware before this point. */
    const gpio_pin_config_t intPinConfig = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0U,
    };
    GPIO_PinInit(BOARD_PCAL6524_INT_GPIO, BOARD_PCAL6524_INT_PIN, &intPinConfig);
    GPIO_SetPinInterruptConfig(BOARD_PCAL6524_INT_GPIO, BOARD_PCAL6524_INT_PIN,
                               kGPIO_InterruptFallingEdge);
    GPIO_GpioClearInterruptFlags(BOARD_PCAL6524_INT_GPIO, 1U << BOARD_PCAL6524_INT_PIN);
    EnableIRQ(BOARD_PCAL6524_INT_IRQ);
}

/*
 * MCU GPIO ISR for the PCAL6524 INT line. Open-drain INT asserts low on any
 * input change. Clears the MCU-side pending flag, then drives the PCAL6524
 * software dispatcher synchronously so each registered per-pin callback
 * (installed via PCAL6524_InstallPinCallback on the singleton handle) fires
 * this turn.
 *
 * I2C-in-ISR caveat: BOARD_LPI2C_Send/Receive are blocking-polling, so this
 * is safe correctness-wise (no nested IRQ deadlock), just costs extra cycles
 * in the ISR. BOARD_PCAL6524_Lock masks this same IRQ during task-context
 * I2C access to keep callers from racing the snapshot.
 *
 * Weak so examples that prefer the defer-to-task pattern (e.g. interrupt_demo
 * sets a flag and dispatches from main loop) can override with a strong
 * definition.
 */
__attribute__((weak)) void BOARD_PCAL6524_INT_IRQ_HANDLER(void)
{
    uint32_t flags = GPIO_GpioGetInterruptFlags(BOARD_PCAL6524_INT_GPIO);

    if (0U != (flags & (1U << BOARD_PCAL6524_INT_PIN)))
    {
        GPIO_GpioClearInterruptFlags(BOARD_PCAL6524_INT_GPIO, 1U << BOARD_PCAL6524_INT_PIN);
        (void)PCAL6524_InterruptHandler(&s_pcal6524Handle);
    }
    SDK_ISR_EXIT_BARRIER;
}
#endif /* BOARD_USE_PCAL6524 */

#if defined(BOARD_USE_PCA9555) && BOARD_USE_PCA9555
/* Board-level singleton; same rationale as PCAL6524 above. */
static pca9555_handle_t s_pca9555Handle;
static bool s_pca9555Initialized = false;

/* IRQ-masking lock; same rationale as PCAL6524 above. */
static status_t BOARD_PCA9555_Lock(bool lock)
{
    if (lock)
    {
        DisableIRQ(BOARD_PCA9555_INT_IRQ);
    }
    else
    {
        EnableIRQ(BOARD_PCA9555_INT_IRQ);
    }
    return kStatus_Success;
}

void BOARD_PCA9555_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_PCA9555_I2C, BOARD_PCA9555_I2C_CLOCK_FREQ);
    BOARD_InitPCA9555Pins();
}

void BOARD_InitPCA9555(pca9555_handle_t *handle)
{
    pca9555_config_t config;

    BOARD_PCA9555_I2C_Init();

    config.i2cBase         = BOARD_PCA9555_I2C;
    config.i2cAddr         = BOARD_PCA9555_I2C_ADDR;
    config.I2C_SendFunc    = BOARD_I2C_DeviceSend;
    config.I2C_ReceiveFunc = BOARD_I2C_DeviceReceive;
    config.lockFunc        = BOARD_PCA9555_Lock;

    (void)PCA9555_Init(handle, &config);
}

pca9555_handle_t *BOARD_GetPCA9555Handle(void)
{
    if (!s_pca9555Initialized)
    {
        BOARD_InitPCA9555(&s_pca9555Handle);
        s_pca9555Initialized = true;
    }
    return &s_pca9555Handle;
}

void BOARD_EnablePCA9555Interrupt(void)
{
    /* PCA9555 EXP_nIRQ2 is active-LOW open-drain → falling edge on the MCU side.
     * The INT pad mux (BOARD_InitPCA9555Pins in pin_mux.c — PIO3_29 →
     * HSP_GPIO1 pin 29) must be invoked from the example's BOARD_InitHardware
     * before this point. */
    const gpio_pin_config_t intPinConfig = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0U,
    };
    GPIO_PinInit(BOARD_PCA9555_INT_GPIO, BOARD_PCA9555_INT_PIN, &intPinConfig);
    GPIO_SetPinInterruptConfig(BOARD_PCA9555_INT_GPIO, BOARD_PCA9555_INT_PIN,
                               kGPIO_InterruptFallingEdge);
    GPIO_GpioClearInterruptFlags(BOARD_PCA9555_INT_GPIO, 1U << BOARD_PCA9555_INT_PIN);
    EnableIRQ(BOARD_PCA9555_INT_IRQ);
}

/*
 * MCU GPIO ISR for the PCA9555 EXP_nIRQ2 line. See BOARD_PCAL6524_INT_IRQ_HANDLER
 * above for the I2C-in-ISR / lockFunc serialization rationale.
 *
 * BOARD_PCA9555_INT_IRQ shares the HSP_GPIO1_CH0 vector with BOARD_USER_BUTTON_IRQ;
 * only the PCA9555 INT pin (BOARD_PCA9555_INT_PIN) is serviced here. Examples that
 * also need to respond to BOARD_USER_BUTTON_GPIO_PIN must override this handler
 * (the weak attribute lets a strong definition win) and dispatch both pins.
 */
__attribute__((weak)) void BOARD_PCA9555_INT_IRQ_HANDLER(void)
{
    uint32_t flags = GPIO_GpioGetInterruptFlags(BOARD_PCA9555_INT_GPIO);

    if (0U != (flags & (1U << BOARD_PCA9555_INT_PIN)))
    {
        GPIO_GpioClearInterruptFlags(BOARD_PCA9555_INT_GPIO, 1U << BOARD_PCA9555_INT_PIN);
        (void)PCA9555_InterruptHandler(&s_pca9555Handle);
    }
    SDK_ISR_EXIT_BARRIER;
}
#endif /* BOARD_USE_PCA9555 */
