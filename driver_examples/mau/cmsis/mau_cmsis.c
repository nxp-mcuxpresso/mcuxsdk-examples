/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_mau.h"
#include "arm_math.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MATH_PI        3.1415926535898
#define FLOAT_2_Q31(x) ((int32_t)((x) * 2147483648.0f))
#define FLOAT_2_Q15(x) (int16_t) __SSAT(((int32_t)((x) * 32768.0f)), 16)

#define EXAMPLE_ASSERT_TRUE(x)            \
    if (!(x))                             \
    {                                     \
        PRINTF("%s error\r\n", __func__); \
        while (1)                         \
        {                                 \
        }                                 \
    }

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void arm_sqrt_q15Example(void);
static void arm_sqrt_q31Example(void);
static void arm_sin_q15Example(void);
static void arm_sin_q31Example(void);
static void arm_sin_f32Example(void);
static void arm_cos_q15Example(void);
static void arm_cos_q31Example(void);
static void arm_cos_f32Example(void);
static void arm_sin_cos_q15Example(void);
static void arm_sin_cos_q31Example(void);
static void arm_sin_cos_f32Example(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    mau_config_t config;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("MAU CMSIS DSP example started\r\n");

    MAU_GetDefaultConfig(&config);
    MAU_Init(MAU0, &config);
    arm_sqrt_q15Example();
    arm_sqrt_q31Example();
    arm_sin_q15Example();
    arm_sin_q31Example();
    arm_sin_f32Example();
    arm_cos_q15Example();
    arm_cos_q31Example();
    arm_cos_f32Example();
    arm_sin_cos_q15Example();
    arm_sin_cos_q31Example();
    arm_sin_cos_f32Example();

    PRINTF("MAU CMSIS DSP successed\r\n");

    while (1)
    {
    }
}

/* Q15 sqrt */
static void arm_sqrt_q15Example(void)
{
    q15_t input      = FLOAT_2_Q15(0.25f);
    q15_t sqrtResult = 0;
    q15_t sqrtRef    = FLOAT_2_Q15(0.5f);

    arm_sqrt_q15(input, &sqrtResult);

    EXAMPLE_ASSERT_TRUE(abs(sqrtRef - sqrtResult) <= 2);
}

/* Q31 sqrt */
static void arm_sqrt_q31Example(void)
{
    q31_t input      = FLOAT_2_Q31(0.25f);
    q31_t sqrtResult = 0;
    q31_t sqrtRef    = FLOAT_2_Q31(0.5f);

    arm_sqrt_q31(input, &sqrtResult);

    EXAMPLE_ASSERT_TRUE(abs(sqrtRef - sqrtResult) <= 2);
}

/* Q15 sin */
static void arm_sin_q15Example(void)
{
    /* sin(pi/6) = 1/2. */
    q15_t input     = FLOAT_2_Q15(0.5f / 6.0f);
    q15_t sinResult = 0;
    q15_t sinRef    = FLOAT_2_Q15(0.5f);

    /* The Q15 input value is in the range [0 +0.9999] and is mapped to a radian
     * value in the range [0 2*pi) */
    sinResult = arm_sin_q15(input);

    EXAMPLE_ASSERT_TRUE(abs(sinRef - sinResult) < 10);
}

/* Q31 sin */
static void arm_sin_q31Example(void)
{
    /* sin(pi/6) = 1/2. */
    q31_t inputValue = FLOAT_2_Q31(0.5f / 6.0f);
    q31_t sinResult  = 0;
    q31_t sinRef     = FLOAT_2_Q31(0.5f);

    sinResult = arm_sin_q31(inputValue);

    EXAMPLE_ASSERT_TRUE(abs(sinRef - sinResult) < 20000);
}

/* Float sin */
static void arm_sin_f32Example(void)
{
    float input;
    float Result;
    float sinRef;

    input  = 3.0;
    sinRef = 0.141120;

    Result = arm_sin_f32(input);

    EXAMPLE_ASSERT_TRUE(fabs((double)(sinRef - Result)) < 0.00001);
}

/* Q15 cos */
static void arm_cos_q15Example(void)
{
    /* cos(pi/3) = 0.5. */
    q15_t inputValue = FLOAT_2_Q15(0.5f / 3.0f);
    q15_t cosResult  = 0;
    q15_t cosRef     = FLOAT_2_Q15(0.5f);

    cosResult = arm_cos_q15(inputValue);

    EXAMPLE_ASSERT_TRUE(abs(cosRef - cosResult) < 10);
}

/* Q31 cos */
static void arm_cos_q31Example(void)
{
    /* cos(pi/3) = 0.5. */
    q31_t inputValue = FLOAT_2_Q31(0.5f / 3.0f);
    q31_t cosResult  = 0;
    q31_t cosRef     = FLOAT_2_Q31(0.5f);

    cosResult = arm_cos_q31(inputValue);

    EXAMPLE_ASSERT_TRUE(abs(cosRef - cosResult) < 20000);
}

/* Float cos */
static void arm_cos_f32Example(void)
{
    float input;
    float Result;
    float cosRef;

    input  = 1.0;
    cosRef = 0.540302;

    Result = arm_cos_f32(input);

    EXAMPLE_ASSERT_TRUE(fabs((double)(cosRef - Result)) < 0.00001);
}

/* Q15 sin cos */
static void arm_sin_cos_q15Example(void)
{
    /* Calculate sin(pi/6) and cos(pi/6) */
    q15_t inputValue = FLOAT_2_Q15(0.5f / 6.0f);
    q15_t sinResult  = 0;
    q15_t cosResult  = 0;
    q15_t sinRef     = FLOAT_2_Q15(0.5f);       /* sin(pi/6) = 0.5 */
    q15_t cosRef     = FLOAT_2_Q15(0.8660254f); /* cos(pi/6) = 0.8660254 */

    arm_sin_cos_q15(inputValue, &sinResult, &cosResult);

    EXAMPLE_ASSERT_TRUE(abs(sinRef - sinResult) < 10);
    EXAMPLE_ASSERT_TRUE(abs(cosRef - cosResult) < 10);
}

/* Q31 sin cos */
static void arm_sin_cos_q31Example(void)
{
    /* Calculate sin(pi/6) and cos(pi/6) */
    q31_t inputValue = FLOAT_2_Q31(0.5f / 6.0f);
    q31_t sinResult  = 0;
    q31_t cosResult  = 0;
    q31_t sinRef     = FLOAT_2_Q31(0.5f);       /* sin(pi/6) = 0.5 */
    q31_t cosRef     = FLOAT_2_Q31(0.8660254f); /* cos(pi/6) = 0.8660254 */

    arm_sin_cos_q31(inputValue, &sinResult, &cosResult);

    EXAMPLE_ASSERT_TRUE(abs(sinRef - sinResult) < 20000);
    EXAMPLE_ASSERT_TRUE(abs(cosRef - cosResult) < 20000);
}
/* Float sin cos */
static void arm_sin_cos_f32Example(void)
{
    float input;
    float sinResult;
    float cosResult;
    float sinRef;
    float cosRef;

    input  = 1.0;
    sinRef = 0.841471;
    cosRef = 0.540302;

    arm_sin_cos_f32(input, &sinResult, &cosResult);

    EXAMPLE_ASSERT_TRUE(fabs((double)(sinRef - sinResult)) < 0.00001);
    EXAMPLE_ASSERT_TRUE(fabs((double)(cosRef - cosResult)) < 0.00001);
}