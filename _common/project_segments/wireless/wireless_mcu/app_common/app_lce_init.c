/*
 * Copyright 2022, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_component_mem_manager.h"
#include "fsl_mu.h"
#include "fsl_ce_cmd.h"
#include "fsl_ce.h"
#include "fsl_component_lce.h"
#include "app_lce_init.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

/* -------------------------------------------------------------------------- */
/*                              Public memory declarations                    */
/* -------------------------------------------------------------------------- */
uint8_t      g_ceHeap_id = 0U;
/* -------------------------------------------------------------------------- */
/*                             Private memory declarations                    */
/* -------------------------------------------------------------------------- */
/* CE buffer definition */
#if defined(__IAR_SYSTEMS_ICC__)
volatile static struct
{
    uint32_t ce_cmd_buffer[CE_CMD_BUF_SIZE];
    int32_t ce_status_buffer[CE_STATUS_BUF_SIZE];
} ce_buffers @ "ceCtrl";
#elif defined(__GNUC__)
volatile static struct
{
    uint32_t ce_cmd_buffer[CE_CMD_BUF_SIZE];
    int32_t ce_status_buffer[CE_STATUS_BUF_SIZE];
} ce_buffers __attribute__((section("ceCtrl")));
#elif defined(__CC_ARM)
volatile static struct
{
    uint32_t ce_cmd_buffer[CE_CMD_BUF_SIZE];
    int32_t ce_status_buffer[CE_STATUS_BUF_SIZE];
} ce_buffers __attribute__((section("ceCtrl")));
#else
#error "Compiler unknown!"
#endif

static ce_cmdbuffer_t cpu_ce_cmdbuffer; /* Non-reentrant */

/* CE data STCM manager initialization */
#ifndef MinimalCeHeapSize_c
#define MinimalCeHeapSize_c (4U)
#endif
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = "ceHeap"
static uint32_t ce_memHeap[MinimalCeHeapSize_c / sizeof(uint32_t)];
#elif defined(__GNUC__)
static uint32_t ce_memHeap[MinimalCeHeapSize_c / sizeof(uint32_t)] __attribute__((section("ceHeap")));
#elif defined(__CC_ARM)
static uint32_t ce_memHeap[MinimalCeHeapSize_c / sizeof(uint32_t)] __attribute__((section("ceHeap")));
#else
#error "Compiler unknown!"
#endif
extern uint32_t __CE_MEM_HEAP_end__[];

/* Name must differ from section "ceHeap", otherwise gcc will complain */
static memAreaCfg_t mCeHeap = {
    .next          = NULL,
    .start_address = ce_memHeap,
    .end_address   = __CE_MEM_HEAP_end__,
    .flags         = AREA_FLAGS_POOL_NOT_SHARED,
};

#if (defined(KW47_core0_SERIES) || defined(MCXW72_core0_SERIES))
#ifndef gMinExtendedHeapSize_c
#define gMinExtendedHeapSize_c (24U*1024U)
#endif

#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = ".extHeap"
static uint32_t maExtHeap[gMinExtendedHeapSize_c / sizeof(uint32_t)];
#elif defined(__GNUC__)
static uint32_t maExtHeap[gMinExtendedHeapSize_c / sizeof(uint32_t)] __attribute__((section(".extHeap")));
#elif defined(__CC_ARM)
static uint32_t maExtHeap[gMinExtendedHeapSize_c / sizeof(uint32_t)] __attribute__((section(".extHeap")));
#else
#error "Compiler unknown!"
#endif

extern uint32_t __EXT_HEAP_end__[];

