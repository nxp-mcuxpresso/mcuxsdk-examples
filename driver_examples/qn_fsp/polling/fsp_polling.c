/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include <math.h>
#include <stdlib.h>
#include "arm_math.h"
#include "arm_const_structs.h"

#include "fsl_debug_console.h"
#include "fsl_power.h"
#include "fsl_fsp.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define __AHB_CLK        CLOCK_GetFreq(kCLOCK_BusClk)
#define START_COUNTING() SysTick_Config(__AHB_CLK)
#define GET_CYCLES()     (__AHB_CLK - SysTick->VAL)
#define STOP_COUNTING()  (SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk))
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/* 256 points fft */

float32_t fft_fsp_input[512];
float32_t fft_mcu_input[512];
float32_t fft_fsp_output[512];
float32_t fft_mcu_output[512];

void fft_example(void)
{
    uint32_t fsp_cycles, mcu_cycles;
    float32_t max_value     = 0;
    float32_t min_value     = 0;
    const float32_t data_sf = 10.0;
    uint32_t npts           = 256;
    int32_t tmp_max, tmp_exp, te_scale;
    uint32_t *tmp_addr = 0;

    // prepare input data
    for (uint32_t i = 0; i < npts; i++)
    {
        fft_fsp_input[i] = data_sf * i / npts;
        fft_mcu_input[i] = data_sf * i / npts;
    }

    // Find the absolute maximum of the array and use this to determine te_scale
    FSP_MaxMinF32(DEMO_FSP_BASE, fft_fsp_input, npts, &max_value, &min_value);

    if (-min_value > max_value)
        max_value = -min_value;

    tmp_addr = (uint32_t *)(&max_value);
    tmp_max  = *(tmp_addr);
    tmp_exp  = ((tmp_max >> 23) & (0x000000FF));
    tmp_exp  = tmp_exp - 127;
    te_scale = -(tmp_exp + 1);

    // FSP
    fsp_te_instance_t fsp_rfft_cfg;
    fsp_rfft_cfg.te_point = kFSP_TePts256Points;
    fsp_rfft_cfg.te_scale = te_scale;

    START_COUNTING();
    FSP_RfftF32(DEMO_FSP_BASE, &fsp_rfft_cfg, (float32_t *)fft_fsp_input, fft_fsp_output);
    fsp_cycles = GET_CYCLES();
    FSP_TePostProcess(DEMO_FSP_BASE, &fsp_rfft_cfg, kFSP_TeIoModeRealInputComplexOutput, kFSP_TeModeFft,
                      fft_fsp_output);

    // MCU
    arm_rfft_fast_instance_f32 arm_rfft_cfg;
    arm_rfft_fast_init_f32(&arm_rfft_cfg, npts);

    START_COUNTING();
    arm_rfft_fast_f32(&arm_rfft_cfg, fft_mcu_input, fft_mcu_output, 0);
    mcu_cycles = GET_CYCLES();

    PRINTF("-- 256 pionts real floating FFT\r\n");
    PRINTF("FSP %d cycles, MCU %d cycles\r\n", fsp_cycles, mcu_cycles);
}

/* Matrix Example */

float32_t A_f32[16] = {
    /* Const,   numTaps,   blockSize,   numTaps*blockSize */
    1.0, 32.0, 4.0, 128.0, 1.0, 32.0, 64.0, 2048.0, 1.0, 16.0, 4.0, 64.0, 1.0, 16.0, 64.0, 1024.0,
};

/* ----------------------------------------------------------------------
 * Temporary buffers  for storing intermediate values
 * ------------------------------------------------------------------- */
/* Transpose of A Buffer */
float32_t AT_f32[16];
/* (Transpose of A * A) Buffer */
float32_t ATMA_f32[16];

