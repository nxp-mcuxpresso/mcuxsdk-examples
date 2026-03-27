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
int32_t status_buffer[128 + 6] @ "dspvB";

float matA[20 * 20 * 2] @ "dspvC";
float matAout[20 * 20 * 2] @ "dspvC";
float matL[20] @ "dspvC";
float scratch[20 * 20 * 2 * 2 + 360] @ "dspvC";
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
__attribute__((section("dspvA"), zero_init)) uint32_t cmd_buffer[256];
__attribute__((section("dspvB"), zero_init)) int32_t status_buffer[128 + 6];

__attribute__((section("dspvC"), zero_init)) float matA[20 * 20 * 2];
__attribute__((section("dspvC"), zero_init)) float matAout[20 * 20 * 2];
__attribute__((section("dspvC"), zero_init)) float matL[20];
__attribute__((section("dspvC"), zero_init)) float scratch[20 * 20 * 2 * 2 + 360];
#elif (defined(__GNUC__))
__attribute__((section(".dspvA,\"aw\",%nobits @"))) uint32_t cmd_buffer[256];
__attribute__((section(".dspvB,\"aw\",%nobits @"))) int32_t status_buffer[128 + 6];

__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matA[20 * 20 * 2];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matAout[20 * 20 * 2];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float matL[20];
__attribute__((section(".dspvC,\"aw\",%nobits @"))) float scratch[20 * 20 * 2 * 2 + 360];
#endif

float refOut[20 * 20 * 2];
float refOutTc[20 * 2];
float refOutB[20];

// EVD SIZE 17 files
int inputEvd_comp_wgn_17[153 * 2] = {
#include "evd_input_comp_wgn_17.txt"
};

int inputEvd_comp_loc_17[153 * 2] = {
#include "evd_input_comp_loc_17.txt"
};

int inputEvd_full_wgn_17[289 * 2] = {
#include "evd_input_rm_wgn_17.txt"
};

int inputEvd_full_loc_17[289 * 2] = {
#include "evd_input_rm_loc_17.txt"
};

int outputEvd_ref_wgn_17[289 * 2] = {
#include "qr_output_U_wgn_17.txt"
};

int outputEvd_ref_loc_17[289 * 2] = {
#include "qr_output_U_loc_17.txt"
};

int outputTc_ref_wgn_17[17] = {
#include "qr_output_T_wgn_17.txt"
};

int outputTc_ref_loc_17[17] = {
#include "qr_output_T_loc_17.txt"
};

// EVD SIZE 18 files
int inputEvd_comp_wgn_18[171 * 2] = {
#include "evd_input_comp_wgn_18.txt"
};

int inputEvd_comp_loc_18[171 * 2] = {
#include "evd_input_comp_loc_18.txt"
};

int inputEvd_full_wgn_18[324 * 2] = {
#include "evd_input_rm_wgn_18.txt"
};

int inputEvd_full_loc_18[324 * 2] = {
#include "evd_input_rm_loc_18.txt"
};

int outputEvd_ref_wgn_18[324 * 2] = {
#include "qr_output_U_wgn_18.txt"
};

int outputEvd_ref_loc_18[324 * 2] = {
#include "qr_output_U_loc_18.txt"
};

int outputTc_ref_wgn_18[18] = {
#include "qr_output_T_wgn_18.txt"
};

int outputTc_ref_loc_18[18] = {
#include "qr_output_T_loc_18.txt"
};

// EVD SIZE 20 files
int inputEvd_comp_wgn_20[210 * 2] = {
#include "evd_input_comp_wgn_20.txt"
};

int inputEvd_full_wgn_20[400 * 2] = {
#include "evd_input_rm_wgn_20.txt"
};

int outputEvd_ref_wgn_20[400 * 2] = {
#include "qr_output_U_wgn_20.txt"
};

int outputTc_ref_wgn_20[20] = {
#include "qr_output_T_wgn_20.txt"
};

