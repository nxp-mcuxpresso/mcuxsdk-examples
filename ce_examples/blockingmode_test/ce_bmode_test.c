/*
 * Copyright 2024 NXP
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

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define Nmax 3600

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (defined(__ICCARM__))
uint32_t cmd_buffer[256] @ "dspvA";
uint32_t status_buffer[128 + 6] @ "dspvB";

float matA[Nmax] @ "dspvC";
float matB[Nmax] @ "dspvC";
float matC[Nmax] @ "dspvC";
float matC2[Nmax] @ "dspvC";
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
__attribute__((section("dspvA"), zero_init)) uint32_t cmd_buffer[256];
__attribute__((section("dspvB"), zero_init)) uint32_t status_buffer[128 + 6];

__attribute__((section("dspvC"), zero_init)) float matA[Nmax];
__attribute__((section("dspvC"), zero_init)) float matB[Nmax];
__attribute__((section("dspvC"), zero_init)) float matC[Nmax];
__attribute__((section("dspvC"), zero_init)) float matC2[Nmax];
#elif (defined(__GNUC__))
__attribute__((section(".dspvA,\"aw\",%nobits @"))) uint32_t cmd_buffer[256];
__attribute__((section(".dspvB,\"aw\",%nobits @"))) uint32_t status_buffer[128 + 6];

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

/*!
 * @brief Main function
 */
int main(void)
{
    int status, i;
    double copyerr = 0, copyerr2 = 0;
    int M, N, P;
    float *temp;

    BOARD_InitHardware();

    ///////////////////////////////////////////////////////////////////
    // One Command tests 
    ///////////////////////////////////////////////////////////////////
    CE_CmdInitBuffer(&ce_cmd_buffer, cmd_buffer, status_buffer, kCE_CmdModeOneBlocking);
    
    status = CE_NullCmd();
    PRINTF("Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    
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

    status = CE_MatrixMul_F32(matC, matA, matB, M, N, P);

    copyerr = 0;
    for (i = 0; i < M * P; i++)
    {
        copyerr += (double)((refOut[i] - matC[i]) * (refOut[i] - matC[i]));
    }
    
    if (copyerr > 1e-10)
        PRINTF("OneCmd Blocking F32 MAT MULT Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("OneCmd Blocking F32 MAT MULT Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    
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

    status = CE_MatrixMul_CF32(matC, matA, matB, M, N, P);
    
    copyerr = 0;
    for (i = 0; i < M * P * 2; i++)
    {
        copyerr += (double)((refOut[i] - matC[i]) * (refOut[i] - matC[i]));
    }

    if (copyerr > 1e-10)
        PRINTF("OneCmd Blocking CF32 MAT MULT Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("OneCmd Blocking CF32 MAT MULT Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    
    ///////////////////////////////////////////////////////////////////
    // Multi Command Queue tests 
    ///////////////////////////////////////////////////////////////////
    CE_CmdInitBuffer(&ce_cmd_buffer, cmd_buffer, status_buffer, kCE_CmdModeMultipleBlocking);
    
    for (i = 0; i < Nmax; i++)
    {
        temp    = (float *)&inputA[i];
        matA[i] = *temp;
    }

    for (i = 0; i < Nmax; i++)
    {
        temp    = (float *)&inputB[i];
        matB[i] = *temp;
    }

    status = CE_MatrixMul_F32(matC, matA, matB, 60, 60, 60);
    status = CE_MatrixMul_CF32(matC2, matA, matB, 40, 40, 40);
    CE_CmdLaunch(1);
    
    for (i = 0; i < 60 * 60; i++)
    {
        temp      = (float *)&outputC_ref[i];
        refOut[i] = *temp;
    }
    
    for (i = 0; i < 40 * 40 * 2; i++)
    {
        temp      = (float *)&outputC2_ref[i];
        refOut2[i] = *temp;
    }
    
    copyerr = 0;
    for (i = 0; i < 60*60; i++)
    {
        copyerr += (double)((refOut[i] - matC[i]) * (refOut[i] - matC[i]));
    }
    
    copyerr2 = 0;
    for (i = 0; i < 40*40*2; i++)
    {
        copyerr2 += (double)((refOut2[i] - matC2[i]) * (refOut2[i] - matC2[i]));
    }
    
    if ( (copyerr > 0) || (copyerr2 > 0) )
        PRINTF("Blocking Cmd Queue Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("Blocking Cmd Queue Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    
    while(1)
    {
      // just hang out
    }
}
