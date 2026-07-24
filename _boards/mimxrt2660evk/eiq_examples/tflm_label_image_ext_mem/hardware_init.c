/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "board_init.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"

/* MPU configuration for NPU */
void BOARD_ConfigMPU_NPU(void)
{
    uint8_t attr;

#if 0
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t nonCacheSize  = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                                 0 :
                                 ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_NCACHE_REGION;
    extern uint32_t __top_NCACHE_REGION;
    uint32_t nonCacheStart = (uint32_t)(&__base_NCACHE_REGION);
    uint32_t nonCacheSize  = (uint32_t)(&__top_NCACHE_REGION) - nonCacheStart;
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t nonCacheSize  = (uint32_t)__NCACHE_REGION_SIZE;
#else
#error "Unsupported compiler"
#endif
#endif

    /* Attr0: Device-nGnRnE */
    ARM_MPU_SetMemAttr(0U, ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE_nGnRnE));
    /* Attr1: Normal memory, Outer non-cacheable, Inner non-cacheable */
    ARM_MPU_SetMemAttr(1U, ARM_MPU_ATTR(ARM_MPU_ATTR_NON_CACHEABLE, ARM_MPU_ATTR_NON_CACHEABLE));
    /* Attr2: Normal memory, Inner write-through transient, read allocate. Inner write-through transient, read allocate */
    attr = ARM_MPU_ATTR_MEMORY_(0U, 0U, 1U, 0U);
    ARM_MPU_SetMemAttr(2U, ARM_MPU_ATTR(attr, attr));
    /* Attr3: Normal memory, Outer write-back transient, read/write allocate. Inner write-back transient, read/write allocate */
    attr = ARM_MPU_ATTR_MEMORY_(0U, 1U, 1U, 1U);
    ARM_MPU_SetMemAttr(3U, ARM_MPU_ATTR(attr, attr));

    attr = ARM_MPU_ATTR_MEMORY_(0U, 1U, 1U, 0U);        // NT 0, WB 1, RA 1, WA 0
    ARM_MPU_SetMemAttr(4U, ARM_MPU_ATTR(attr, attr));

    attr = ARM_MPU_ATTR_MEMORY_(0U, 0U, 1U, 1U);        // NT 0, WB 0, RA 1, WA 1
    ARM_MPU_SetMemAttr(5U, ARM_MPU_ATTR(attr, attr));

    attr = ARM_MPU_ATTR_MEMORY_(0U, 1U, 1U, 1U);
    ARM_MPU_SetMemAttr(6U, ARM_MPU_ATTR(attr, ARM_MPU_ATTR_NON_CACHEABLE));
    /* Region 0 (FlexSPI2, PSRAM) : [0x8000_0000, 0x8200_0000, 32M]*/
    /* non-shareable, read/write in privilege and non-privilege, executable. Attr 3 */
    /*
     * Note :
     *     1. in psram_debug version, setting this to 3 will cause hardfault
     *     2. in psram_txt_debug version, setting this to 3 works fine
     */
    ARM_MPU_SetRegion(0U, ARM_MPU_RBAR(0x80000000, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x817FFFFF, 3U));      // 24MB Cacheable PSRAM


    /* Region 1 (ITCM): [0x0FFE0000, 0x0FFFFFFF, 128K] */
    /* non-shareable, read/write in privilege and non-privilege, executable. Attr 2 */
    ARM_MPU_SetRegion(1U, ARM_MPU_RBAR(0x00000000, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x0001FFFF, 2U));

    /* Region 2 (DTCM): [0x20000000, 0x2001FFFF, 128K] */
    /* non-shareable, read/write in privilege and non-privilege, executable. Attr 3 */
    ARM_MPU_SetRegion(2U, ARM_MPU_RBAR(0x20000000, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x2001FFFF, 3U));

