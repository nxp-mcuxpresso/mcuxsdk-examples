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

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (defined(__ICCARM__))
uint32_t cmd_buffer[256] @ "dspvA";
uint32_t status_buffer[128 + 6] @ "dspvB";

float matA[3600] @ "dspvC";
float matB[3600] @ "dspvC";
float matC[3600] @ "dspvC";
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
__attribute__((section("dspvA"), zero_init)) uint32_t cmd_buffer[256];
__attribute__((section("dspvB"), zero_init)) uint32_t status_buffer[128 + 6];

__attribute__((section("dspvC"), zero_init)) float matA[3600];
__attribute__((section("dspvC"), zero_init)) float matB[3600];
__attribute__((section("dspvC"), zero_init)) float matC[3600];
#elif (defined(__GNUC__))
__attribute__((section(".dspvA,\"aw\",%nobits @"))) uint32_t cmd_buffer[256];
__attribute__((section(".dspvB,\"aw\",%nobits @"))) uint32_t status_buffer[128 + 6];

__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matA[3600];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matB[3600];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matC[3600];
#endif

float refOut[3600];

int inputA[3600] = {
#include "mat_mult_input_A.txt"
};

int inputB[3600] = {
#include "mat_mult_input_B.txt"
};

int outputC_ref[3600] = {
#include "mat_multf32_60x60x60_C_ref.txt"
};

int outputC2_ref[3600] = {
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
    double copyerr = 0;
    int M, N, P;
    float *temp;

    BOARD_InitHardware();

    CE_CmdInitBuffer(&ce_cmd_buffer, cmd_buffer, status_buffer, kCE_CmdModeOneBlocking);

    status = CE_NullCmd();
    PRINTF("Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

    // F32 A[2x1] * Ones[1x3]
    temp = (float *)&inputA[0];
    matA[0] = *temp;
    temp = (float *)&inputA[1];
    matA[1] = *temp;

    matB[0] = 1.0;
    matB[1] = 1.0;
    matB[2] = 1.0;

    refOut[0] = matA[0];
    refOut[1] = matA[0];
    refOut[2] = matA[0];
    refOut[3] = matA[1];
    refOut[4] = matA[1];
    refOut[5] = matA[1];

    status = CE_MatrixMul_F32(matC, matA, matB, 2, 1, 3);

    copyerr = 0;
    for (i = 0; i < 6; i++)
    {
        copyerr += (double)((refOut[i] - matC[i]) * (refOut[i] - matC[i]));
    }

    if (copyerr > 1e-10)
        PRINTF("F32 2x1 x 1x3 MAT MULT Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("F32 2x1 x 1x3 MAT MULT Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

    // F32 A[2x2] * Identity[2x2]
    for (i = 0; i < 4; i++)
    {
      temp  = (float *)&inputA[i];
      matA[i] = *temp;
      refOut[i] = *temp;
    }
    matB[0] = 1.0;
    matB[1] = 0;
    matB[2] = 0;
    matB[3] = 1.0;

    status = CE_MatrixMul_F32(matC, matA, matB, 2, 2, 2);

    copyerr = 0;
    for (i = 0; i < 4; i++)
    {
        copyerr += (double)((refOut[i] - matC[i]) * (refOut[i] - matC[i]));
    }

    if (copyerr > 1e-10)
        PRINTF("F32 2x2 MAT MULT Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("F32 2x2 MAT MULT Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

    // CF32 A[2x1] * Ones[1x3]
    temp = (float *)&inputA[0];
    matA[0] = *temp;
    temp = (float *)&inputA[1];
    matA[1] = *temp;
    temp = (float *)&inputA[2];
    matA[2] = *temp;
    temp = (float *)&inputA[3];
    matA[3] = *temp;

    matB[0] = 1.0;
    matB[1] = 0;
    matB[2] = 1.0;
    matB[3] = 0;
    matB[4] = 1.0;
    matB[5] = 0;

    refOut[0] = matA[0];
    refOut[1] = matA[1];
    refOut[2] = matA[0];
    refOut[3] = matA[1];
    refOut[4] = matA[0];
    refOut[5] = matA[1];

    refOut[6] = matA[2];
    refOut[7] = matA[3];
    refOut[8] = matA[2];
    refOut[9] = matA[3];
    refOut[10] = matA[2];
    refOut[11] = matA[3];

    status = CE_MatrixMul_CF32(matC, matA, matB, 2, 1, 3);

    copyerr = 0;
    for (i = 0; i < 6*2; i++)
    {
        copyerr += (double)((refOut[i] - matC[i]) * (refOut[i] - matC[i]));
    }

    if (copyerr > 1e-10)
        PRINTF("CF32 2x1 x 1x3 MAT MULT Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("CF32 2x1 x 1x3 MAT MULT Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

    // CF32 A[2x2] * Identity[2x2]
    for (i = 0; i < 4*2; i++)
    {
      temp = (float *)&inputA[i];
      matA[i] = *temp;
      refOut[i] = *temp;
    }
    matB[0] = 1.0;
    matB[1] = 0;
    matB[2] = 0;
    matB[3] = 0;
    matB[4] = 0;
    matB[5] = 0;
    matB[6] = 1.0;
    matB[7] = 0;

    status = CE_MatrixMul_CF32(matC, matA, matB, 2, 2, 2);

    copyerr = 0;
    for (i = 0; i < 4*2; i++)
    {
        copyerr += (double)((refOut[i] - matC[i]) * (refOut[i] - matC[i]));
    }

    if (copyerr > 1e-10)
        PRINTF("CF32 2x2 MAT MULT Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("CF32 2x2 MAT MULT Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

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
        PRINTF("F32 MAT MULT Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("F32 MAT MULT Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

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
        PRINTF("CF32 MAT MULT Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("CF32 MAT MULT Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

    while (1)
    {
        // just hang out
    }
}