static memAreaCfg_t mExtHeapCfg = {
    .next          = NULL,
    .start_address = maExtHeap,
    .end_address   = __EXT_HEAP_end__,
    .flags         = 0U,
};
/* CE image */
static const char ceImage[] = {
#include "ce_kw47_mcxw72.txt"
};
#elif (defined(KW43_core0_SERIES) || defined(MCXW70_core0_SERIES))
/* CE image */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment = 128
static const uint8_t ceImage[] = {
#elif defined(__GNUC__)
static const uint8_t ceImage[] __attribute__((aligned(128))) = {
#elif defined(__CC_ARM)
__align(128) static const uint8_t ceImage[] = {
#else
#error "Compiler unknown!"
#endif
#include "ce_kw43_mcxw70.txt"
};
#endif /* (defined(KW47_core0_SERIES) || defined(MCXW72_core0_SERIES)) */

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static mem_status_t CE_MEM_Init(void)
{
    (void)MEM_Init();
    mem_status_t st = kStatus_MemSuccess;
    if (st == kStatus_MemSuccess)
    {
        st = MEM_RegisterExtendedArea(&mCeHeap, &g_ceHeap_id, AREA_FLAGS_POOL_NOT_SHARED);
    }
#if (defined(KW47_core0_SERIES) || defined(MCXW72_core0_SERIES))
    if (st == kStatus_MemSuccess)
    {
        uint8_t      memHeap_id;
        st = MEM_RegisterExtendedArea(&mExtHeapCfg, &memHeap_id, 0);
    }
#endif /* (defined(KW47_core0_SERIES) || defined(MCXW72_core0_SERIES)) */
    return st;
}

#if (defined(KW47_core0_SERIES) || defined(MCXW72_core0_SERIES))
static void CE_ImageLoad(volatile char *ceImageBase, char const *ceImage, uint32_t ceImageSize)
{
    for (int i = 0; i < ceImageSize; i++)
    {
        ceImageBase[i] = ceImage[i];
    }
}
#endif /* (defined(KW47_core0_SERIES) || defined(MCXW72_core0_SERIES)) */
/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

#if (defined(KW47_core0_SERIES) || defined(MCXW72_core0_SERIES))
void APP_InitLce(void)
{
    /* Load CE image to CE code SRAM */
    volatile char *ceImageBase = (volatile char *)CE_STCM5_BASE;
    (void)CE_ImageLoad(ceImageBase, ceImage, sizeof(ceImage));
    /* CE memory manager initialization */
    (void)CE_MEM_Init();
    /* CE MU initialization */
    (void)MU_Init((MU_Type *)MUA_BASE);
    /* CE boot configuration */
    (void)MU_BootOtherCore((MU_Type *)MUA_BASE, kMU_CoreBootFromSTCM5);
    /* CE API execution buffer initialization */
    (void)CE_CmdInitBuffer(&cpu_ce_cmdbuffer, ce_buffers.ce_cmd_buffer, ce_buffers.ce_status_buffer,
                           kCE_CmdModeOneNonBlocking);
    /* Enable CE clock */
    (void)CLOCK_EnableClockLPMode(kCLOCK_DSP0, kCLOCK_IpClkControl_fun2);
    /* Initialization for LCE APIs */
    (void)LCE_Init();
    return;
}
#elif (defined(KW43_core0_SERIES) || defined(MCXW70_core0_SERIES))
void APP_InitLce(void)
{
    union
    {
        const uint8_t *pI;
        uint32_t  address;
    }ceImageSource = {.pI = ceImage};
    /* Enable DSP RAM clock */
    CLOCK_EnableClockLPMode(kCLOCK_Dsp_ramc0, kCLOCK_IpClkControl_fun2);
    /* Enable LCE clock */
    CLOCK_EnableClockLPMode(kCLOCK_Zenv_core, kCLOCK_IpClkControl_fun2); 
    /*Enable MU clock */
    CLOCK_EnableClockLPMode(kCLOCK_Mu1, kCLOCK_IpClkControl_fun2); 
    /* CE MU initialization */
    MU_Init(MU1_MUA);
    /* By default, TRDC configuration does not enable the DSP's master permissions.
     * This statement configures the DSP's DID (Domain ID) and its Secure/Non-secure
     * attributes. Additionally, since there is PRESET data, the ROM has already
     * configured SRAM access permissions, but these permissions are mapped to a
     * specific DID. Therefore, it's necessary to configure the DSP's DID in TRDC
     * to match the DID that has SRAM access permissions. */
    TRDC_0_MGR->MDA_DFMT0[6].MDA_W_DFMT0[0] = 0x80000011U;
    /* Configure MBC3 GLBAC1: read and execute allowed for all security/privilege levels on Flash. */
    TRDC_0_MGR->MBC_INDEX[3].MBC_MEMN_GLBAC[1] = 0x5555U;
    /* Unlock SYSCON */
    SYSCON->AUTHENTICATE = SYSCON_UNLOCK_CODE;
    /* Set ZENV vector start address */
    SYSCON->ZENV_CTRL = ceImageSource.address;
    /* Release reset of ZENV CORE */
    SYSCON->ZENV_RESET_CTRL = 0UL;
    /* Lock SYSCON again */
    SYSCON->AUTHENTICATE = 0UL;
    (void)CE_MEM_Init();
    (void)CE_CmdInitBuffer(&cpu_ce_cmdbuffer, ce_buffers.ce_cmd_buffer, ce_buffers.ce_status_buffer,
                           kCE_CmdModeOneNonBlocking);
    /* Initialization for LCE APIs */
    (void)LCE_Init();
}
#endif /* (defined(KW47_core0_SERIES) || defined(MCXW72_core0_SERIES)) */