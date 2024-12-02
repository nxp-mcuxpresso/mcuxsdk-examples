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

float srcdata[25][2] @ "dspvC";
float srcdata2[25][2] @ "dspvC";
float dstdata[25][2] @ "dspvC";
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
__attribute__((section("dspvA"), zero_init)) uint32_t cmd_buffer[256];
__attribute__((section("dspvB"), zero_init)) uint32_t status_buffer[128 + 6];

__attribute__((section("dspvC"), zero_init)) float srcdata[25][2];
__attribute__((section("dspvC"), zero_init)) float srcdata2[25][2];
__attribute__((section("dspvC"), zero_init)) float dstdata[25][2];
#elif (defined(__GNUC__))
__attribute__((section(".dspvA,\"aw\",%nobits @"))) uint32_t cmd_buffer[256];
__attribute__((section(".dspvB,\"aw\",%nobits @"))) uint32_t status_buffer[128 + 6];

__attribute__((section(".dspvC,\"aw\",%nobits @"))) float srcdata[25][2];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float srcdata2[25][2];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float dstdata[25][2];
#endif

float refdata[25][2];

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

    status = CE_NullCmd();
    PRINTF("Status=%8X, Reply=%8X\n", status, status_buffer[0]);

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
        PRINTF("VECADD Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("VECADD Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    for (i = 0; i < 25; i++)
    {
        for (j = 0; j < 2; j++)
            refdata[i][j] = srcdata[i][j] * srcdata2[i][j];
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
        PRINTF("VECMULT Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("VECMULT Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    while (1)
    {
        // just hang out
    }
}
