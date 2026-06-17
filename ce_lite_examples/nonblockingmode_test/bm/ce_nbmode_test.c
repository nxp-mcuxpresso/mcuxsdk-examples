/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_lpuart.h"
#include "fsl_mu.h"
#include "fsl_ce.h"
#include "fsl_ce_cmd.h"
#include "fsl_os_abstraction.h"
#include "fsl_component_lce.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef gLCE_Task1_TaskPriority_c
#define gLCE_Task1_TaskPriority_c        1
#endif /* gLCE_Task1_TaskPriority_c */
#ifndef gLCE_Task1_TaskStackSize_c
#define gLCE_Task1_TaskStackSize_c       1024
#endif /* gLCE_Task1_TaskStackSize_c */

#ifndef gLCE_Task2_TaskPriority_c
#define gLCE_Task2_TaskPriority_c        1
#endif /* gLCE_Task2_TaskPriority_c */
#ifndef gLCE_Task2_TaskStackSize_c
#define gLCE_Task2_TaskStackSize_c       1024
#endif /* gLCE_Task2_TaskStackSize_c */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (defined(__ICCARM__))
uint32_t cmd_buffer[256] @ "dspvA";
int32_t status_buffer[128 + 6] @ "dspvB";

float srcdata_cf32[1024] @ "dspvC";
float dstdata_cf32[1024] @ "dspvC";
float scratch[1024] @ "dspvC";
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
__attribute__((section("dspvA"), zero_init)) uint32_t cmd_buffer[256];
__attribute__((section("dspvB"), zero_init)) int32_t status_buffer[128 + 6];

__attribute__((section("dspvC"), zero_init)) float srcdata_cf32[1024];
__attribute__((section("dspvC"), zero_init)) float dstdata_cf32[1024];
__attribute__((section("dspvC"), zero_init)) float scratch[1024];
#elif (defined(__GNUC__))
__attribute__((section(".dspvA,\"aw\",%nobits @"))) uint32_t cmd_buffer[256];
__attribute__((section(".dspvB,\"aw\",%nobits @"))) int32_t status_buffer[128 + 6];

__attribute__((section(".dspvC,\"aw\",%nobits @"))) float srcdata_cf32[1024];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float dstdata_cf32[1024];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float scratch[1024];
#endif

float refdata_cf32[1024 * 2];

int fft512_cf32_input[1024] = {
#include "fft512_cf32_input.txt"
};

int fft512_cf32_ref_output[1024] = {
#include "fft512_cf32_ref_output.txt"
};

ce_cmdbuffer_t ce_cmd_buffer;
/*******************************************************************************
 * Code
 ******************************************************************************/
static OSA_TASK_HANDLE_DEFINE(LCE_Task1_TaskId);

static void LCE_Task1(void *param)
{
    int status, i;
    float copyerr = 0;
    float pwr     = 0;
    int N, log2N;
    float *temp;

    PRINTF("LCE task1 start\r\n");

    N     = 512;
    log2N = 9;
    for (i = 0; i < 2 * N; i++)
    {
        temp            = (float *)&fft512_cf32_input[i];
        srcdata_cf32[i] = *temp;
        temp            = (float *)&fft512_cf32_ref_output[i];
        refdata_cf32[i] = *temp;
    }

    status = LCE_TransformCFFT_F32(dstdata_cf32, srcdata_cf32, scratch, log2N);

    copyerr = 0;
    pwr     = 0;
    for (i = 0; i < 2 * N; i++)
    {
        copyerr += (refdata_cf32[i] - dstdata_cf32[i]) * (refdata_cf32[i] - dstdata_cf32[i]);
        pwr += refdata_cf32[i] * refdata_cf32[i];
    }
    copyerr = copyerr / pwr;

    if (copyerr > 1e-10f)
        PRINTF("CFFT 512 F32 Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("CFFT 512 F32 Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

    OSA_TaskDestroy(LCE_Task1_TaskId);
}

static OSA_TASK_DEFINE(LCE_Task1, gLCE_Task1_TaskPriority_c, 1, gLCE_Task1_TaskStackSize_c, (uint8_t)false);

static OSA_TASK_HANDLE_DEFINE(LCE_Task2_TaskId);

static void LCE_Task2(void *param)
{
    int status, i;
    float copyerr = 0;
    float pwr     = 0;
    int N, log2N;
    float *temp;

    PRINTF("LCE task2 start\r\n");

    N     = 512;
    log2N = 9;
    for (i = 0; i < 2 * N; i++)
    {
        temp            = (float *)&fft512_cf32_input[i];
        srcdata_cf32[i] = *temp;
        temp            = (float *)&fft512_cf32_ref_output[i];
        refdata_cf32[i] = *temp;
    }

    status = LCE_TransformCFFT_F32(dstdata_cf32, srcdata_cf32, scratch, log2N);

    copyerr = 0;
    pwr     = 0;
    for (i = 0; i < 2 * N; i++)
    {
        copyerr += (refdata_cf32[i] - dstdata_cf32[i]) * (refdata_cf32[i] - dstdata_cf32[i]);
        pwr += refdata_cf32[i] * refdata_cf32[i];
    }
    copyerr = copyerr / pwr;

    if (copyerr > 1e-10f)
        PRINTF("CFFT 512 F32 Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("CFFT 512 F32 Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

    OSA_TaskDestroy(LCE_Task2_TaskId);
}

static OSA_TASK_DEFINE(LCE_Task2, gLCE_Task2_TaskPriority_c, 1, gLCE_Task2_TaskStackSize_c, (uint8_t)false);

/*!
 * @brief Main function
 */
int main(void)
{
    osa_status_t osaStatus;

    BOARD_InitHardware();

    PRINTF("CE lite non-blocking mode test using the LCE component\r\n");

    osaStatus = LCE_Init();

    if (KOSA_StatusSuccess != osaStatus)
    {
        PRINTF("Failed to initialize the LCE component\r\n");
        assert(0);
        return 1;
    }

    /* Init OSA: should be called before any other OSA API */
    OSA_Init();

    CE_CmdInitBuffer(&ce_cmd_buffer, cmd_buffer, status_buffer, kCE_CmdModeOneNonBlocking);

    osaStatus = OSA_TaskCreate((osa_task_handle_t)LCE_Task1_TaskId, OSA_TASK(LCE_Task1), NULL);
    if (KOSA_StatusSuccess != osaStatus)
    {
        PRINTF("Failed to create Task1\r\n");
        assert(0);
        return 1;
    }

    osaStatus = OSA_TaskCreate((osa_task_handle_t)LCE_Task2_TaskId, OSA_TASK(LCE_Task2), NULL);
    if (KOSA_StatusSuccess != osaStatus)
    {
        PRINTF("Failed to create Task2\r\n");
        assert(0);
        return 1;
    }

    OSA_Start();

    /* Won't run here */
    assert(0);
    return 0;
}
