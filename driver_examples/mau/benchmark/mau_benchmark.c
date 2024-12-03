/*
 * Copyright 2023 NXP
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
#define MAU_TEST_LOOP               (100000U)
#define MAU_EXAMPLE_PI              (3.1415926535898)
#define MAU_EXAMPLE_MATH_LEN        (32)
#define MAU_EXAMPLE_Q31_TO_FLOAT(x) (1.0 * (x) / 2147483648.0f)
#define MAU_EXAMPLE_Q29_TO_FLOAT(x) (1.0 * (x) / 536870912.0f)
#define MAU_EXAMPLE_Q15_TO_FLOAT(x) (1.0 * (x) / 32768.0f)
#define MAU_EXAMPLE_Q13_TO_FLOAT(x) (1.0 * (x) / 8192.0f)
#define MAU_EXAMPLE_FLOAT_2_Q31(x)  ((int32_t)((x) * 2147483648.0f))
#define MAU_EXAMPLE_FLOAT_2_Q15(x)  (int16_t) __SSAT(((int32_t)((x) * 32768.0f)), 16)
#define MAU_EXAMPLE_ASSERT_TRUE(x)                     \
    if (!(x))                                          \
    {                                                  \
        PRINTF("%s error:%d\r\n", __func__, __LINE__); \
        while (1)                                      \
        {                                              \
        }                                              \
    }
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void MAU_FloatExample(void);
static void MAU_Q31Example(void);
static void MAU_Q15Example(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint32_t s_timeMs = 0;
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
    BOARD_InitSystick1Ms();

    PRINTF("MAU benchmark example\r\n");

    MAU_GetDefaultConfig(&config);
    MAU_Init(MAU0, &config);

    MAU_FloatExample();
    MAU_Q31Example();
    MAU_Q15Example();

    PRINTF("MAU benchmark example successed\r\n");

    while (1)
    {
    }
}

void SYSTICH_HANDLER(void)
{
    s_timeMs++;
}

static void MAU_FloatExample(void)
{
    float input  = 0.5;
    float result = 0;
    float ref    = 0;
    int32_t i = 0, oldTime = 0;

    /* SQRT */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        arm_sqrt_f32(input, &result);
    }
    PRINTF("Float SQRT(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    arm_sqrt_f32(input, &ref);
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_SqrtFloat(MAU0, input, kMAU_RES0);
    }
    PRINTF("Float SQRT(MAU)\t: %d ms\r\n", s_timeMs - oldTime);
    MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result)) <= 0.000001);

    /* SIN */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = arm_sin_f32(input);
    }
    PRINTF("Float SIN(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    ref     = arm_sin_f32((double)input * MAU_EXAMPLE_PI);
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_SinPIXFloat(MAU0, input, kMAU_RES0);
    }
    PRINTF("Float SIN(MAU)\t: %d ms\r\n", s_timeMs - oldTime);
    MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result)) <= 0.000001);

    /* COS */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = arm_cos_f32(input);
    }
    PRINTF("Float COS(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    ref     = arm_cos_f32((double)input * MAU_EXAMPLE_PI);
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_CosPIXFloat(MAU0, input, kMAU_RES0);
    }
    PRINTF("Float COS(MAU)\t: %d ms\r\n", s_timeMs - oldTime);
    MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result)) <= 0.000001);

    /* ATAN */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        arm_atan2_f32(input, 1, &result);
    }
    PRINTF("Float ATAN(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    arm_atan2_f32(input, 1, &ref);
    ref     = (double)ref / MAU_EXAMPLE_PI;
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_AtanXDivPIFloat(MAU0, input, kMAU_RES0);
    }
    PRINTF("Float ATAN(MAU)\t: %d ms\r\n", s_timeMs - oldTime);
    MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result)) <= 0.000001);
}

