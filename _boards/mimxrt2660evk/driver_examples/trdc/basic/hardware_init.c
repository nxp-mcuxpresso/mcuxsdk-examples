/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
}

void APP_SetTrdcGlobalConfig()
{
    uint32_t i, j, m, n;

    CLOCK_EnableClock(kCLOCK_MAIN_trdc);

    TRDC_Init(EXAMPLE_TRDC_INSTANCE);

    /* 1. Get the hardware configuration of the EXAMPLE_TRDC_INSTANCE module */
    trdc_hardware_config_t hwConfig;
    TRDC_GetHardwareConfig(EXAMPLE_TRDC_INSTANCE, &hwConfig);

    /* 2. Set control policies for MBC and MRC access control configuration registers */
    trdc_memory_access_control_config_t memAccessConfig;
    (void)memset(&memAccessConfig, 0, sizeof(memAccessConfig));

    /* Disable all access modes for MBC and MRC access control configuration register EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_NONE_INDEX. */
    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        TRDC_MbcSetMemoryAccessConfig(EXAMPLE_TRDC_INSTANCE, &memAccessConfig, i, EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_NONE_INDEX);
    }

    for (i = 0U; i < hwConfig.mrcNumber; i++)
    {
        TRDC_MrcSetMemoryAccessConfig(EXAMPLE_TRDC_INSTANCE, &memAccessConfig, i, EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_NONE_INDEX);
    }

    /* Enable all access modes for MBC and MRC access control configuration register EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_ALL_INDEX. */
    memAccessConfig.nonsecureUsrX  = 1U;
    memAccessConfig.nonsecureUsrW  = 1U;
    memAccessConfig.nonsecureUsrR  = 1U;
    memAccessConfig.nonsecurePrivX = 1U;
    memAccessConfig.nonsecurePrivW = 1U;
    memAccessConfig.nonsecurePrivR = 1U;
    memAccessConfig.secureUsrX     = 1U;
    memAccessConfig.secureUsrW     = 1U;
    memAccessConfig.secureUsrR     = 1U;
    memAccessConfig.securePrivX    = 1U;
    memAccessConfig.securePrivW    = 1U;
    memAccessConfig.securePrivR    = 1U;

    for (i = 0U; i < hwConfig.mrcNumber; i++)
    {
        TRDC_MrcSetMemoryAccessConfig(EXAMPLE_TRDC_INSTANCE, &memAccessConfig, i, EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_ALL_INDEX);
    }

    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        TRDC_MbcSetMemoryAccessConfig(EXAMPLE_TRDC_INSTANCE, &memAccessConfig, i, EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_ALL_INDEX);
    }

    /* 3. Set the configuration for all MRC regions */
    trdc_mrc_region_descriptor_config_t mrcRegionConfig;
    (void)memset(&mrcRegionConfig, 0, sizeof(mrcRegionConfig));
    mrcRegionConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_ALL_INDEX;
    mrcRegionConfig.valid                     = true;
    mrcRegionConfig.nseEnable                 = false;
    mrcRegionConfig.mrcIdx                    = EXAMPLE_TRDC_MRC_INDEX;

    for (j = 0; j < hwConfig.domainNumber; j++)
    {
        mrcRegionConfig.domainIdx = j;
        n                         = TRDC_GetMrcRegionNumber(EXAMPLE_TRDC_INSTANCE, EXAMPLE_TRDC_MRC_INDEX);
        for (m = 0U; m < n; m++)
        {
            mrcRegionConfig.regionIdx = m;
            mrcRegionConfig.startAddr =
                EXAMPLE_TRDC_MRC_START_ADDR + (EXAMPLE_TRDC_MRC_END_ADDR - EXAMPLE_TRDC_MRC_START_ADDR) / n * m;
            mrcRegionConfig.endAddr = EXAMPLE_TRDC_MRC_START_ADDR +
                                        (EXAMPLE_TRDC_MRC_END_ADDR - EXAMPLE_TRDC_MRC_START_ADDR) / n * (m + 1U);

            TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_INSTANCE, &mrcRegionConfig);
        }
    }

    TRDC_SetMrcGlobalValid(EXAMPLE_TRDC_INSTANCE);

    /* 4. Set the configuration for all MBC slave memory blocks */
    trdc_slave_memory_hardware_config_t mbcHwConfig;
    trdc_mbc_memory_block_config_t mbcBlockConfig;
    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_ALL_INDEX;
    mbcBlockConfig.nseEnable                 = false;

    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        mbcBlockConfig.mbcIdx = i;
        for (j = 0U; j < hwConfig.domainNumber; j++)
        {
            mbcBlockConfig.domainIdx = j;
            for (m = 0U; m < 4; m++)
            {
                TRDC_GetMbcHardwareConfig(EXAMPLE_TRDC_INSTANCE, &mbcHwConfig, i, m);
                if (mbcHwConfig.blockNum == 0U)
                {
                    break;
                }
                mbcBlockConfig.slaveMemoryIdx = m;
                for (n = 0U; n < mbcHwConfig.blockNum; n++)
                {
                    mbcBlockConfig.memoryBlockIdx = n;

                    TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
                }
            }
        }
    }
    TRDC_SetMbcGlobalValid(EXAMPLE_TRDC_INSTANCE);
}

