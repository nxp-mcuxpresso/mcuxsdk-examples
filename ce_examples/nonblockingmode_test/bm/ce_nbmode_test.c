/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
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
#define Nmax 3600

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
int status_buffer[128 + 6] @ "dspvB";

float matA[Nmax] @ "dspvC";
float matB[Nmax] @ "dspvC";
float matC[Nmax] @ "dspvC";
float matC2[Nmax] @ "dspvC";
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
__attribute__((section("dspvA"), zero_init)) uint32_t cmd_buffer[256];
__attribute__((section("dspvB"), zero_init)) int status_buffer[128 + 6];

__attribute__((section("dspvC"), zero_init)) float matA[Nmax];
__attribute__((section("dspvC"), zero_init)) float matB[Nmax];
__attribute__((section("dspvC"), zero_init)) float matC[Nmax];
__attribute__((section("dspvC"), zero_init)) float matC2[Nmax];
#elif (defined(__GNUC__))
__attribute__((section(".dspvA,\"aw\",%nobits @"))) uint32_t cmd_buffer[256];
__attribute__((section(".dspvB,\"aw\",%nobits @"))) int status_buffer[128 + 6];

__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matA[Nmax];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matB[Nmax];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matC[Nmax];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matC2[Nmax];
#endif

float refOut[Nmax];
float refOut2[Nmax];

int inputA[Nmax] = {
#include "mat_mult_input_A.txt"
};

int inputB[Nmax] = {
#include "mat_mult_input_B.txt"
};

int outputC_ref[Nmax] = {
#include "mat_multf32_60x60x60_C_ref.txt"
};

int outputC2_ref[Nmax] = {
#include "mat_multcf32_40x40x40_C_ref.txt"
};

ce_cmdbuffer_t ce_cmd_buffer;
/*******************************************************************************
 * Code
 ******************************************************************************/
static OSA_TASK_HANDLE_DEFINE(LCE_Task1_TaskId);

static void LCE_Task1(void *param)
{
    int status, i;
    double copyerr = 0;
    int M, N, P;
    float *temp;

    PRINTF("LCE task1 start\r\n");

    // F32 A[60x60] * B[60x60]
    M = 60;
    N = 60;
    P = 60;
    for (i = 0; i < M * N; i++)
    {
        temp    = (float *)&inputA[i];
        matA[i] = *temp;
    }

    for (i = 0; i < N * P; i++)
    {
        temp    = (float *)&inputB[i];
        matB[i] = *temp;
    }

    for (i = 0; i < M * P; i++)
    {
        temp      = (float *)&outputC_ref[i];
        refOut[i] = *temp;
    }

    status = LCE_MatrixMul_F32(matC, matA, matB, M, N, P);

    copyerr = 0;
    for (i = 0; i < M * P; i++)
    {
        copyerr += (double)((refOut[i] - matC[i]) * (refOut[i] - matC[i]));
    }
    
    if (copyerr > 1e-10)
        PRINTF("F32 MAT MULT Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("F32 MAT MULT Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    
    OSA_TaskDestroy(LCE_Task1_TaskId);
}

static OSA_TASK_DEFINE(LCE_Task1, gLCE_Task1_TaskPriority_c, 1, gLCE_Task1_TaskStackSize_c, (uint8_t)false);

static OSA_TASK_HANDLE_DEFINE(LCE_Task2_TaskId);

static void LCE_Task2(void *param)
{
    int status, i;
    double copyerr = 0;
    int M, N, P;
    float *temp;

    PRINTF("LCE task2 start\r\n");

    // CF32 A[40x40] * B[40x40]
    M = 40;
    N = 40;
    P = 40;
    for (i = 0; i < M * N * 2; i++)
    {
        temp    = (float *)&inputA[i];
        matA[i] = *temp;
    }

    for (i = 0; i < N * P * 2; i++)
    {
        temp    = (float *)&inputB[i];
        matB[i] = *temp;
    }

    for (i = 0; i < M * P * 2; i++)
    {
        temp      = (float *)&outputC2_ref[i];
        refOut[i] = *temp;
    }

    status = LCE_MatrixMul_CF32(matC, matA, matB, M, N, P);

    copyerr = 0;
    for (i = 0; i < M * P * 2; i++)
    {
        copyerr += (double)((refOut[i] - matC[i]) * (refOut[i] - matC[i]));
    }

    if (copyerr > 1e-10)
        PRINTF("CF32 MAT MULT Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("CF32 MAT MULT Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    
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

    PRINTF("CE non-blocking mode test using the LCE component\r\n");

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
