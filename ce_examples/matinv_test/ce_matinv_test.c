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

float matA[3200] @ "dspvC";
float matAout[3200] @ "dspvC";
float scratch[6402] @ "dspvC";
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
__attribute__((section("dspvA"), zero_init)) uint32_t cmd_buffer[256];
__attribute__((section("dspvB"), zero_init)) uint32_t status_buffer[128 + 6];

__attribute__((section("dspvC"), zero_init)) float matA[3200];
__attribute__((section("dspvC"), zero_init)) float matAout[3200];
__attribute__((section("dspvC"), zero_init)) float scratch[6402];
#elif (defined(__GNUC__))
__attribute__((section(".dspvA,\"aw\",%nobits @"))) uint32_t cmd_buffer[256];
__attribute__((section(".dspvB,\"aw\",%nobits @"))) uint32_t status_buffer[128 + 6];

__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matA[3200];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matAout[3200];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float scratch[6402];
#endif

float refOut[3200];

int inputInv[3600] = {
#include "matinv_18_cf32_input.txt"
};

int inputInvPck[1640] = {
#include "matinv_18_cf32_input_pck.txt"
};

int outputInv_ref[3600] = {
#include "matinv_18_cf32_ref_output.txt"
};

int outputLInv_ref[3600] = {
#include "matinv_18_cf32_ref_Vout.txt"
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
    int M, Mc;
    float *temp;
    matAout[0] = 1.0;
    int packIn = 1;
    int Linv   = 1;

    BOARD_InitHardware();

    CE_CmdInitBuffer(&ce_cmd_buffer, cmd_buffer, status_buffer, kCE_CmdModeOneBlocking);

    status = CE_NullCmd();
    PRINTF("Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    // MAT Inv Test
    M  = 18;
    Mc = (M * (1 + M)) / 2;

    if (packIn == 0)
    {
        for (i = 0; i < M * M * 2; i++)
        {
            temp    = (float *)&inputInv[i];
            matA[i] = *temp;
        }
    }
    else
    {
        for (i = 0; i < Mc * 2; i++)
        {
            temp    = (float *)&inputInvPck[i];
            matA[i] = *temp;
        }
    }

    if (Linv == 0)
    {
        for (i = 0; i < Mc * 2; i++)
        {
            temp      = (float *)&outputInv_ref[i];
            refOut[i] = *temp;
        }
    }
    else
    {
        for (i = 0; i < Mc * 2; i++)
        {
            temp      = (float *)&outputLInv_ref[i];
            refOut[i] = *temp;
        }
    }

    status = CE_MatrixInvHerm_CF32(matAout, matA, scratch, M, packIn, Linv);

    copyerr = 0;
    for (i = 0; i < Mc * 2; i++)
    {
        copyerr += (double)((refOut[i] - matAout[i]) * (refOut[i] - matAout[i]));
    }

    if (copyerr > 1e-7)
        PRINTF("CF32 MAT Inv Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("CF32 MAT Inv Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    while (1)
    {
        // just hang out
    }
}