#if 0
    /*Use hard code way to configure PSRAM/OCRAM into cacheable and noncacheable area respectively*/
    if (nonCacheSize > 0)
        ARM_MPU_SetRegion(3U, ARM_MPU_RBAR(nonCacheStart, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(nonCacheStart + nonCacheSize -1, 1U));
#endif

    // Test OCRAM Uncacheable
    /*
     * Note :
     *     1. in psram_debug version, setting this to 3 is OK
     *     2. in psram_txt_debug version, setting this to 3 will cause hard_fault
     */
    ARM_MPU_SetRegion(4U, ARM_MPU_RBAR(0x22000000, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x2207FFFF, 3U));      // 512KB Cacheable OCRAM

#if 1
    /*Hard Code Non Cacheable Region*/
    ARM_MPU_SetRegion(5U, ARM_MPU_RBAR(0x81800000, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x81FFFFFF, 1U));      // 8MB Non Cacheable PSRAM
    ARM_MPU_SetRegion(6U, ARM_MPU_RBAR(0x22080000, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x220BFFFF, 1U));      // 256KB Non Cacheable OCRAM
#endif

#if 1
    /* Setting PSRAM LLC Region */
    ARM_MPU_SetRegion(7U, ARM_MPU_RBAR(0x88000000, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x897FFFFF, 3U));      // 24MB Cacheable PSRAM
    ARM_MPU_SetRegion(8U, ARM_MPU_RBAR(0x89800000, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x89FFFFFF, 6U));      // 8MB Non Cacheable PSRAM
#endif

    ARM_MPU_SetRegion(9U, ARM_MPU_RBAR(0x20900000, ARM_MPU_SH_NON, 0U, 0U, 0U), ARM_MPU_RLAR(0x20900FFF, 0U));
    // ICB->ACTLR &= ~ICB_ACTLR_DISNWAMODE_Msk;

    /*
     * Enable MPU and HFNMIENA feature
     * HFNMIENA ensures the core uses MPU configuration when in hard fault, NMI, and FAULTMASK handlers,
     * otherwise all memory regions are accessed without MPU protection.
     */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);

    /* Re-enable branch predictor: BOARD_ResetMPU() cleared it before the MPU
     * reconfiguration; restore it now that the NPU-specific layout is active. */
    SCB->CCR |= SCB_CCR_BP_Msk;

    // Enable caches
    SCB_EnableICache();
    SCB_EnableDCache();
}

void SysInit()
{
#if 0
    *(uint32_t *)0x40550018 = 0x003f003f;
    *(uint32_t *)0x4055001c = 0x00000000;
    *(uint32_t *)0x40550030 = 0x0000003f;

    uint32_t wdata;
    wdata = *(uint32_t *)0x50040000 & 0xffff3fff;
    *(uint32_t *)0x50040000 = wdata;
    wdata = *(uint32_t *)0x54040000 & 0xffff3fff;
    *(uint32_t *)0x54040000 = wdata;

    *(uint32_t *)0xe000ed98 = 0x00000000;
    *(uint32_t *)0xe000ed9c = 0x20900000;
    *(uint32_t *)0xe000edc0 = 0x00000000;
    *(uint32_t *)0xe000edc4 = 0x00000000;
    *(uint32_t *)0xe000eda0 = 0x20900fe1;
    *(uint32_t *)0xe000ed94 = 0x00000007;
#endif
}

void cleanCache_by_Addr(uint32_t addr, uint32_t size)
{
    if (SCB->CCR & SCB_CCR_DC_Msk) {
        SCB_CleanDCache_by_Addr((uint32_t *)addr, size);
    }
    /* Do memory barrier */
    __DSB();
    __ISB();
}

void BOARD_Init()
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    /* Reset cache, MPU, and all regions before applying NPU-specific MPU layout. */
    BOARD_ResetMPU();
    BOARD_ConfigMPU_NPU();

    // TRDC config for NPU
    CMPT__TRDC->MDA_DFMT1[kTRDC_CMPT_MasterNPU].MDA_W_DFMT1[0] = TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;

    SysInit();
}
