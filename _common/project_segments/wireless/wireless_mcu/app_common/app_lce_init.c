/*
 * Copyright 2022 NXP
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

/* -------------------------------------------------------------------------- */
/*                              Public memory declarations                    */
/* -------------------------------------------------------------------------- */
uint8_t      g_ceHeap_id = 0U;
/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
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

#ifndef gExtendedHeapSize_c
#define gExtendedHeapSize_c (24U*1024U)
#endif
static uint32_t maExtHeap[gExtendedHeapSize_c / sizeof(uint32_t)];

static memAreaCfg_t mExtHeapCfg = {
    .next          = NULL,
    .start_address = maExtHeap,
    .end_address   = &((uint8_t*)maExtHeap)[gExtendedHeapSize_c - 1],
    .flags         = 0U,
};

static mem_status_t CE_MEM_Init(void)
{
    (void)MEM_Init();
    mem_status_t st = kStatus_MemSuccess;
    uint8_t      memHeap_id;
    if (st == kStatus_MemSuccess)
    {
        st = MEM_RegisterExtendedArea(&mCeHeap, &g_ceHeap_id, AREA_FLAGS_POOL_NOT_SHARED);
    }
    if (st == kStatus_MemSuccess)
    {
        st = MEM_RegisterExtendedArea(&mExtHeapCfg, &memHeap_id, 0);
    }
    return st;
}

/* CE image */
static const char ceImage[] = {
#include "ce_kw47_mcxw72.txt"
};

static void CE_ImageLoad(volatile char *ceImageBase, char const *ceImage, uint32_t ceImageSize)
{
    for (int i = 0; i < ceImageSize; i++)
    {
        ceImageBase[i] = ceImage[i];
    }
}

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

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