void matrix_example(void)
{
    uint32_t fsp_cycles, mcu_cycles;

    fsp_matrix_instance_t FSP_A;    /* Matrix A Instance */
    fsp_matrix_instance_t FSP_AT;   /* Matrix AT(A transpose) instance */
    fsp_matrix_instance_t FSP_ATMA; /* Matrix ATMA( AT multiply with A) instance */

    arm_matrix_instance_f32 A;    /* Matrix A Instance */
    arm_matrix_instance_f32 AT;   /* Matrix AT(A transpose) instance */
    arm_matrix_instance_f32 ATMA; /* Matrix ATMA( AT multiply with A) instance */

    uint32_t srcRows, srcColumns; /* Temporary variables */

    START_COUNTING();

    /* Initialise A Matrix Instance with numRows, numCols and data array(A_f32) */
    srcRows    = 4;
    srcColumns = 4;
    FSP_MatInit(&FSP_A, srcRows, srcColumns, (float32_t *)A_f32);

    /* Initialise Matrix Instance AT with numRows, numCols and data array(AT_f32) */
    srcRows    = 4;
    srcColumns = 4;
    FSP_MatInit(&FSP_AT, srcRows, srcColumns, AT_f32);

    /* calculation of A transpose */
    FSP_MatTransF32(DEMO_FSP_BASE, &FSP_A, &FSP_AT);

    /* Initialise ATMA Matrix Instance with numRows, numCols and data array(ATMA_f32) */
    srcRows    = 4;
    srcColumns = 4;
    FSP_MatInit(&FSP_ATMA, srcRows, srcColumns, ATMA_f32);

    /* calculation of AT Multiply with A */
    FSP_MatMultF32(DEMO_FSP_BASE, &FSP_AT, &FSP_A, &FSP_ATMA);

    fsp_cycles = GET_CYCLES();

    START_COUNTING();

    /* Initialise A Matrix Instance with numRows, numCols and data array(A_f32) */
    srcRows    = 4;
    srcColumns = 4;
    arm_mat_init_f32(&A, srcRows, srcColumns, (float32_t *)A_f32);

    /* Initialise Matrix Instance AT with numRows, numCols and data array(AT_f32) */
    srcRows    = 4;
    srcColumns = 4;
    arm_mat_init_f32(&AT, srcRows, srcColumns, AT_f32);

    /* calculation of A transpose */
    arm_mat_trans_f32(&A, &AT);

    /* Initialise ATMA Matrix Instance with numRows, numCols and data array(ATMA_f32) */
    srcRows    = 4;
    srcColumns = 4;
    arm_mat_init_f32(&ATMA, srcRows, srcColumns, ATMA_f32);

    /* calculation of AT Multiply with A */
    arm_mat_mult_f32(&AT, &A, &ATMA);

    mcu_cycles = GET_CYCLES();

    PRINTF("-- matrix example\r\n");
    PRINTF("FSP %d cycles, MCU %d cycles\r\n", fsp_cycles, mcu_cycles);
}

/* Sum of the squares of the elements of a floating-point vector */

float32_t power_acc_input[1024];
float32_t power_acc_fsp_output;
float32_t power_acc_mcu_output;

void power_example(void)
{
    uint32_t fsp_cycles, mcu_cycles;

    for (uint32_t i = 0; i < 256; i++)
        power_acc_input[i] = 0.1 * i;

    // FSP
    START_COUNTING();
    FSP_PowerF32(DEMO_FSP_BASE, power_acc_input, 1024, &power_acc_fsp_output);
    fsp_cycles = GET_CYCLES();

    // MCU
    START_COUNTING();
    arm_power_f32(power_acc_input, 1024, &power_acc_mcu_output);
    mcu_cycles = GET_CYCLES();

    PRINTF("-- Sum of the squares of the 1024 elements\r\n");
    PRINTF("FSP %d cycles, MCU %d cycles\r\n", fsp_cycles, mcu_cycles);
}

/* Sin */
void sin_example(void)
{
    uint32_t fsp_cycles, mcu_cycles;
    float32_t sin_input = 3.14159f / 6;

    // FSP
    START_COUNTING();
    FSP_SinF32(DEMO_FSP_BASE, sin_input / 3.14159f);
    fsp_cycles = GET_CYCLES();

    // MCU
    START_COUNTING();
    arm_sin_f32(sin_input);
    mcu_cycles = GET_CYCLES();

    PRINTF("-- sin(3.14159 / 6)\r\n");
    PRINTF("FSP %d cycles, MCU %d cycles\r\n", fsp_cycles, mcu_cycles);
}
/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    POWER_DisablePD(kPDRUNCFG_PD_FSP);
    POWER_DisablePD(kPDRUNCFG_PD_FIR);

    /*Fsp module init*/
    FSP_Init(DEMO_FSP_BASE);

    fft_example();

    matrix_example();

    power_example();

    sin_example();

    STOP_COUNTING();

    while (1)
    {
    }
}
