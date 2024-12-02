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
#include "app_lce_init.h"

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */
/* CE buffer definition */
#if defined(__IAR_SYSTEMS_ICC__)
volatile static struct
{
    uint32_t ce_cmd_buffer[CE_CMD_BUF_SIZE];
    uint32_t ce_status_buffer[CE_STATUS_BUF_SIZE];
} ce_buffers @ "ceCtrl";
#elif defined(__GNUC__)
volatile static struct
{
    uint32_t ce_cmd_buffer[CE_CMD_BUF_SIZE];
    uint32_t ce_status_buffer[CE_STATUS_BUF_SIZE];
} ce_buffers __attribute__((section("ceCtrl")));
#elif defined(__CC_ARM)
volatile static struct
{
    uint32_t ce_cmd_buffer[CE_CMD_BUF_SIZE];
    uint32_t ce_status_buffer[CE_STATUS_BUF_SIZE];
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
#pragma location = "ceHeap_1"
static uint32_t ce_memHeap_1[MinimalCeHeapSize_c / sizeof(uint32_t)];
#pragma location = "ceHeap_2"
static uint32_t ce_memHeap_2[MinimalCeHeapSize_c / sizeof(uint32_t)];
#elif defined(__GNUC__)
static uint32_t ce_memHeap_1[MinimalCeHeapSize_c / sizeof(uint32_t)] __attribute__((section("ceHeap_1")));
static uint32_t ce_memHeap_2[MinimalCeHeapSize_c / sizeof(uint32_t)] __attribute__((section("ceHeap_2")));
#elif defined(__CC_ARM)
static uint32_t ce_memHeap_1[MinimalCeHeapSize_c / sizeof(uint32_t)] __attribute__((section("ceHeap_1")));
static uint32_t ce_memHeap_2[MinimalCeHeapSize_c / sizeof(uint32_t)] __attribute__((section("ceHeap_2")));
#else
#error "Compiler unknown!"
#endif
extern uint32_t __CE_MEM_HEAP1_end__[];
extern uint32_t __CE_MEM_HEAP2_end__[];
/* Name must differ from section "ceHeap_1", otherwise gcc will complain */
static memAreaCfg_t ceHeap1 = {
    .next          = NULL,
    .start_address = ce_memHeap_1,
    .end_address   = __CE_MEM_HEAP1_end__,
    .flags         = 0,
};
/* Name must differ from section "ceHeap_2", otherwise gcc will complain */
static memAreaCfg_t ceHeap2 = {
    .next          = NULL,
    .start_address = ce_memHeap_2,
    .end_address   = __CE_MEM_HEAP2_end__,
    .flags         = 0,
};
static mem_status_t CE_MEM_Init(void)
{
    (void)MEM_Init();
    mem_status_t st = kStatus_MemSuccess;
    uint8_t      memHeap_id;
    if (st == kStatus_MemSuccess)
    {
        st = MEM_RegisterExtendedArea(&ceHeap1, &memHeap_id, 0);
    }
    if (st == kStatus_MemSuccess)
    {
        st = MEM_RegisterExtendedArea(&ceHeap2, &memHeap_id, 0);
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
                           kCE_CmdModeOneBlocking);
    /* Enable CE clock */
    (void)CLOCK_EnableClock(kCLOCK_DSP0);
    return;
}
