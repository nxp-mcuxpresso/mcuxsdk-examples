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
#include "math.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAU_EXAMPLE_PI             (3.1415926535898)
#define MAU_EXAMPLE_MATH_LEN       (32)
#define MAU_EXAMPLE_FLOAT_2_Q31(x) ((int32_t)((x) * 2147483648.0f))
#define MAU_EXAMPLE_FLOAT_2_Q15(x) (int16_t) __SSAT(((int32_t)((x) * 32768.0f)), 16)
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
static void MAU_Uint32Example(void);
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

    PRINTF("MAU calculation example\r\n");

    MAU_GetDefaultConfig(&config);
    MAU_Init(MAU0, &config);

    MAU_FloatExample();
    MAU_Q31Example();
    MAU_Q15Example();
    MAU_Uint32Example();

    PRINTF("MAU calculation example successed\r\n");

    while (1)
    {
    }
}

static void MAU_FloatExample(void)
{
    float input  = 0;
    float result = 0, result2 = 0;
    float ref = 0;
    int32_t i;

    for (i = 1; i <= MAU_EXAMPLE_MATH_LEN; i++)
    {
        input = i;

        ref    = 1 / input;
        result = MAU_RecipFloat(MAU0, input, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result)) <= 0.000001);

        ref    = sqrt((double)input);
        result = MAU_SqrtFloat(MAU0, input, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result)) <= 0.000001);

        ref    = 1 / sqrt((double)input);
        result = MAU_SqrtRecipFloat(MAU0, input, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result)) <= 0.000001);

        /* The input value is in radians.*/
        input = (double)((((float)(i * 2) / (float)MAU_EXAMPLE_MATH_LEN) - 1.0f));

        ref    = sin((double)input * MAU_EXAMPLE_PI);
        result = MAU_SinPIXFloat(MAU0, input, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result)) <= 0.000001);

        ref    = cos((double)input * MAU_EXAMPLE_PI);
        result = MAU_CosPIXFloat(MAU0, input, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result)) <= 0.000001);

        MAU_SinCosPIXFloat(MAU0, input, &result, &result2, kMAU_RES0, kMAU_RES1);
        ref = sin((double)input * MAU_EXAMPLE_PI);
        MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result)) <= 0.000001);
        ref = cos((double)input * MAU_EXAMPLE_PI);
        MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result2)) <= 0.000001);

        ref    = atan((double)input) / MAU_EXAMPLE_PI;
        result = MAU_AtanXDivPIFloat(MAU0, input, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(fabs((double)(ref - result)) <= 0.000001);
    }
}

static void MAU_Q31Example(void)
{
    float input       = 0;
    int32_t input_q1x = 0;
    int32_t result = 0, result2 = 0;
    int32_t ref = 0;
    int32_t i;

    for (i = 1; i <= MAU_EXAMPLE_MATH_LEN; i++)
    {
        input     = (float)i / MAU_EXAMPLE_MATH_LEN;
        input_q1x = MAU_EXAMPLE_FLOAT_2_Q31(input);

        ref    = MAU_EXAMPLE_FLOAT_2_Q31((float)i / input);
        result = MAU_RecipQ31(MAU0, input_q1x, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

        ref    = MAU_EXAMPLE_FLOAT_2_Q31(sqrt((double)input));
        result = MAU_SqrtQ31(MAU0, input_q1x, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

        /* The input value is in radians.*/
        input     = (double)((((float)(i * 2) / (float)MAU_EXAMPLE_MATH_LEN) - 1.0f));
        input_q1x = MAU_EXAMPLE_FLOAT_2_Q31(input);

        ref    = MAU_EXAMPLE_FLOAT_2_Q31(sin((double)input * MAU_EXAMPLE_PI));
        result = MAU_SinPIXQ31(MAU0, input_q1x, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

        ref    = MAU_EXAMPLE_FLOAT_2_Q31(cos((double)input * MAU_EXAMPLE_PI));
        result = MAU_CosPIXQ31(MAU0, input_q1x, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

        MAU_SinCosPIXQ31(MAU0, input_q1x, &result, &result2, kMAU_RES0, kMAU_RES1);
        ref = MAU_EXAMPLE_FLOAT_2_Q31(sin((double)input * MAU_EXAMPLE_PI));
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);
        ref = MAU_EXAMPLE_FLOAT_2_Q31(cos((double)input * MAU_EXAMPLE_PI));
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result2) <= 400);

        ref    = MAU_EXAMPLE_FLOAT_2_Q31(atan((double)input) / MAU_EXAMPLE_PI);
        result = MAU_AtanXDivPIQ31(MAU0, input_q1x, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);
    }
}
static void MAU_Q15Example(void)
{
    float input       = 0;
    int16_t input_q1x = 0;
    int16_t result = 0, result2 = 0;
    int16_t ref = 0;
    int16_t i;

    for (i = 1; i <= MAU_EXAMPLE_MATH_LEN; i++)
    {
        input     = (float)i / MAU_EXAMPLE_MATH_LEN;
        input_q1x = MAU_EXAMPLE_FLOAT_2_Q15(input);

        ref    = MAU_EXAMPLE_FLOAT_2_Q15((float)i / input);
        result = MAU_RecipQ15(MAU0, input_q1x, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

        ref    = MAU_EXAMPLE_FLOAT_2_Q15(sqrt((double)input));
        result = MAU_SqrtQ15(MAU0, input_q1x, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

        /* The input value is in radians.*/
        input     = (double)((((float)(i * 2) / (float)MAU_EXAMPLE_MATH_LEN) - 1.0f));
        input_q1x = MAU_EXAMPLE_FLOAT_2_Q15(input);

        ref    = MAU_EXAMPLE_FLOAT_2_Q15(sin((double)input * MAU_EXAMPLE_PI));
        result = MAU_SinPIXQ15(MAU0, input_q1x, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

        ref    = MAU_EXAMPLE_FLOAT_2_Q15(cos((double)input * MAU_EXAMPLE_PI));
        result = MAU_CosPIXQ15(MAU0, input_q1x, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);

        MAU_SinCosPIXQ15(MAU0, input_q1x, &result, &result2, kMAU_RES0, kMAU_RES1);
        ref = MAU_EXAMPLE_FLOAT_2_Q15(sin((double)input * MAU_EXAMPLE_PI));
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);
        ref = MAU_EXAMPLE_FLOAT_2_Q15(cos((double)input * MAU_EXAMPLE_PI));
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result2) <= 400);

        ref    = MAU_EXAMPLE_FLOAT_2_Q15(atan((double)input) / MAU_EXAMPLE_PI);
        result = MAU_AtanXDivPIQ15(MAU0, input_q1x, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 400);
    }
}

static void MAU_Uint32Example(void)
{
    uint32_t input  = 0;
    uint32_t result = 0;
    uint32_t ref    = 0;
    uint32_t i;

    for (i = 1; i <= MAU_EXAMPLE_MATH_LEN; i++)
    {
        input = i * i;

        ref    = i;
        result = MAU_SqrtUint32(MAU0, input, kMAU_RES0);
        MAU_EXAMPLE_ASSERT_TRUE(abs(ref - result) <= 1);
    }
}
