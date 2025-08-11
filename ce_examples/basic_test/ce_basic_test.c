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

float srcdata[25][2] @ "dspvC";
float srcdata2[25][2] @ "dspvC";
float dstdata[25][2] @ "dspvC";

int32_t srcdataintA[80] @ "dspvC";
int srcdataintA2[80] @ "dspvC";
int32_t dstdata_int[80] @ "dspvC";

int16_t srcdataintB[80] @ "dspvC";
int16_t srcdataintB2[80] @ "dspvC";
int16_t dstdata_int16[80] @ "dspvC";

#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
__attribute__((section("dspvA"), zero_init)) uint32_t cmd_buffer[256];
__attribute__((section("dspvB"), zero_init)) uint32_t status_buffer[128 + 6];

__attribute__((section("dspvC"), zero_init)) float srcdata[25][2];
__attribute__((section("dspvC"), zero_init)) float srcdata2[25][2];
__attribute__((section("dspvC"), zero_init)) float dstdata[25][2];

__attribute__((section("dspvC"), zero_init)) int32_t srcdataintA[80] @ "dspvC";
__attribute__((section("dspvC"), zero_init)) int32_t srcdataintA2[80] @ "dspvC";
__attribute__((section("dspvC"), zero_init)) int32_t dstdata_int[80] @ "dspvC";

__attribute__((section("dspvC"), zero_init)) int16_t srcdataintB[80] @ "dspvC";
__attribute__((section("dspvC"), zero_init)) int16_t srcdataintB2[80] @ "dspvC";
__attribute__((section("dspvC"), zero_init)) int16_t dstdata_int16[80] @ "dspvC";
#elif (defined(__GNUC__))
__attribute__((section(".dspvA,\"aw\",%nobits @"))) uint32_t cmd_buffer[256];
__attribute__((section(".dspvB,\"aw\",%nobits @"))) uint32_t status_buffer[128 + 6];

__attribute__((section(".dspvC,\"aw\",%nobits @"))) float srcdata[25][2];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float srcdata2[25][2];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float dstdata[25][2];

__attribute__((section(".dspvC,\"aw\",%nobits @"))) int32_t srcdataintA[80];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) int32_t srcdataintA2[80];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) int32_t dstdata_int[80];

__attribute__((section(".dspvC,\"aw\",%nobits @"))) int16_t srcdataintB[80];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) int16_t srcdataintB2[80];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) int16_t dstdata_int16[80];
#endif

float refdata[25][2];
int refdata_int[80];
int16_t refdata_int16[80];

ce_cmdbuffer_t ce_cmd_buffer;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    int status, i, j;
    float copyerr = 0;

    BOARD_InitHardware();

    CE_CmdInitBuffer(&ce_cmd_buffer, cmd_buffer, status_buffer, kCE_CmdModeOneBlocking);

    for (i = 0; i < 25; i++)
    {
        for (j = 0; j < 2; j++)
        {
            srcdata[i][j]  = (float)(25 * i + j);
            srcdata2[i][j] = (float)(100 * i + j);
        }
    }

    for (i = 0; i < 80; i++)
    {
        srcdataintA[i] = 25 * i + 1;
        srcdataintA2[i] = 100 * i + 3;

        srcdataintB[i] = 15 * i + 4;
        srcdataintB2[i] = 10 * i + 7;
    }

    status = CE_NullCmd();
    PRINTF("Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

    for (i = 0; i < 25; i++)
    {
        for (j = 0; j < 2; j++)
            refdata[i][j] = srcdata[i][j] + srcdata2[i][j];
    }

    status = CE_MatrixAdd_F32(&dstdata[0][0], &srcdata[0][0], &srcdata2[0][0], 25, 2);

    copyerr = 0;
    for (i = 0; i < 25; i++)
    {
        for (j = 0; j < 2; j++)
        {
            copyerr += (refdata[i][j] - dstdata[i][j]) * (refdata[i][j] - dstdata[i][j]);
        }
    }

    if (copyerr > 0)
        PRINTF("VECADD F32 Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("VECADD F32 Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

    for (i = 0; i < 80; i++)
    {
        refdata_int[i] = srcdataintA[i] + srcdataintA2[i];
    }

    status = CE_MatrixAdd_Q31(&dstdata_int[0], &srcdataintA[0], &srcdataintA2[0], 80, 1);

    copyerr = 0;
    for (i = 0; i < 80; i++)
    {
        copyerr += (refdata_int[i] - dstdata_int[i]) * (refdata_int[i] - dstdata_int[i]);
    }

    if (copyerr > 0)
        PRINTF("VECADD Q31 Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("VECADD Q31 Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    for (i = 0; i < 80; i++)
    {
        refdata_int16[i] = srcdataintB[i] + srcdataintB2[i];
    }

    status = CE_MatrixAdd_Q15(&dstdata_int16[0], &srcdataintB[0], &srcdataintB2[0], 80, 1);

    copyerr = 0;
    for (i = 0; i < 80; i++)
    {
        copyerr += (refdata_int16[i] - dstdata_int16[i]) * (refdata_int16[i] - dstdata_int16[i]);
    }

    if (copyerr > 0)
        PRINTF("VECADD Q15 Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("VECADD Q15 Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    for (i = 0; i < 25; i++)
    {
        for (j = 0; j < 2; j++)
        {
            refdata[i][j] = srcdata[i][j] * srcdata2[i][j];
        }
    }

    status = CE_MatrixElemMul_F32(&dstdata[0][0], &srcdata[0][0], &srcdata2[0][0], 25, 2);

    copyerr = 0;
    for (i = 0; i < 25; i++)
    {
        for (j = 0; j < 2; j++)
        {
            copyerr += (refdata[i][j] - dstdata[i][j]) * (refdata[i][j] - dstdata[i][j]);
        }
    }

    if (copyerr > 0)
        PRINTF("VECMULT Test Failed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    else
        PRINTF("VECMULT Test Passed: Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);

    while (1)
    {
        // just hang out
    }
}