void APP_SetMrcUnaccessible(void)
{
    /* Set the MRC region descriptor configuration and select the memory access control of no access for this region */
    trdc_mrc_region_descriptor_config_t mrcRegionConfig;
    (void)memset(&mrcRegionConfig, 0, sizeof(mrcRegionConfig));
    mrcRegionConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_NONE_INDEX;
    mrcRegionConfig.startAddr                 = EXAMPLE_TRDC_MRC_START_ADDR;
    mrcRegionConfig.valid                     = true;
    /* CPU is secure mode by default, enable NSE bit to disable secure access. */
    mrcRegionConfig.nseEnable = true;
    mrcRegionConfig.endAddr   = EXAMPLE_TRDC_MRC_START_ADDR +
        (EXAMPLE_TRDC_MRC_END_ADDR - EXAMPLE_TRDC_MRC_START_ADDR) / TRDC_GetMrcRegionNumber(EXAMPLE_TRDC_INSTANCE, EXAMPLE_TRDC_MRC_INDEX);
    mrcRegionConfig.mrcIdx    = EXAMPLE_TRDC_MRC_INDEX;
    /* On RT2660 the CM85 core does not present Domain ID 0 to MAIN_TRDC (unlike the
     * single-core CM33 parts this example was templated from). An MRC access is only
     * evaluated against the region descriptor for the requester's own DID (RM
     * Rev.1DraftM, section 41.4.6.2), so denying domain 0 has no effect on the CPU and
     * the expected fault never fires. Use the requesting master's actual domain from
     * TRDC_HWCFG1[DID] instead of the hardcoded EXAMPLE_TRDC_DOMAIN_INDEX. */
    mrcRegionConfig.domainIdx = TRDC_GetCurrentMasterDomainId(EXAMPLE_TRDC_INSTANCE);
    mrcRegionConfig.regionIdx = EXAMPLE_TRDC_MRC_REGION_INDEX;

    TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_INSTANCE, &mrcRegionConfig);
}

void APP_SetMbcUnaccessible(void)
{
    /* Set the MBC slave memory block configuration and select the memory access control of no access for this memory
     * block */
    trdc_mbc_memory_block_config_t mbcBlockConfig;
    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_NONE_INDEX;
    /* CPU is secure mode by default, enable NSE bit to disable secure access. */
    mbcBlockConfig.nseEnable      = true;
    mbcBlockConfig.mbcIdx         = TRDC_MBC_GetMBCIdx(EXAMPLE_TRDC_MBC);
    /* Use the requesting master's actual domain (see APP_SetMrcUnaccessible). */
    mbcBlockConfig.domainIdx      = TRDC_GetCurrentMasterDomainId(EXAMPLE_TRDC_INSTANCE);
    mbcBlockConfig.slaveMemoryIdx = TRDC_MBC_GetSlvIdx(EXAMPLE_TRDC_MBC);
    mbcBlockConfig.memoryBlockIdx = TRDC_MBC_GetBlkIdx(EXAMPLE_TRDC_MBC);

    TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
}

void APP_TouchMrcMemory(void)
{
    /* Touch the memory. */
    (*(volatile uint32_t *)EXAMPLE_TRDC_MRC_START_ADDR);
}

void APP_TouchMbcMemory(void)
{
    /* Touch the memory. */
    (*(volatile uint32_t *)0x43800000);
}

void APP_CheckAndResolveMrcAccessError(trdc_domain_error_t *error)
{
    if (error->controller == kTRDC_MemRegionChecker2)
    {
        PRINTF("Violent access at address: 0x%8X\r\n", error->address);

        /* Set the MRC region descriptor configuration and select the memory access control of all access for this
         * region */
        trdc_mrc_region_descriptor_config_t mrcRegionConfig;
        (void)memset(&mrcRegionConfig, 0, sizeof(mrcRegionConfig));
        mrcRegionConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_ALL_INDEX;
        mrcRegionConfig.startAddr                 = EXAMPLE_TRDC_MRC_START_ADDR;
        mrcRegionConfig.valid                     = true;
        /* Disable NSE to enable secure access. */
        mrcRegionConfig.nseEnable = false;
        mrcRegionConfig.endAddr   = EXAMPLE_TRDC_MRC_START_ADDR +
            (EXAMPLE_TRDC_MRC_END_ADDR - EXAMPLE_TRDC_MRC_START_ADDR) / TRDC_GetMrcRegionNumber(EXAMPLE_TRDC_INSTANCE, EXAMPLE_TRDC_MRC_INDEX);
        mrcRegionConfig.mrcIdx    = EXAMPLE_TRDC_MRC_INDEX;
        /* Re-allow the same domain that was denied (see APP_SetMrcUnaccessible). */
        mrcRegionConfig.domainIdx = TRDC_GetCurrentMasterDomainId(EXAMPLE_TRDC_INSTANCE);
        mrcRegionConfig.regionIdx = EXAMPLE_TRDC_MRC_REGION_INDEX;

        TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_INSTANCE, &mrcRegionConfig);
    }
}

void APP_CheckAndResolveMbcAccessError(trdc_domain_error_t *error)
{
    if (error->controller == kTRDC_MemBlockController1)
    {
        PRINTF("Violent access at address: 0x%8X\r\n", error->address);

        /* Set the MBC slave memory block configuration and select the memory access control of no access for this
         * memory block */
        trdc_mbc_memory_block_config_t mbcBlockConfig;
        (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
        mbcBlockConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_ALL_INDEX;
        /* Disable NSE to enable secure access. */
        mbcBlockConfig.nseEnable      = false;
        mbcBlockConfig.mbcIdx         = TRDC_MBC_GetMBCIdx(EXAMPLE_TRDC_MBC);
        /* Re-allow the same domain that was denied (see APP_SetMrcUnaccessible). */
        mbcBlockConfig.domainIdx      = TRDC_GetCurrentMasterDomainId(EXAMPLE_TRDC_INSTANCE);
        mbcBlockConfig.slaveMemoryIdx = TRDC_MBC_GetSlvIdx(EXAMPLE_TRDC_MBC);
        mbcBlockConfig.memoryBlockIdx = TRDC_MBC_GetBlkIdx(EXAMPLE_TRDC_MBC);

        TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
    }
}
/*${function:end}*/
