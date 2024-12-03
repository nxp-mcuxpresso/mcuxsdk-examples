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
// #define STANDALONERUN

#define MAXSIZE             40
#define FINALQ_BUFFER       0
#define TOG_BUFFER1         (40 * 40)
#define TOG_BUFFER2         (40 * 40 * 2)
#define A_OFFSET            (40 * 40 * 3 + 0)
#define B_OFFSET            (40 * 40 * 3 + 1)
#define S_OFFSET            (40 * 40 * 3 + 2)
#define C_OFFSET            (40 * 40 * 3 + 3)
#define TOL_OFFSET          (40 * 40 * 3 + 8)
#define ZEROMULCONST_OFFSET (40 * 40 * 3 + 9)
#define TOLCOND_OFFSET      (40 * 40 * 3 + 10)
#define TOLCONDINT_OFFSET   (40 * 40 * 3 + 10 + 40)

#define TEST_TYPE 0
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
float matL[40] @ "dspvC";
float scratch[40 * 40 * 2 * 2 + 360] @ "dspvC";
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
__attribute__((section("dspvA"), zero_init)) uint32_t cmd_buffer[256];
__attribute__((section("dspvB"), zero_init)) uint32_t status_buffer[128 + 6];

__attribute__((section("dspvC"), zero_init)) float matA[3200];
__attribute__((section("dspvC"), zero_init)) float matAout[3200];
__attribute__((section("dspvC"), zero_init)) float matL[40];
__attribute__((section("dspvC"), zero_init)) float scratch[40 * 40 * 2 * 2 + 360];
#elif (defined(__GNUC__))
__attribute__((section(".dspvA,\"aw\",%nobits @"))) uint32_t cmd_buffer[256];
__attribute__((section(".dspvB,\"aw\",%nobits @"))) uint32_t status_buffer[128 + 6];

__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matA[3200];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matAout[3200];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matL[40];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float scratch[40 * 40 * 2 * 2 + 360];
#endif

float refOut[3200];
float refOutTc[40 * 2];
float refOutB[40];

#define EVDSZ 40
int inputEvd[3200] = {
#if TEST_TYPE == 0
#include "evd_input_comp_wgn_40.txt"
#else
#include "evd_input_comp_loc_40.txt"
#endif
};

int inputBeta[40];

int outputVm_int[3200];

int outputEvd_ref[3200] = {
#if TEST_TYPE == 0
#include "qr_output_U_wgn_40.txt"
#else
#include "qr_output_U_loc_40.txt"
#endif
};

int outputTc_ref[40] = {
#if TEST_TYPE == 0
#include "qr_output_T_wgn_40.txt"
#else
#include "qr_output_T_loc_40.txt"
#endif
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
    float evd_tol;
    int evd_max_iter;
    int M;
    float *temp;
    matAout[0] = 1.0;
    matL[0]    = 1.0;
    scratch[0] = 0;

    BOARD_InitHardware();

    CE_CmdInitBuffer(&ce_cmd_buffer, cmd_buffer, status_buffer, kCE_CmdModeOneBlocking);

    status = CE_NullCmd();
    PRINTF("Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    // MAT EVD Test
    M = EVDSZ;
    for (i = 0; i < M * M * 2; i++)
    {
        temp    = (float *)&inputEvd[i];
        matA[i] = *temp;

        temp      = (float *)&outputEvd_ref[i];
        refOut[i] = *temp;
    }

    for (i = 0; i < M; i++)
    {
        temp       = (float *)&outputTc_ref[i];
        refOutB[i] = *temp;
    }

    evd_tol      = 1.0000e-11;
    evd_max_iter = 100;
    CE_MatrixEvdHerm_CF32(matL, matAout, matA, scratch, M, evd_tol, evd_max_iter, 1);

    copyerr = 0;
    for (i = 0; i < M * M * 2; i++)
    {
        copyerr += (double)((refOut[i] - matAout[i]) * (refOut[i] - matAout[i]));
    }

    copyerr2 = 0;
    for (i = 0; i < M; i++)
    {
        copyerr2 += (double)((refOutB[i] - matL[i]) * (refOutB[i] - matL[i]));
    }

    if ((copyerr > 1e-7) || (copyerr2 > 1e-7))
        PRINTF("CF32 MAT EVD Test Failed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);
    else
        PRINTF("CF32 MAT EVD Test Passed: Status=%8X, Reply=%8X\n", status, status_buffer[0]);

    while (1)
    {
        // just hang out
    }
}
