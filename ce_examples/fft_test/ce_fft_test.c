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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (defined(__ICCARM__))
uint32_t cmd_buffer[256] @ "dspvA";
uint32_t status_buffer[128 + 6] @ "dspvB";

float srcdata[1024] @ "dspvC";
float scratch[1024] @ "dspvC";
float dstdata[1024] @ "dspvC";
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
__attribute__((section("dspvA"), zero_init)) uint32_t cmd_buffer[256];
__attribute__((section("dspvB"), zero_init)) uint32_t status_buffer[128 + 6];

__attribute__((section("dspvC"), zero_init)) float srcdata[1024];
__attribute__((section("dspvC"), zero_init)) float scratch[1024];
__attribute__((section("dspvC"), zero_init)) float dstdata[1024];
#elif (defined(__GNUC__))
__attribute__((section(".dspvA,\"aw\",%nobits @"))) uint32_t cmd_buffer[256];
__attribute__((section(".dspvB,\"aw\",%nobits @"))) uint32_t status_buffer[128 + 6];

__attribute__((section(".dspvC,\"aw\",%nobits @"))) float srcdata[1024];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float scratch[1024];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float dstdata[1024];
#endif

float refdata[1024 * 2];

// Test 1
int fft32_cf16_input[32] = {
#include "fft32_cf16_input.txt"
};

int fft32_cf16_ref_output[32] = {
#include "fft32_cf16_ref_output.txt"
};

// Test 2
int fft512_cf32_input[1024] = {
#include "fft512_cf32_input.txt"
};

int fft512_cf32_ref_output[1024] = {
#include "fft512_cf32_ref_output.txt"
};

// Test 3
int ifft1024_cf16_input[1024] = {
#include "ifft1024_cf16_input.txt"
};

int ifft1024_cf16_ref_output[1024] = {
#include "ifft1024_cf16_ref_output.txt"
};

// Test 4
int ifft256_cf32_input[512] = {
#include "ifft256_cf32_input.txt"
};

int ifft256_cf32_ref_output[512] = {
#include "ifft256_cf32_ref_output.txt"
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
    float copyerr = 0;
    int N, log2N;
    float *temp;

    BOARD_InitHardware();

    CE_CmdInitBuffer(&ce_cmd_buffer, cmd_buffer, status_buffer, kCE_CmdModeOneBlocking);

    status = CE_NullCmd();
    PRINTF("Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    // Test 1: CF16 32pt FFT Test
    N     = 32;
    log2N = 5;
    for (i = 0; i < N; i++)
    {
        temp       = (float *)&fft32_cf16_input[i];
        srcdata[i] = *temp;
        temp       = (float *)&fft32_cf16_ref_output[i];
        refdata[i] = *temp;
    }

    status = CE_TransformCFFT_F16(dstdata, srcdata, scratch, log2N);

    copyerr = 0;
    for (i = 0; i < N; i++)
    {
        copyerr += (refdata[i] - dstdata[i]) * (refdata[i] - dstdata[i]);
    }

    if (copyerr > 0)
        PRINTF("Test 1: CFFT 32 F16 Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("Test 1: CFFT 32 F16 Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    // Test 2: CF32 512pt FFT Test
    N     = 512;
    log2N = 9;
    for (i = 0; i < 2 * N; i++)
    {
        temp       = (float *)&fft512_cf32_input[i];
        srcdata[i] = *temp;
        temp       = (float *)&fft512_cf32_ref_output[i];
        refdata[i] = *temp;
    }

    status = CE_TransformCFFT_F32(dstdata, srcdata, scratch, log2N);

    copyerr = 0;
    for (i = 0; i < 2 * N; i++)
    {
        copyerr += (refdata[i] - dstdata[i]) * (refdata[i] - dstdata[i]);
    }

    if (copyerr > 0)
        PRINTF("Test 2: CFFT 512 F32 Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("Test 2: CFFT 512 F32 Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    // Test 3: CF16 1024pt IFFT Test
    N     = 1024;
    log2N = 10;
    for (i = 0; i < N; i++)
    {
        temp       = (float *)&ifft1024_cf16_input[i];
        srcdata[i] = *temp;
        temp       = (float *)&ifft1024_cf16_ref_output[i];
        refdata[i] = *temp;
    }

    status = CE_TransformIFFT_F16(dstdata, srcdata, scratch, log2N);

    copyerr = 0;
    for (i = 0; i < N; i++)
    {
        copyerr += (refdata[i] - dstdata[i]) * (refdata[i] - dstdata[i]);
    }

    if (copyerr > 0)
        PRINTF("Test 3: IFFT 1024 F16 Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("Test 3: IFFT 1024 F16 Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    // Test 4: CF32 32pt IFFT Test
    N     = 256;
    log2N = 8;
    for (i = 0; i < 2 * N; i++)
    {
        temp       = (float *)&ifft256_cf32_input[i];
        srcdata[i] = *temp;
        temp       = (float *)&ifft256_cf32_ref_output[i];
        refdata[i] = *temp;
    }

    status = CE_TransformIFFT_F32(dstdata, srcdata, scratch, log2N);

    copyerr = 0;
    for (i = 0; i < 2 * N; i++)
    {
        copyerr += (refdata[i] - dstdata[i]) * (refdata[i] - dstdata[i]);
    }

    if (copyerr > 0)
        PRINTF("Test 4: IFFT 256 F32 Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("Test 4: IFFT 256 F32 Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    while (1)
    {
        // just hang out
    }
}
