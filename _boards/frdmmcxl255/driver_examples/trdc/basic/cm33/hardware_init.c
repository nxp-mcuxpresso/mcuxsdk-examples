/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "app.h"
#include "fsl_trdc.h"
#include "fsl_glikey.h"
/*${header:end}*/

/*${variable:start}*/
static trdc_mbc_memory_block_config_t mbcBlockConfig;
static const int glikey_index_mbc = 15;
const volatile uint8_t * g_mbc_mem = (uint8_t*)EXAMPLE_TRDC_MBC_BLK_ADDR;
/*${variable:end}*/

/*${function:start}*/
void APP_SetTrdcAccessible(void)
{
    /* Use Glikey to enable modifications of MBC registers: */
  
    status_t status = GLIKEY_IsLocked(GLIKEY0);
    assert(kStatus_GLIKEY_NotLocked == status); 

    status = GLIKEY_SyncReset(GLIKEY0);
    assert(kStatus_Success == status);
    
    status = GLIKEY_StartEnable(GLIKEY0, glikey_index_mbc);
    assert(kStatus_Success == status);
    
    status = GLIKEY_ContinueEnable(GLIKEY0, GLIKEY_CODEWORD_STEP1);
    assert(kStatus_Success == status);
    
    status = GLIKEY_ContinueEnable(GLIKEY0, GLIKEY_CODEWORD_STEP2);
    assert(kStatus_Success == status);
    
    status = GLIKEY_ContinueEnable(GLIKEY0, GLIKEY_CODEWORD_STEP3);
    assert(kStatus_Success == status);
    
    status = GLIKEY_ContinueEnable(GLIKEY0, GLIKEY_CODEWORD_STEP_EN);
    assert(kStatus_Success == status);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    
    /* Make sure alignment of g_mbc_mem equals to MBC0_MEM0_GLBCFG_SIZE_LOG2 so
       buffers maps exctly to one MBC block */
    assert (((MBC0->MBC_INDEX[0].MBC_MEM_GLBCFG[0] & 
              TRDC_MBC_MEM_GLBCFG_SIZE_LOG2_MASK) >>
             TRDC_MBC_MEM_GLBCFG_SIZE_LOG2_SHIFT) \
                 == \
             EXAMPLE_TRDC_MBC0_MEM0_GLBCFG_SIZE_LOG2);

    APP_SetTrdcAccessible();
}

uint32_t APP_GetMbc0Mem0BlockIdx(const void* mem_ptr)
{
  uint32_t idx = (uint32_t)mem_ptr;
  const uint32_t shift = (MBC0->MBC_INDEX[0].MBC_MEM_GLBCFG[0] & 
                          TRDC_MBC_MEM_GLBCFG_SIZE_LOG2_MASK ) >>
                          TRDC_MBC_MEM_GLBCFG_SIZE_LOG2_SHIFT;
  const uint32_t nblks = (MBC0->MBC_INDEX[0].MBC_MEM_GLBCFG[0] & 
                          TRDC_MBC_MEM_GLBCFG_NBLKS_MASK ) >>
                          TRDC_MBC_MEM_GLBCFG_NBLKS_SHIFT;
  
  /* Zero secure bit from address */
  idx &= ~0x10000000U;
  
  /* Shift by GLBCFG_NBLKS_LOG2*/
  idx >>= shift;
  
  /* Check if mem_ptr fits to MBC0_MEM0 */
  assert(idx < nblks);
  
  return idx;  
}

void APP_SetTrdcGlobalConfig(void)
{
    TRDC_Init(EXAMPLE_TRDC_INSTANCE);

    /* Make the all flash region accessible. */
    *(volatile uint32_t *)0x4008E020 = 0x00007777;
    *(volatile uint32_t *)0x4008E040 = 0x00000000;
    *(volatile uint32_t *)0x4008E044 = 0x00000000;
    *(volatile uint32_t *)0x4008E048 = 0x00000000;
    *(volatile uint32_t *)0x4008E04C = 0x00000000;
    *(volatile uint32_t *)0x4008E050 = 0x00000000;
    *(volatile uint32_t *)0x4008E054 = 0x00000000;
    *(volatile uint32_t *)0x4008E058 = 0x00000000;
    *(volatile uint32_t *)0x4008E05C = 0x00000000;

    /* 1. Set control policies for MBC access control configuration registers */
    trdc_memory_access_control_config_t memAccessConfig;
    (void)memset(&memAccessConfig, 0, sizeof(memAccessConfig));

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

    TRDC_MbcSetMemoryAccessConfig(EXAMPLE_TRDC_INSTANCE, &memAccessConfig, 0U, EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX);

    memAccessConfig.securePrivX    = 0U;
    memAccessConfig.securePrivW    = 0U;
    memAccessConfig.securePrivR    = 0U;
    memAccessConfig.nonsecurePrivX = 0U;
    memAccessConfig.nonsecurePrivW = 0U;
    memAccessConfig.nonsecurePrivR = 0U;
    memAccessConfig.secureUsrX     = 0U;
    memAccessConfig.secureUsrW     = 0U;
    memAccessConfig.secureUsrR     = 0U;
    memAccessConfig.nonsecureUsrX  = 0U;
    memAccessConfig.nonsecureUsrW  = 0U;
    memAccessConfig.nonsecureUsrR  = 0U;

    TRDC_MbcSetMemoryAccessConfig(EXAMPLE_TRDC_INSTANCE, &memAccessConfig, 0U, EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX_NO_ACCESS);

    /* 2. Set the configuration for the MBC slave memory block that is to be tested */

    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX;
    mbcBlockConfig.nseEnable                 = false;
    mbcBlockConfig.mbcIdx                    = 0U; /* Only have one MBC */
    mbcBlockConfig.domainIdx                 = 0U; /* Only have one domain */
    mbcBlockConfig.slaveMemoryIdx            = EXAMPLE_TRDC_MBC_SLAVE_INDEX;
    mbcBlockConfig.memoryBlockIdx            = APP_GetMbc0Mem0BlockIdx((void*)g_mbc_mem);

    TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
}

void APP_SetMbcUnaccessible(void)
{
    /* Use policy that can't access the memory region. */
    mbcBlockConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX_NO_ACCESS;
    TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
}

void APP_TouchMbcMemory(void)
{
    /* Try to read the memory.*/
    (void)g_mbc_mem[0];
}

void APP_ResolveMbcAccessError(void)
{
    PRINTF("Resolve access error\r\n");
    /* Use policy that can access the memory region. */
    mbcBlockConfig.memoryAccessControlSelect = EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX;
    TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
}