static void MAU_Q31Example(void)
{
    float input       = 0.5;
    int32_t input_q1x = 0;
    int32_t ref = 0, result = 0;
    float ref_float = 0, result_float = 0;
    int32_t i = 0, oldTime = 0;

    input_q1x = MAU_EXAMPLE_FLOAT_2_Q31(input);

    /* SQRT */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        arm_sqrt_q31(input_q1x, &result);
    }
    PRINTF("Q31 SQRT(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    ref = result;

    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_SqrtQ31(MAU0, input_q1x, kMAU_RES0);
    }
    PRINTF("Q31 SQRT(MAU)\t: %d ms\r\n", s_timeMs - oldTime);
    MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

    /* SIN */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = arm_sin_q31(input_q1x);
    }
    PRINTF("Q31 SIN(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    /* CMSIS-DSP SIN&COS input = (input*PI) / (2*PI) */
    ref     = arm_sin_q31(MAU_EXAMPLE_FLOAT_2_Q31(input / 2));
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_SinPIXQ31(MAU0, input_q1x, kMAU_RES0);
    }
    PRINTF("Q31 SIN(MAU)\t: %d ms\r\n", s_timeMs - oldTime);
    MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

    /* COS */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = arm_cos_q31(input_q1x);
    }
    PRINTF("Q31 COS(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    /* CMSIS-DSP SIN&COS input = (input*PI) / (2*PI) */
    ref     = arm_cos_q31(MAU_EXAMPLE_FLOAT_2_Q31(input / 2));
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_CosPIXQ31(MAU0, input_q1x, kMAU_RES0);
    }
    PRINTF("Q31 COS(MAU)\t: %d ms\r\n", s_timeMs - oldTime);
    MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

    /* ATAN */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        arm_atan2_q31(input_q1x, MAU_EXAMPLE_FLOAT_2_Q31(0.5), &result);
    }
    PRINTF("Q31 ATAN(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    arm_atan2_q31(MAU_EXAMPLE_FLOAT_2_Q31(input / 2), MAU_EXAMPLE_FLOAT_2_Q31(0.5), &ref);
    /* arm_atan2_q31 result is Q2.29 */
    ref_float = MAU_EXAMPLE_Q29_TO_FLOAT(ref) / MAU_EXAMPLE_PI;

    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_AtanXDivPIQ31(MAU0, input_q1x, kMAU_RES0);
    }
    PRINTF("Q31 ATAN(MAU)\t: %d ms\r\n", s_timeMs - oldTime);

    result_float = MAU_EXAMPLE_Q31_TO_FLOAT(result);
    MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref_float - result_float)) <= 0.000001);
}
static void MAU_Q15Example(void)
{
    float input       = 0.5;
    int16_t input_q1x = 0;
    int16_t ref = 0, result = 0;
    float ref_float = 0, result_float = 0;
    int32_t i = 0, oldTime = 0;

    input_q1x = MAU_EXAMPLE_FLOAT_2_Q15(input);

    /* SQRT */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        arm_sqrt_q15(input_q1x, &result);
    }
    PRINTF("Q15 SQRT(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    ref = result;

    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_SqrtQ15(MAU0, input_q1x, kMAU_RES0);
    }
    PRINTF("Q15 SQRT(MAU)\t: %d ms\r\n", s_timeMs - oldTime);
    MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

    /* SIN */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = arm_sin_q15(input_q1x);
    }
    PRINTF("Q15 SIN(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    /* CMSIS-DSP SIN&COS input = (input*PI) / (2*PI) */
    ref     = arm_sin_q15(MAU_EXAMPLE_FLOAT_2_Q15(input / 2));
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_SinPIXQ15(MAU0, input_q1x, kMAU_RES0);
    }
    PRINTF("Q15 SIN(MAU)\t: %d ms\r\n", s_timeMs - oldTime);
    MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

    /* COS */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = arm_cos_q15(input_q1x);
    }
    PRINTF("Q15 COS(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    /* CMSIS-DSP SIN&COS input = (input*PI) / (2*PI) */
    ref     = arm_cos_q15(MAU_EXAMPLE_FLOAT_2_Q15(input / 2));
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_CosPIXQ15(MAU0, input_q1x, kMAU_RES0);
    }
    PRINTF("Q15 COS(MAU)\t: %d ms\r\n", s_timeMs - oldTime);
    MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

    /* ATAN */
    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        arm_atan2_q15(input_q1x, MAU_EXAMPLE_FLOAT_2_Q15(0.5), &result);
    }
    PRINTF("Q15 ATAN(DSP)\t: %d ms\r\n", s_timeMs - oldTime);

    arm_atan2_q15(MAU_EXAMPLE_FLOAT_2_Q15(input / 2), MAU_EXAMPLE_FLOAT_2_Q15(0.5), &ref);
    /* arm_atan2_q15 result is Q2.13 */
    ref_float = MAU_EXAMPLE_Q13_TO_FLOAT(ref) / MAU_EXAMPLE_PI;

    oldTime = s_timeMs;
    for (i = 0; i < MAU_TEST_LOOP; i++)
    {
        result = MAU_AtanXDivPIQ15(MAU0, input_q1x, kMAU_RES0);
    }
    PRINTF("Q15 ATAN(MAU)\t: %d ms\r\n", s_timeMs - oldTime);

    result_float = MAU_EXAMPLE_Q15_TO_FLOAT(result);
    MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref_float - result_float)) <= 0.0001);
}
