/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "fsl_mau.h"
#include <math.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief PI used for angle conversions. */
#define MAU_FLOAT_PI (3.14159265358979323846f)

/*! @brief Floating-point comparison tolerance. */
#define MAU_FLOAT_TOLERANCE (1e-5f)

/*! @brief Frac32 comparison tolerance (LSBs). */
#define MAU_FRAC32_TOLERANCE (1 << 8)

/*! @brief Frac16 comparison tolerance (LSBs). */
#define MAU_FRAC16_TOLERANCE (4)

/*! @brief Absolute value for float. */
#define MAU_FABS(x) (((x) < 0.0f) ? -(x) : (x))

/*! @brief Absolute value for int32_t. */
#define MAU_ABS32(x) (((x) < 0) ? -(x) : (x))

/*! @brief Absolute value for int16_t. */
#define MAU_ABS16(x) (((x) < 0) ? -(x) : (x))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void MAU_DemoSinFlt32(void);
static void MAU_DemoCosFlt32(void);
static void MAU_DemoAtanFlt32(void);
static void MAU_DemoSqrtFlt32(void);
static void MAU_DemoInvSqrtFlt32(void);
static void MAU_DemoRecipFlt32(void);
static void MAU_DemoSinFrac32(void);
static void MAU_DemoCosFrac32(void);
static void MAU_DemoAtanFrac32(void);
static void MAU_DemoSqrtFrac32(void);
static void MAU_DemoSinFrac16(void);
static void MAU_DemoCosFrac16(void);
static void MAU_DemoAtanFrac16(void);
static void MAU_DemoSqrtFrac16(void);
static void MAU_DemoSqrtInt32(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t g_totalTests  = 0U;
static uint32_t g_passedTests = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/

static inline int32_t FtoQ31(float fltVal)
{
    if (fltVal >= 1.0f)
    {
        return (int32_t)0x7FFFFFFF;
    }
    if (fltVal <= -1.0f)
    {
        return (int32_t)0x80000000;
    }
    return (int32_t)(fltVal * 2147483648.0f);
}

static inline float Q31toF(int32_t q31Val)
{
    return (float)q31Val / 2147483648.0f;
}

static inline int16_t FtoQ15(float fltVal)
{
    if (fltVal >= 1.0f)
    {
        return (int16_t)0x7FFF;
    }
    if (fltVal <= -1.0f)
    {
        return (int16_t)0x8000;
    }
    return (int16_t)(fltVal * 32768.0f);
}

static inline float Q15toF(int16_t q15Val)
{
    return (float)q15Val / 32768.0f;
}

/*!
 * @brief Check float result against reference value.
 */
static void CheckFlt32(const char *label, float result, float ref)
{
    float err = MAU_FABS(result - ref);
    bool pass = (err < MAU_FLOAT_TOLERANCE);

    g_totalTests++;
    if (pass)
    {
        g_passedTests++;
    }
    PRINTF("  %-20s MAU=%f  ref=%f  err=%f  [%s]\r\n",
           label, result, ref, err, pass ? "PASS" : "FAIL");
}

/*!
 * @brief Check Frac32 result against reference value.
 */
static void CheckFrac32(const char *label, int32_t result, int32_t ref)
{
    int32_t err = MAU_ABS32(result - ref);
    bool pass   = (err <= MAU_FRAC32_TOLERANCE);

    g_totalTests++;
    if (pass)
    {
        g_passedTests++;
    }
    PRINTF("  %-20s MAU=0x%08lX(%f)  ref=0x%08lX(%f)  [%s]\r\n",
           label,
           result, Q31toF(result),
           ref, Q31toF(ref),
           pass ? "PASS" : "FAIL");
}

/*!
 * @brief Check Frac16 result against reference value.
 */
static void CheckFrac16(const char *label, int16_t result, int16_t ref)
{
    int16_t err = MAU_ABS16((int16_t)(result - ref));
    bool pass   = (err <= MAU_FRAC16_TOLERANCE);

    g_totalTests++;
    if (pass)
    {
        g_passedTests++;
    }
    PRINTF("  %-20s MAU=0x%04X(%f)  ref=0x%04X(%f)  [%s]\r\n",
           label,
           result, Q15toF(result),
           ref, Q15toF(ref),
           pass ? "PASS" : "FAIL");
}

/*!
 * @brief Check Int32 result against reference value (exact).
 */
static void CheckInt32(const char *label, int32_t result, int32_t ref)
{
    bool pass = (result == ref);

    g_totalTests++;
    if (pass)
    {
        g_passedTests++;
    }
    PRINTF("  %-20s MAU=%d  ref=%d  [%s]\r\n",
           label, (int)result, (int)ref, pass ? "PASS" : "FAIL");
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\nMAU Driver Example\r\n");

    MAU_Init();

    /* Float32 demonstrations. */
    MAU_DemoSinFlt32();
    MAU_DemoCosFlt32();
    MAU_DemoAtanFlt32();
    MAU_DemoSqrtFlt32();
    MAU_DemoInvSqrtFlt32();
    MAU_DemoRecipFlt32();

    /* Frac32 (Q1.31) demonstrations. */
    MAU_DemoSinFrac32();
    MAU_DemoCosFrac32();
    MAU_DemoAtanFrac32();
    MAU_DemoSqrtFrac32();

    /* Frac16 (Q1.15) demonstrations. */
    MAU_DemoSinFrac16();
    MAU_DemoCosFrac16();
    MAU_DemoAtanFrac16();
    MAU_DemoSqrtFrac16();

    /* Int32 demonstration. */
    MAU_DemoSqrtInt32();

    MAU_Deinit();

    PRINTF("\r\n====================\r\n");
    PRINTF("Results: %d / %d passed\r\n", (int)g_passedTests, (int)g_totalTests);
    if (g_passedTests == g_totalTests)
    {
        PRINTF("MAU Driver Example Completed Successfully.\r\n");
    }
    else
    {
        PRINTF("MAU Driver Example FAILED (%d errors).\r\n", (int)(g_totalTests - g_passedTests));
    }

    while (1)
    {
    }
}

/* ---------------------------------------------------------------------------
 * Float32 sine — MAU_SinFlt32Reg0..Reg3
 * Reference: sinf(x * PI)
 * ---------------------------------------------------------------------------*/
static void MAU_DemoSinFlt32(void)
{
    PRINTF("--- SinFlt32 ---\r\n");
    CheckFlt32("sin(0)",     MAU_SinFlt32Reg0(0.0f),  sinf(0.0f * MAU_FLOAT_PI));
    CheckFlt32("sin(PI/2)",  MAU_SinFlt32Reg1(0.5f),  sinf(0.5f * MAU_FLOAT_PI));
    CheckFlt32("sin(PI/4)",  MAU_SinFlt32Reg2(0.25f), sinf(0.25f * MAU_FLOAT_PI));
    CheckFlt32("sin(-PI/2)", MAU_SinFlt32Reg3(-0.5f), sinf(-0.5f * MAU_FLOAT_PI));
    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Float32 cosine — MAU_CosFlt32Reg0..Reg3
 * Reference: cosf(x * PI)
 * ---------------------------------------------------------------------------*/
static void MAU_DemoCosFlt32(void)
{
    PRINTF("--- CosFlt32 ---\r\n");
    CheckFlt32("cos(0)",    MAU_CosFlt32Reg0(0.0f), cosf(0.0f * MAU_FLOAT_PI));
    CheckFlt32("cos(PI/2)", MAU_CosFlt32Reg1(0.5f), cosf(0.5f * MAU_FLOAT_PI));
    CheckFlt32("cos(PI/4)", MAU_CosFlt32Reg2(0.25f), cosf(0.25f * MAU_FLOAT_PI));
    CheckFlt32("cos(PI)",   MAU_CosFlt32Reg3(1.0f), cosf(1.0f * MAU_FLOAT_PI));
    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Float32 arctangent — MAU_AtanFlt32Reg0..Reg3
 * Reference: atanf(x) / PI
 * ---------------------------------------------------------------------------*/
static void MAU_DemoAtanFlt32(void)
{
    PRINTF("--- AtanFlt32 ---\r\n");
    CheckFlt32("atan(0.0)/PI",  MAU_AtanFlt32Reg0(0.0f),  atanf(0.0f) / MAU_FLOAT_PI);
    CheckFlt32("atan(1.0)/PI",  MAU_AtanFlt32Reg1(1.0f),  atanf(1.0f) / MAU_FLOAT_PI);
    CheckFlt32("atan(-1.0)/PI", MAU_AtanFlt32Reg2(-1.0f), atanf(-1.0f) / MAU_FLOAT_PI);
    CheckFlt32("atan(0.5)/PI",  MAU_AtanFlt32Reg3(0.5f),  atanf(0.5f) / MAU_FLOAT_PI);
    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Float32 square root — MAU_SqrtFlt32Reg0..Reg3
 * Reference: sqrtf(x)
 * ---------------------------------------------------------------------------*/
static void MAU_DemoSqrtFlt32(void)
{
    PRINTF("--- SqrtFlt32 ---\r\n");
    CheckFlt32("sqrt(1.0)",  MAU_SqrtFlt32Reg0(1.0f),  sqrtf(1.0f));
    CheckFlt32("sqrt(4.0)",  MAU_SqrtFlt32Reg1(4.0f),  sqrtf(4.0f));
    CheckFlt32("sqrt(9.0)",  MAU_SqrtFlt32Reg2(9.0f),  sqrtf(9.0f));
    CheckFlt32("sqrt(0.25)", MAU_SqrtFlt32Reg3(0.25f), sqrtf(0.25f));
    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Float32 inverse square root — MAU_InvSqrtFlt32Reg0..Reg3
 * Reference: 1.0f / sqrtf(x)
 * ---------------------------------------------------------------------------*/
static void MAU_DemoInvSqrtFlt32(void)
{
    PRINTF("--- InvSqrtFlt32 ---\r\n");
    CheckFlt32("1/sqrt(1.0)",  MAU_InvSqrtFlt32Reg0(1.0f),  1.0f / sqrtf(1.0f));
    CheckFlt32("1/sqrt(4.0)",  MAU_InvSqrtFlt32Reg1(4.0f),  1.0f / sqrtf(4.0f));
    CheckFlt32("1/sqrt(0.25)", MAU_InvSqrtFlt32Reg2(0.25f), 1.0f / sqrtf(0.25f));
    CheckFlt32("1/sqrt(9.0)",  MAU_InvSqrtFlt32Reg3(9.0f),  1.0f / sqrtf(9.0f));
    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Float32 reciprocal — MAU_RecipFlt32Reg0..Reg3
 * Reference: 1.0f / x
 * ---------------------------------------------------------------------------*/
static void MAU_DemoRecipFlt32(void)
{
    PRINTF("--- RecipFlt32 ---\r\n");
    CheckFlt32("1/1.0", MAU_RecipFlt32Reg0(1.0f), 1.0f / 1.0f);
    CheckFlt32("1/2.0", MAU_RecipFlt32Reg1(2.0f), 1.0f / 2.0f);
    CheckFlt32("1/4.0", MAU_RecipFlt32Reg2(4.0f), 1.0f / 4.0f);
    CheckFlt32("1/0.5", MAU_RecipFlt32Reg3(0.5f), 1.0f / 0.5f);
    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Frac32 (Q1.31) sine — MAU_SinFrac32Reg0..Reg3
 * Reference: FtoQ31(sinf(flt_in * PI))
 * ---------------------------------------------------------------------------*/
static void MAU_DemoSinFrac32(void)
{
    float flt_in;
    int32_t q31In;

    PRINTF("--- SinFrac32 (Q1.31) ---\r\n");

    flt_in = 0.0f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("sin(0)",     MAU_SinFrac32Reg0(q31In), FtoQ31(sinf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.5f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("sin(PI/2)",  MAU_SinFrac32Reg1(q31In), FtoQ31(sinf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.25f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("sin(PI/4)",  MAU_SinFrac32Reg2(q31In), FtoQ31(sinf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.75f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("sin(3PI/4)", MAU_SinFrac32Reg3(q31In), FtoQ31(sinf(flt_in * MAU_FLOAT_PI)));

    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Frac32 (Q1.31) cosine — MAU_CosFrac32Reg0..Reg3
 * Reference: FtoQ31(cosf(flt_in * PI))
 * ---------------------------------------------------------------------------*/
static void MAU_DemoCosFrac32(void)
{
    float flt_in;
    int32_t q31In;

    PRINTF("--- CosFrac32 (Q1.31) ---\r\n");

    flt_in = 0.0f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("cos(0)",     MAU_CosFrac32Reg0(q31In), FtoQ31(cosf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.5f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("cos(PI/2)",  MAU_CosFrac32Reg1(q31In), FtoQ31(cosf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.25f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("cos(PI/4)",  MAU_CosFrac32Reg2(q31In), FtoQ31(cosf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.75f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("cos(3PI/4)", MAU_CosFrac32Reg3(q31In), FtoQ31(cosf(flt_in * MAU_FLOAT_PI)));

    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Frac32 (Q1.31) arctangent — MAU_AtanFrac32Reg0..Reg3
 * Reference: FtoQ31(atanf(flt_in) / PI)
 * ---------------------------------------------------------------------------*/
static void MAU_DemoAtanFrac32(void)
{
    float flt_in;
    int32_t q31In;

    PRINTF("--- AtanFrac32 (Q1.31) ---\r\n");

    flt_in = 0.0f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("atan(0.0)/PI", MAU_AtanFrac32Reg0(q31In), FtoQ31(atanf(flt_in) / MAU_FLOAT_PI));

    flt_in = 1.0f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("atan(1.0)/PI", MAU_AtanFrac32Reg1(q31In), FtoQ31(atanf(flt_in) / MAU_FLOAT_PI));

    flt_in = 0.0f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("atan(0.0)/PI", MAU_AtanFrac32Reg2(q31In), FtoQ31(atanf(flt_in) / MAU_FLOAT_PI));

    flt_in = 1.0f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("atan(1.0)/PI", MAU_AtanFrac32Reg3(q31In), FtoQ31(atanf(flt_in) / MAU_FLOAT_PI));

    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Frac32 (Q1.31) square root — MAU_SqrtFrac32Reg0..Reg3
 * Reference: FtoQ31(sqrtf(flt_in))
 * ---------------------------------------------------------------------------*/
static void MAU_DemoSqrtFrac32(void)
{
    float flt_in;
    int32_t q31In;

    PRINTF("--- SqrtFrac32 (Q1.31) ---\r\n");

    flt_in = 0.0f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("sqrt(0.0)",  MAU_SqrtFrac32Reg0(q31In), FtoQ31(sqrtf(flt_in)));

    flt_in = 0.25f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("sqrt(0.25)", MAU_SqrtFrac32Reg1(q31In), FtoQ31(sqrtf(flt_in)));

    flt_in = 0.5f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("sqrt(0.5)",  MAU_SqrtFrac32Reg2(q31In), FtoQ31(sqrtf(flt_in)));

    flt_in = 1.0f;
    q31In  = FtoQ31(flt_in);
    CheckFrac32("sqrt(1.0)",  MAU_SqrtFrac32Reg3(q31In), FtoQ31(sqrtf(flt_in)));

    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Frac16 (Q1.15) sine — MAU_SinFrac16Reg0..Reg3
 * Reference: FtoQ15(sinf(flt_in * PI))
 * ---------------------------------------------------------------------------*/
static void MAU_DemoSinFrac16(void)
{
    float flt_in;
    int16_t q15In;

    PRINTF("--- SinFrac16 (Q1.15) ---\r\n");

    flt_in = 0.0f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("sin(0)",     MAU_SinFrac16Reg0(q15In), FtoQ15(sinf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.5f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("sin(PI/2)",  MAU_SinFrac16Reg1(q15In), FtoQ15(sinf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.25f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("sin(PI/4)",  MAU_SinFrac16Reg2(q15In), FtoQ15(sinf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.75f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("sin(3PI/4)", MAU_SinFrac16Reg3(q15In), FtoQ15(sinf(flt_in * MAU_FLOAT_PI)));

    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Frac16 (Q1.15) cosine — MAU_CosFrac16Reg0..Reg3
 * Reference: FtoQ15(cosf(flt_in * PI))
 * ---------------------------------------------------------------------------*/
static void MAU_DemoCosFrac16(void)
{
    float flt_in;
    int16_t q15In;

    PRINTF("--- CosFrac16 (Q1.15) ---\r\n");

    flt_in = 0.0f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("cos(0)",     MAU_CosFrac16Reg0(q15In), FtoQ15(cosf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.5f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("cos(PI/2)",  MAU_CosFrac16Reg1(q15In), FtoQ15(cosf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.25f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("cos(PI/4)",  MAU_CosFrac16Reg2(q15In), FtoQ15(cosf(flt_in * MAU_FLOAT_PI)));

    flt_in = 0.75f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("cos(3PI/4)", MAU_CosFrac16Reg3(q15In), FtoQ15(cosf(flt_in * MAU_FLOAT_PI)));

    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Frac16 (Q1.15) arctangent — MAU_AtanFrac16Reg0..Reg3
 * Reference: FtoQ15(atanf(flt_in) / PI)
 * ---------------------------------------------------------------------------*/
static void MAU_DemoAtanFrac16(void)
{
    float flt_in;
    int16_t q15In;

    PRINTF("--- AtanFrac16 (Q1.15) ---\r\n");

    flt_in = 0.0f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("atan(0.0)/PI", MAU_AtanFrac16Reg0(q15In), FtoQ15(atanf(flt_in) / MAU_FLOAT_PI));

    flt_in = 1.0f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("atan(1.0)/PI", MAU_AtanFrac16Reg1(q15In), FtoQ15(atanf(flt_in) / MAU_FLOAT_PI));

    flt_in = 0.0f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("atan(0.0)/PI", MAU_AtanFrac16Reg2(q15In), FtoQ15(atanf(flt_in) / MAU_FLOAT_PI));

    flt_in = 1.0f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("atan(1.0)/PI", MAU_AtanFrac16Reg3(q15In), FtoQ15(atanf(flt_in) / MAU_FLOAT_PI));

    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Frac16 (Q1.15) square root — MAU_SqrtFrac16Reg0..Reg3
 * Reference: FtoQ15(sqrtf(flt_in))
 * ---------------------------------------------------------------------------*/
static void MAU_DemoSqrtFrac16(void)
{
    float flt_in;
    int16_t q15In;

    PRINTF("--- SqrtFrac16 (Q1.15) ---\r\n");

    flt_in = 0.0f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("sqrt(0.0)",  MAU_SqrtFrac16Reg0(q15In), FtoQ15(sqrtf(flt_in)));

    flt_in = 0.25f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("sqrt(0.25)", MAU_SqrtFrac16Reg1(q15In), FtoQ15(sqrtf(flt_in)));

    flt_in = 0.5f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("sqrt(0.5)",  MAU_SqrtFrac16Reg2(q15In), FtoQ15(sqrtf(flt_in)));

    flt_in = 1.0f;
    q15In  = FtoQ15(flt_in);
    CheckFrac16("sqrt(1.0)",  MAU_SqrtFrac16Reg3(q15In), FtoQ15(sqrtf(flt_in)));

    PRINTF("\r\n");
}

/* ---------------------------------------------------------------------------
 * Int32 square root — MAU_SqrtInt32Reg0..Reg3
 * Reference: (int32_t)sqrtf((float)n)   (perfect-square inputs, exact match)
 * ---------------------------------------------------------------------------*/
static void MAU_DemoSqrtInt32(void)
{
    PRINTF("--- SqrtInt32 ---\r\n");
    CheckInt32("sqrt(0)",   MAU_SqrtInt32Reg0(0),   (int32_t)sqrtf(0.0f));
    CheckInt32("sqrt(4)",   MAU_SqrtInt32Reg1(4),   (int32_t)sqrtf(4.0f));
    CheckInt32("sqrt(9)",   MAU_SqrtInt32Reg2(9),   (int32_t)sqrtf(9.0f));
    CheckInt32("sqrt(100)", MAU_SqrtInt32Reg3(100), (int32_t)sqrtf(100.0f));
    PRINTF("\r\n");
}
