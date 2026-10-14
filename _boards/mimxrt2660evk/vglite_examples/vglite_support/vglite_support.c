/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "vglite_support.h"
#include "fsl_clock.h"
#include "FreeRTOS.h"
#include "task.h"
#include "vg_lite.h"

extern void *vglite_heap_base;
extern uint32_t vglite_heap_size;

uint32_t g_envErrCnt;

#define MDAC_NON_PROCESSOR_VALUE ((1u << 31) | (0u << 8) | (0u << 6) | (1u << 4) | (0u << 0))
#define CR_GVLDM (1u << 0)
#define SENTMU_TR_COUNT (16U)
#define SENTMU_RR_COUNT (16U)

static void HDL_InitMediaTrdcMdac(void)
{
    for (uint8_t inst = 0; inst <= 14; inst++)
    {
        *(volatile uint32_t *)(MEDIA__TRDC_BASE + 0x800 + 0x20 * inst) = MDAC_NON_PROCESSOR_VALUE;
    }
}

static void HDL_EnableMediaTrdcMdac(void)
{
    *(volatile uint32_t *)(MEDIA__TRDC_BASE) = CR_GVLDM;
}

static uint32_t HDL_S3MU_SendMessage(S3MU_Type *base, uint32_t *msg, uint8_t size)
{
    uint32_t i, idx;
    for (i = 0; i < size; i++)
    {
        idx = i % SENTMU_TR_COUNT;
        while (!((base->TSR) & (1u << idx)))
        {
        }
        base->TR[idx] = msg[i];
    }
    return (uint32_t)size;
}

static uint32_t HDL_S3MU_ReceiveMessage(S3MU_Type *base, uint32_t *resp, uint8_t size)
{
    uint32_t i, idx;
    if (size < 1)
    {
        return 0;
    }
    for (i = 0; i < size; i++)
    {
        idx = i % SENTMU_RR_COUNT;
        while (!(base->RSR & (1u << idx)))
        {
        }
        resp[i] = base->RR[idx];
    }
    return (uint32_t)size;
}

static void HDL_Release_RDC(void)
{
    uint32_t muCMD[2] = {0};
    uint32_t muRES[2] = {0};
    S3MU_Type *base   = MAIN__SENTMU0_SENTMUA;
    muCMD[0] = 0x17C40206;
    muCMD[1] = 0xAA01;
    HDL_S3MU_SendMessage(base, muCMD, sizeof(muCMD) / 4);
    HDL_S3MU_ReceiveMessage(base, muRES, sizeof(muRES) / 4);
}

static void TRDC_Config(void)
{
    HDL_Release_RDC();
    HDL_InitMediaTrdcMdac();
    HDL_EnableMediaTrdcMdac();
}

void MEDIA_TINGPU_DriverIRQHandler(void)
{
    vg_lite_irq_handler_ext();
}

static status_t BOARD_InitVGliteClock(void)
{
    CLOCK_EnableClock(kCLOCK_MEDIA_gpu);
    TRDC_Config();
    *(uint32_t *)0x50530018 = 0x000000ff;
    *(uint32_t *)0x4C0E0004 |= 0x2;
    *(uint32_t *)0x4C0E0004 = 0x0;
    *(uint32_t *)0x4C0E0008 = 0x1;
    NVIC_SetPriority(MEDIA_TINGPU_IRQn, 4);
    /* Clear any stale pending FlexGPU IRQ before enabling */
    NVIC_ClearPendingIRQ(MEDIA_TINGPU_IRQn);
    EnableIRQ(MEDIA_TINGPU_IRQn);
    return kStatus_Success;
}

status_t BOARD_PrepareVGLiteController(void)
{
    return BOARD_InitVGliteClock();
}

