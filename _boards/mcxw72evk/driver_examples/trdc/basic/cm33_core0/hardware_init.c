/*
 * Copyright 2021 NXP
 * All rights reserved.
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
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void APP_SetTrdcGlobalConfig()
{
    uint32_t i, j, m, n;

    TRDC_Init(TRDC);

    /* 1. Get the hardware configuration of the TRDC module */
    trdc_hardware_config_t hwConfig;
    TRDC_GetHardwareConfig(TRDC, &hwConfig);

    /* 2. Set control policies for MBC and MRC access control configuration registers */
    trdc_memory_access_control_config_t memAccessConfig;
    (void)memset(&memAccessConfig, 0, sizeof(memAccessConfig));

    /* Disable all access modes for MBC and MRC access control configuration register 1-7. */
    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        for (j = 1U; j < 8U; j++)
        {
            TRDC_MbcSetMemoryAccessConfig(TRDC, &memAccessConfig, i, j);
        }
    }

    for (i = 0U; i < hwConfig.mrcNumber; i++)
    {
        for (j = 1U; j < 8U; j++)
        {
            TRDC_MrcSetMemoryAccessConfig(TRDC, &memAccessConfig, i, j);
        }
    }

    /* Enable all access modes for MBC and MRC access control configuration register 0. */
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
        TRDC_MrcSetMemoryAccessConfig(TRDC, &memAccessConfig, i, EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_ALL_INDEX);
    }

    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        TRDC_MbcSetMemoryAccessConfig(TRDC, &memAccessConfig, i, EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_ALL_INDEX);
    }

    /* 3. Set the configuration for all MRC regions */
    trdc_mrc_region_descriptor_config_t mrcRegionConfig;
    (void)memset(&mrcRegionConfig, 0, sizeof(mrcRegionConfig));
    mrcRegionConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_ALL_INDEX;
    mrcRegionConfig.valid                     = true;
    mrcRegionConfig.nseEnable                 = false;

    for (i = 0; i < hwConfig.mrcNumber; i++)
    {
        mrcRegionConfig.mrcIdx = i;
        for (j = 0; j < hwConfig.domainNumber; j++)
        {
            mrcRegionConfig.domainIdx = j;
            n                         = TRDC_GetMrcRegionNumber(TRDC, i);
            for (m = 0U; m < n; m++)
            {
                mrcRegionConfig.regionIdx = m;
                mrcRegionConfig.startAddr =
                    EXAMPLE_TRDC_MRC_START_ADDR + (EXAMPLE_TRDC_MRC_END_ADDR - EXAMPLE_TRDC_MRC_START_ADDR) / n * m;
                mrcRegionConfig.endAddr = EXAMPLE_TRDC_MRC_START_ADDR +
                                          (EXAMPLE_TRDC_MRC_END_ADDR - EXAMPLE_TRDC_MRC_START_ADDR) / n * (m + 1U);

                TRDC_MrcSetRegionDescriptorConfig(TRDC, &mrcRegionConfig);
            }
        }
    }

    TRDC_SetMrcGlobalValid(TRDC);

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
                TRDC_GetMbcHardwareConfig(TRDC, &mbcHwConfig, i, m);
                if (mbcHwConfig.blockNum == 0U)
                {
                    break;
                }
                mbcBlockConfig.slaveMemoryIdx = m;
                for (n = 0U; n < mbcHwConfig.blockNum; n++)
                {
                    mbcBlockConfig.memoryBlockIdx = n;

                    TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
                }
            }
        }
    }
    TRDC_SetMbcGlobalValid(TRDC);

    /* 5. Set master domain ID for processor master */
    trdc_processor_domain_assignment_t pDomainAssignment;
    TRDC_GetDefaultProcessorDomainAssignment(&pDomainAssignment);
    pDomainAssignment.domainId = EXAMPLE_TRDC_PROCESSOR_MASTER_DOMAIN_ID;

    TRDC_SetProcessorDomainAssignment(TRDC, &pDomainAssignment);

    TRDC_SetDacGlobalValid(TRDC);
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
    mrcRegionConfig.endAddr   = 0x48840000UL;
    mrcRegionConfig.mrcIdx    = EXAMPLE_TRDC_MRC_INDEX;
    mrcRegionConfig.domainIdx = EXAMPLE_TRDC_DOMAIN_INDEX;
    mrcRegionConfig.regionIdx = EXAMPLE_TRDC_MRC_REGION_INDEX;

    TRDC_MrcSetRegionDescriptorConfig(TRDC, &mrcRegionConfig);
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
    mbcBlockConfig.mbcIdx         = EXAMPLE_TRDC_MBC_INDEX;
    mbcBlockConfig.domainIdx      = EXAMPLE_TRDC_DOMAIN_INDEX;
    mbcBlockConfig.slaveMemoryIdx = EXAMPLE_TRDC_MBC_SLAVE_INDEX;
    mbcBlockConfig.memoryBlockIdx = EXAMPLE_TRDC_MBC_MEMORY_INDEX;

    TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
}

void APP_TouchMrcMemory(void)
{
    /* Touch the memory. */
    (*(volatile uint32_t *)EXAMPLE_TRDC_MRC_START_ADDR);
}

void APP_TouchMbcMemory(void)
{
    /* Touch the memory. */
    (*(volatile uint32_t *)0x02000000);
}

void APP_CheckAndResolveMrcAccessError(trdc_domain_error_t *error)
{
    if (error->controller == kTRDC_MemRegionChecker0)
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
        mrcRegionConfig.endAddr   = 0x48840000UL;
        mrcRegionConfig.mrcIdx    = EXAMPLE_TRDC_MRC_INDEX;
        mrcRegionConfig.domainIdx = EXAMPLE_TRDC_DOMAIN_INDEX;
        mrcRegionConfig.regionIdx = EXAMPLE_TRDC_MRC_REGION_INDEX;

        TRDC_MrcSetRegionDescriptorConfig(TRDC, &mrcRegionConfig);
    }
}

void APP_CheckAndResolveMbcAccessError(trdc_domain_error_t *error)
{
    if (error->controller == kTRDC_MemBlockController0)
    {
        PRINTF("Violent access at address: 0x%8X\r\n", error->address);

        /* Set the MBC slave memory block configuration and select the memory access control of no access for this
         * memory block */
        trdc_mbc_memory_block_config_t mbcBlockConfig;
        (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
        mbcBlockConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_ALL_INDEX;
        /* Disable NSE to enable secure access. */
        mbcBlockConfig.nseEnable      = false;
        mbcBlockConfig.mbcIdx         = EXAMPLE_TRDC_MBC_INDEX;
        mbcBlockConfig.domainIdx      = EXAMPLE_TRDC_DOMAIN_INDEX;
        mbcBlockConfig.slaveMemoryIdx = EXAMPLE_TRDC_MBC_SLAVE_INDEX;
        mbcBlockConfig.memoryBlockIdx = EXAMPLE_TRDC_MBC_MEMORY_INDEX;

        TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
    }
}
/*${function:end}*/