ce_cmdbuffer_t ce_cmd_buffer;
int ce_evd_cycle_counts[12];

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    int status, i, test_counter = 0;
    double copyerr = 0, copyerr2 = 0;
    int interror = 0, passcount = 0, failcount = 0, scratch_corrupt = 0;
    float evd_tol;
    int evd_max_iter;
    int M, Mc;
    float *temp;

    matAout[0] = 1.0;
    matL[0]    = 1.0;
    scratch[0] = 0;

    BOARD_InitHardware();

    CE_CmdInitBuffer(&ce_cmd_buffer, cmd_buffer, status_buffer, kCE_CmdModeOneBlocking);

    status = CE_NullCmd();
    PRINTF("Status=%8X, Reply=%8X\r\n", status, status_buffer[0]);
    ce_evd_cycle_counts[test_counter++] = status_buffer[6];

    evd_tol      = 1.0000e-11;
    evd_max_iter = 100;

    // write random data to scratch and output buffers
    memset(scratch, 0xFF, sizeof(scratch));
    memset(matAout, 0xFF, sizeof(matAout));
    memset(matL, 0xFF, sizeof(matL));

    //////////////// MAT EVD Tests for size 17 ////////////////
    M  = 17;
    Mc = M * (M + 1) / 2;

    // initialize what should be unused portion of scratch
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        scratch[i] = i;
    }

    ///////// WGN Test /////////
    // FULL Input Matrix
    for (i = 0; i < M * M * 2; i++)
    {
        temp    = (float *)&inputEvd_full_wgn_17[i];
        matA[i] = *temp;

        temp      = (float *)&outputEvd_ref_wgn_17[i];
        refOut[i] = *temp;
    }

    for (i = 0; i < M; i++)
    {
        temp       = (float *)&outputTc_ref_wgn_17[i];
        refOutB[i] = *temp;
    }

    CE_MatrixEvdHerm_CF32(matL, matAout, matA, scratch, M, evd_tol, evd_max_iter, 0);
    // ce_evd_cycle_counts[test_counter++] = status_buffer[6];

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

    // check unused scratch is intact (not corrupted)
    interror = 0;
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        interror += (scratch[i] - i) * (scratch[i] - i);
    }

    if (interror > 0)
        scratch_corrupt++;

    if ((copyerr > 1e-7) || (copyerr2 > 1e-7))
    {
        PRINTF("FULL WGN %d CF32 MAT EVD Test Failed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        failcount++;
    }
    else
    {
        PRINTF("FULL WGN %d CF32 MAT EVD Test Passed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        passcount++;
    }

    // COMP Input Matrix
    for (i = 0; i < Mc * 2; i++)
    {
        temp    = (float *)&inputEvd_comp_wgn_17[i];
        matA[i] = *temp;
    }

    CE_MatrixEvdHerm_CF32(matL, matAout, matA, scratch, M, evd_tol, evd_max_iter, 1);
    ce_evd_cycle_counts[test_counter++] = status_buffer[6];

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

    // check unused scratch is intact (not corrupted)
    interror = 0;
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        interror += (scratch[i] - i) * (scratch[i] - i);
    }

    if (interror > 0)
        scratch_corrupt++;

    if ((copyerr > 1e-7) || (copyerr2 > 1e-7))
    {
        PRINTF("COMP WGN %d CF32 MAT EVD Test Failed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        failcount++;
    }
    else
    {
        PRINTF("COMP WGN %d CF32 MAT EVD Test Passed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        passcount++;
    }

    ///////// LOC Test /////////
    // FULL Input Matrix
    for (i = 0; i < M * M * 2; i++)
    {
        temp    = (float *)&inputEvd_full_loc_17[i];
        matA[i] = *temp;

        temp      = (float *)&outputEvd_ref_loc_17[i];
        refOut[i] = *temp;
    }

    for (i = 0; i < M; i++)
    {
        temp       = (float *)&outputTc_ref_loc_17[i];
        refOutB[i] = *temp;
    }

    CE_MatrixEvdHerm_CF32(matL, matAout, matA, scratch, M, evd_tol, evd_max_iter, 0);
    // ce_evd_cycle_counts[test_counter++] = status_buffer[6];

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

    // check unused scratch is intact (not corrupted)
    interror = 0;
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        interror += (scratch[i] - i) * (scratch[i] - i);
    }

    if (interror > 0)
        scratch_corrupt++;

    if ((copyerr > 1e-7) || (copyerr2 > 1e-7))
    {
        PRINTF("FULL LOC %d CF32 MAT EVD Test Failed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        failcount++;
    }
    else
    {
        PRINTF("FULL LOC %d CF32 MAT EVD Test Passed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        passcount++;
    }

    // COMP Input Matrix
    for (i = 0; i < Mc * 2; i++)
    {
        temp    = (float *)&inputEvd_comp_loc_17[i];
        matA[i] = *temp;
    }

    CE_MatrixEvdHerm_CF32(matL, matAout, matA, scratch, M, evd_tol, evd_max_iter, 1);
    ce_evd_cycle_counts[test_counter++] = status_buffer[6];

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

    // check unused scratch is intact (not corrupted)
    interror = 0;
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        interror += (scratch[i] - i) * (scratch[i] - i);
    }

    if (interror > 0)
        scratch_corrupt++;

    if ((copyerr > 1e-7) || (copyerr2 > 1e-7))
    {
        PRINTF("COMP LOC %d CF32 MAT EVD Test Failed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        failcount++;
    }
    else
    {
        PRINTF("COMP LOC %d CF32 MAT EVD Test Passed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        passcount++;
    }

    //////////////// MAT EVD Tests for size 18 ////////////////
    M  = 18;
    Mc = M * (M + 1) / 2;

    // initialize what should be unused portion of scratch
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        scratch[i] = i;
    }

    ///////// WGN Test /////////
    // FULL Input Matrix
    for (i = 0; i < M * M * 2; i++)
    {
        temp    = (float *)&inputEvd_full_wgn_18[i];
        matA[i] = *temp;

        temp      = (float *)&outputEvd_ref_wgn_18[i];
        refOut[i] = *temp;
    }

    for (i = 0; i < M; i++)
    {
        temp       = (float *)&outputTc_ref_wgn_18[i];
        refOutB[i] = *temp;
    }

    CE_MatrixEvdHerm_CF32(matL, matAout, matA, scratch, M, evd_tol, evd_max_iter, 0);
    // ce_evd_cycle_counts[test_counter++] = status_buffer[6];

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

    // check unused scratch is intact (not corrupted)
    interror = 0;
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        interror += (scratch[i] - i) * (scratch[i] - i);
    }

    if (interror > 0)
        scratch_corrupt++;

    if ((copyerr > 1e-7) || (copyerr2 > 1e-7))
    {
        PRINTF("FULL WGN %d CF32 MAT EVD Test Failed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        failcount++;
    }
    else
    {
        PRINTF("FULL WGN %d CF32 MAT EVD Test Passed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        passcount++;
    }

    // COMP Input Matrix
    for (i = 0; i < Mc * 2; i++)
    {
        temp    = (float *)&inputEvd_comp_wgn_18[i];
        matA[i] = *temp;
    }

    CE_MatrixEvdHerm_CF32(matL, matAout, matA, scratch, M, evd_tol, evd_max_iter, 1);
    ce_evd_cycle_counts[test_counter++] = status_buffer[6];

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

    // check unused scratch is intact (not corrupted)
    interror = 0;
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        interror += (scratch[i] - i) * (scratch[i] - i);
    }

    if (interror > 0)
        scratch_corrupt++;

    if ((copyerr > 1e-7) || (copyerr2 > 1e-7))
    {
        PRINTF("COMP WGN %d CF32 MAT EVD Test Failed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        failcount++;
    }
    else
    {
        PRINTF("COMP WGN %d CF32 MAT EVD Test Passed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        passcount++;
    }

    ///////// LOC Test /////////
    // FULL Input Matrix
    for (i = 0; i < M * M * 2; i++)
    {
        temp    = (float *)&inputEvd_full_loc_18[i];
        matA[i] = *temp;

        temp      = (float *)&outputEvd_ref_loc_18[i];
        refOut[i] = *temp;
    }

    for (i = 0; i < M; i++)
    {
        temp       = (float *)&outputTc_ref_loc_18[i];
        refOutB[i] = *temp;
    }

    CE_MatrixEvdHerm_CF32(matL, matAout, matA, scratch, M, evd_tol, evd_max_iter, 0);
    // ce_evd_cycle_counts[test_counter++] = status_buffer[6];

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

    // check unused scratch is intact (not corrupted)
    interror = 0;
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        interror += (scratch[i] - i) * (scratch[i] - i);
    }

    if (interror > 0)
        scratch_corrupt++;

    if ((copyerr > 1e-7) || (copyerr2 > 1e-7))
    {
        PRINTF("FULL LOC %d CF32 MAT EVD Test Failed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        failcount++;
    }
    else
    {
        PRINTF("FULL LOC %d CF32 MAT EVD Test Passed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        passcount++;
    }

    // COMP Input Matrix
    for (i = 0; i < Mc * 2; i++)
    {
        temp    = (float *)&inputEvd_comp_loc_18[i];
        matA[i] = *temp;
    }

    CE_MatrixEvdHerm_CF32(matL, matAout, matA, scratch, M, evd_tol, evd_max_iter, 1);
    ce_evd_cycle_counts[test_counter++] = status_buffer[6];

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

    // check unused scratch is intact (not corrupted)
    interror = 0;
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        interror += (scratch[i] - i) * (scratch[i] - i);
    }

    if (interror > 0)
        scratch_corrupt++;

    if ((copyerr > 1e-7) || (copyerr2 > 1e-7))
    {
        PRINTF("COMP LOC %d CF32 MAT EVD Test Failed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        failcount++;
    }
    else
    {
        PRINTF("COMP LOC %d CF32 MAT EVD Test Passed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        passcount++;
    }

    //////////////// MAT EVD Tests for size 20 ////////////////
    M  = 20;
    Mc = M * (M + 1) / 2;

    // initialize what should be unused portion of scratch
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        scratch[i] = i;
    }

    ///////// WGN Test /////////
    // FULL Input Matrix
    for (i = 0; i < M * M * 2; i++)
    {
        temp    = (float *)&inputEvd_full_wgn_20[i];
        matA[i] = *temp;

        temp      = (float *)&outputEvd_ref_wgn_20[i];
        refOut[i] = *temp;
    }

    for (i = 0; i < M; i++)
    {
        temp       = (float *)&outputTc_ref_wgn_20[i];
        refOutB[i] = *temp;
    }

    CE_MatrixEvdHerm_CF32(matL, matAout, matA, scratch, M, evd_tol, evd_max_iter, 0);
    // ce_evd_cycle_counts[test_counter++] = status_buffer[6];

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

    // check unused scratch is intact (not corrupted)
    interror = 0;
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        interror += (scratch[i] - i) * (scratch[i] - i);
    }

    if (interror > 0)
        scratch_corrupt++;

    if ((copyerr > 1e-7) || (copyerr2 > 1e-7))
    {
        PRINTF("FULL WGN %d CF32 MAT EVD Test Failed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        failcount++;
    }
    else
    {
        PRINTF("FULL WGN %d CF32 MAT EVD Test Passed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        passcount++;
    }

    // COMP Input Matrix
    for (i = 0; i < Mc * 2; i++)
    {
        temp    = (float *)&inputEvd_comp_wgn_20[i];
        matA[i] = *temp;
    }

    CE_MatrixEvdHerm_CF32(matL, matAout, matA, scratch, M, evd_tol, evd_max_iter, 1);
    ce_evd_cycle_counts[test_counter++] = status_buffer[6];

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

    // check unused scratch is intact (not corrupted)
    interror = 0;
    for (i = M * M * 4 + 360; i < 20 * 20 * 4 + 360; i++)
    {
        interror += (scratch[i] - i) * (scratch[i] - i);
    }

    if (interror > 0)
        scratch_corrupt++;

    if ((copyerr > 1e-7) || (copyerr2 > 1e-7))
    {
        PRINTF("COMP WGN %d CF32 MAT EVD Test Failed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        failcount++;
    }
    else
    {
        PRINTF("COMP WGN %d CF32 MAT EVD Test Passed: Status=%8X, Reply=%8X\n", M, status, status_buffer[0]);
        passcount++;
    }

    while (1)
    {
        // just hang out
    }
}
