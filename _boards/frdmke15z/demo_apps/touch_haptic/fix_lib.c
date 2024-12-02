/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************/
/*                                                                      */
/*  Library with simple fixed-point calculation macros.                 */
/*                                                                      */
/************************************************************************/


/************************************************************************/
/*                                                                      */
/*  Includes                                                            */
/*                                                                      */
/************************************************************************/
#include "fix_lib.h"

/************************************************************************/
/*                                                                      */
/*  Definitions                                                         */
/*                                                                      */
/************************************************************************/
/*  Sine wave definitions.                                              */
/* 4th order Taylor coefficients: */
/* x[k] = (pi)^k / factorial(k) * 2^(32-1-3)  */
#define FXL_SIN_TAYLOR_EXP (3)
fix_t sinTaylor[] = { 843314857, 1387197337, 684554447, 160863847 };

/* Nested Chebyshev polynomials * (2^31-2) */
fix_t sinCheby[] = { -1686629708, 1087764605, -281339767, 40365855, -3683035, 207199 };
#define FXL_SIN_CHEBY_EXP (2)

/* Low pass filter exponent */
#define FXL_SIN_LFP_EXP (0)

/************************************************************************/
/*                                                                      */
/*  Types                                                               */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/*  Functions                                                           */
/*                                                                      */
/************************************************************************/
/* Initialize first order low-pass filter with:
 * alpha = exp( - F / FS ) and y[k-1] = initValue
 * y[k] = u[k]*(1-alpha) + y[k-1]*alpha
 */
void fxl_lpf_init( fxl_lpf_t * pS, fix_t alpha, fix_t initVal )
{
    pS->alpha0 = FIXT_MAX - alpha; /* u[k] */
    pS->alpha1 = alpha;            /* u[k-1] */
    pS->y1 = initVal;              /* y[k-1] */
}

/* First order low-pass filter process */
FXL_FORCE_INLINE fix_t fxl_lpf_process( fxl_lpf_t* pS, fix_t u0 )
{
    fix_t y0 = 0;
    /* u[k] * (1 - alpha) */
    y0 += MUL_FIX_EXP(u0, 0, pS->alpha0, FXL_LPF_ALPHA_EXP, 0);
    /* y[k-1]*alpha */
    y0 += MUL_FIX_EXP(pS->y1, 0, pS->alpha1, FXL_LPF_ALPHA_EXP, 0);
    /* y[k-1] = y[k] */
    pS->y1 = y0;
    return(y0);
}

/* Initialize first order asymmetric smoothing filter with:
 * alphaUp = exp( - F / FS ), alphaDown = exp( - F / FS ), and y[k-1] = initValue
 * y[k] = u[k]*(1-alpha) + y[k-1]*alpha
 */
void fxl_asym_smooth_init( fxl_asym_smooth_t * pS, fix_t alphaUp , fix_t alphaDown, fix_t initVal )
{
    pS->alphaUp0   = FIXT_MAX - alphaUp;
    pS->alphaUp1   = alphaUp;
    pS->alphaDown0 = FIXT_MAX - alphaDown;
    pS->alphaDown1 = alphaDown;
    pS->y1 = initVal;
}

/* First order asymmetric smoothing filter process */
FXL_FORCE_INLINE fix_t fxl_asym_smooth_process( fxl_asym_smooth_t* pS, fix_t u0 )
{
    fix_t y0 = 0;
    if( u0 > pS->y1 )
    {
        /* Envelope up */
        /* u[k] * (1 - alpha) */
        y0 += MUL_FIX_EXP(u0, 0, pS->alphaUp0, FXL_LPF_ALPHA_EXP, 0);
        /* y[k-1]*alpha */
        y0 += MUL_FIX_EXP(pS->y1, 0, pS->alphaUp1, FXL_LPF_ALPHA_EXP, 0);
    } else
    {
        /* Envelope down */
        /* u[k] * (1 - alpha) */
        y0 += MUL_FIX_EXP(u0, 0, pS->alphaDown0, FXL_LPF_ALPHA_EXP, 0);
        /* y[k-1]*alpha */
        y0 += MUL_FIX_EXP(pS->y1, 0, pS->alphaDown1, FXL_LPF_ALPHA_EXP, 0);

    }
    /* y[k-1] = y[k] */
    pS->y1 = y0;
    return(y0);
}

/* Biquad filter init
 *   y(k) = b0 * x(k) + b1 * x(k-1) + b2 * x(k-2) - a1 * y(k-1) - a2 * y(k-2)
 */
void fxl_biquad_init(fxl_biquad_t* pS, fix_t b0, fix_t b1, fix_t b2, fix_t a1, fix_t a2)
{
    pS->b0 = b0; pS->b1 = b1; pS->b2 = b2;
    pS->a1 = a1; pS->a2 = a2;
    pS->u1 =  0; pS->u2 =  0;
    pS->y1 =  0; pS->y2 =  0;
}

/* Biquad filter process */
FXL_FORCE_INLINE fix_t fxl_biquad_process(fxl_biquad_t* pS, fix_t u0)
{
    dfix_t acc = 0;
    fix_t y0 = 0;
    /* Biquad : IN_EXP + FXL_BIQUAD_EXP */
    acc += MUL_FIX(    u0, pS->b0);
    acc += MUL_FIX(pS->u1, pS->b1);
    acc += MUL_FIX(pS->u2, pS->b2);
    acc -= MUL_FIX(pS->y1, pS->a1);
    acc -= MUL_FIX(pS->y2, pS->a2);
    /* Final value in original IN_EXP */
    y0 = DF2F_EXP(acc, 0 + FXL_BIQUAD_EXP, 0);
    /* Update memory */
    pS->u2 = pS->u1;
    pS->u1 = u0;
    pS->y2 = pS->y1;
    pS->y1 = y0;
    return (y0);
}

/* Initialize sine wave generator with given fixed-point phase increment. */
void fxl_sin_init(fxl_sin_t* pS, fix_t phaseInc, fix_t amp, fix_t phaseInit )
{
    pS->phaseInc = phaseInc;
    pS->phase = phaseInit;
    pS->phaseInit = phaseInit;
    pS->amp = amp;
}
/* Update sine wave frequency based on phase increment */
void fxl_sin_update_freq(fxl_sin_t* pS, fix_t phaseInc)
{
    /* Update phase increment, keep current up/down count state */
    pS->phaseInc = ( pS->phaseInc > 0 ? phaseInc : -phaseInc );
}

/* Reset sine wave generator to initially configured state. */
void fxl_sin_reset(fxl_sin_t* pS)
{
    /* Reset up/down increment */
    pS->phaseInc = FIX_ABS(pS->phaseInc);
    /* Reset to starting phase */
    pS->phase = pS->phaseInit;
}

/* Sine-wave generator process:
 * Sine based on 4th order Taylor approximation [-pi/2 pi/2]
 * Matlab code:
 * function y = sinT(x)
 *  f3 = 1/factorial(3); f5 = 1/factorial(5); f7 = 1/factorial(7);
 *  y = x - f3 .* x.^3 + f5 .* x .^5 - f7 .* x .^7;
 *  end
*/
fix_t fxl_sin_process_taylor(fxl_sin_t* pS)
{
    dfix_t acc = 0;
    fix_t sinOut = 0;

    /* Increment phase */
    pS->phase += pS->phaseInc;
    /* Wrap phase [-0.5 0.5] - > pi included in the Taylor table */
    /* Phase: count up -> count down -> count up ... */
    if (pS->phase > FXL_SIN_PHASE_PI_2)
    {
        pS->phase = FXL_SIN_PHASE_PI - pS->phase;
        pS->phaseInc = -pS->phaseInc;
    }
    else if(pS->phase < -FXL_SIN_PHASE_PI_2)
    {
        pS->phase = -FXL_SIN_PHASE_PI - pS->phase;
        pS->phaseInc = -pS->phaseInc;
    }
    /* Calculate phase^N */
    fix_t phase1 = pS->phase;
    fix_t phase2 = MUL_FIX_EXP(phase1, 0, phase1, 0, 0);
    fix_t phase3 = MUL_FIX_EXP(phase2, 0, phase1, 0, 0);
    fix_t phase5 = MUL_FIX_EXP(phase3, 0, phase2, 0, 0);
    fix_t phase7 = MUL_FIX_EXP(phase5, 0, phase2, 0, 0);

    /* Calculate Taylor series */
    acc += MUL_FIX(phase1, sinTaylor[0]);
    acc -= MUL_FIX(phase3, sinTaylor[1]);
    acc += MUL_FIX(phase5, sinTaylor[2]);
    acc -= MUL_FIX(phase7, sinTaylor[3]);

    /* dfix_t to fix_t */
    sinOut = (DF2F_EXP(acc, (0 + FXL_SIN_TAYLOR_EXP), 0));
    /* Scale to specified amplitude */
    sinOut = MUL_FIX_EXP(sinOut, 0, pS->amp, 0, 0);
    return(sinOut);
}

FXL_FORCE_INLINE fix_t fxl_sin_process(fxl_sin_t* pS)
{
    fix_t acc = 0;
    fix_t phase2 = 0;

    /* Increment and wrap phase [-1 1] */
    pS->phase += pS->phaseInc;

    /* Calculate phase^2 */
    phase2 = MUL_FIX_EXP(pS->phase, 0, pS->phase, 0, 0);

    /* Calculate nested chebyshev polynomials */
    /* Even number guarantees sin( phase ) < "1" */
#if(0)
    acc = sinCheby[5];
    acc = MUL_FIX_EXP(acc, FXL_SIN_CHEBY_EXP, phase2, 0, FXL_SIN_CHEBY_EXP) + sinCheby[4];
    acc = MUL_FIX_EXP(acc, FXL_SIN_CHEBY_EXP, phase2, 0, FXL_SIN_CHEBY_EXP) + sinCheby[3];
#else
    acc = sinCheby[3];
#endif
    acc = MUL_FIX_EXP(acc, FXL_SIN_CHEBY_EXP, phase2, 0, FXL_SIN_CHEBY_EXP) + sinCheby[2];
    acc = MUL_FIX_EXP(acc, FXL_SIN_CHEBY_EXP, phase2, 0, FXL_SIN_CHEBY_EXP) + sinCheby[1];
    acc = MUL_FIX_EXP(acc, FXL_SIN_CHEBY_EXP, phase2, 0, FXL_SIN_CHEBY_EXP) + sinCheby[0];
    /* acc = acc * phase */
    acc = MUL_FIX_EXP(acc, FXL_SIN_CHEBY_EXP, pS->phase, 0, FXL_SIN_CHEBY_EXP);
    /* acc = (x^2 - 1) * acc */
    acc = MUL_FIX_EXP( (phase2 - FXL_SIN_PHASE_PI), 0, acc, FXL_SIN_CHEBY_EXP, 0);
    /* Scale to specified amplitude */
    acc = MUL_FIX_EXP(acc, 0, pS->amp, 0, 0);
    return(acc);
}


/* Inverse fix_t number and clip inversion to 2^invExp*/
FXL_FORCE_INLINE fix_t inv_fix(fix_t aVal, int aExp, int invExp)
{
    dfix_t one = DFIXT_MAX >> (1 + invExp + aExp);
    /* Maximum ranges for division with ceil rounding */
    fix_t invMin = (FIXT_MIN >> (invExp + aExp)) + ( (1 << (aExp)) - 1 );
    fix_t invMax = (FIXT_MAX >> (invExp + aExp)) + ( (1 << (aExp)) - 1 );
    /* Clip number not to exceed 2^invExp */
    aVal = (aVal > 0) ? FIX_MAX(invMax, aVal) : FIX_MIN(invMin, aVal);
    fix_t invVal = (fix_t)( one / aVal );
    /*fix_t invVal = FIX_CLIP( one / aVal, FIXT_MIN, FIXT_MAX ); */
    return (invVal);
}

#if (defined(FXL_EXAMPLES))
/* Example function usage. */
#include <stdio.h>
#define MY_EXP     (2)
#define MY_EXP_INV (2)
#define MY_OUT_EXP (MY_EXP+MY_EXP_INV)

void fix_math_example(void)
{
    /* Example multiplication */
    fix_t x1 = DOUBLE_TO_FIX_EXP(1.50, MY_EXP);
    fix_t x2 = DOUBLE_TO_FIX_EXP(0.50, MY_EXP);
    fix_t r0 = DOUBLE_TO_FIX_EXP(0.75, MY_EXP);
    /* fix_t * fix_t to fix_t */
    fix_t r1 = MUL_FIX_EXP(x1, MY_EXP, x2, MY_EXP, MY_EXP);

    /* Accumulator: fix_t * fix_t to d_fix */
    dfix_t a1 = MUL_FIX(x1, x2); /* MY_EXP+MY_EXP */
    /* dfix_t to fix_t */
    fix_t r2 = DF2F_EXP(a1, MY_EXP+MY_EXP, MY_EXP); /* MY_EXP */

    /* Show results */
    printf("x1 * x2 = % .6f \n",  FIX_TO_DOUBLE(r0, MY_EXP));
    printf("x1 * x2 = % .6f \n",  FIX_TO_DOUBLE(r1, MY_EXP));
    printf("x1 * x2 = % .6f \n", DFIX_TO_DOUBLE(a1, MY_EXP + MY_EXP));
    printf("x1 * x2 = % .6f \n",  FIX_TO_DOUBLE(r2, MY_EXP));


    /* Example division */
    fix_t y0  = DOUBLE_TO_FIX_EXP(1.5 , MY_EXP);
    fix_t num = DOUBLE_TO_FIX_EXP(0.75, MY_EXP);
    fix_t den = DOUBLE_TO_FIX_EXP(0.5 , MY_EXP);
    fix_t denInv = inv_fix(den, MY_EXP, MY_EXP_INV);
    fix_t y1 = MUL_FIX_EXP(num, MY_EXP, denInv, MY_EXP_INV, MY_OUT_EXP);

    printf("num / den = % .6f \n"    , FIX_TO_DOUBLE(y0, MY_EXP));
    printf("1 / den = % .6f \n"      , FIX_TO_DOUBLE(denInv, MY_EXP_INV));
    printf("num * 1 / den = % .6f \n", FIX_TO_DOUBLE(y1, MY_OUT_EXP));
}

#define SIN_N_SAMPLES (160)
void fix_math_example_sin(void)
{
    /* Sine params */
    double sinF = 100; /* [Hz] */
    double fs = 8000;  /* [Hz] */
    fix_t sinVec[SIN_N_SAMPLES] = { 0 };
    /* Sine wave process state */
    fxl_sin_t sinState = { 0 };
    /* Calculate phase step */
    fix_t phaseInc = FXL_SIN_PHASE_INC(sinF, fs);
    fix_t amp  = 32767;
    fix_t phaseInit = 0;
    /* Init phase to cos */
    /*phaseInit = FXL_SIN_PHASE_PI_2; */
    fxl_sin_init(&sinState, phaseInc, amp, phaseInit);
    /* Generate sine wave */
    for (int c1 = 0; c1 < SIN_N_SAMPLES; ++c1)
    {
        sinVec[c1] = fxl_sin_process(&sinState);
    }
}

#define FILTER_N_SAMPLES (500)
void fix_math_example_lpf(void)
{
    /* low-pass filter params */
    double fLp = 100; /* [Hz] */
    double fs = 8000; /* [Hz] */
    fxl_lpf_t filtState = { 0 };
    /* alpha = exp( -fLp/fs ) */
    fix_t alpha = DOUBLE_TO_FIX_EXP(0.987577800493881, 0);
    fix_t initOut = FIXT_MAX;
    /* Filter init */
    fxl_lpf_init(&filtState, alpha, initOut);
    /* Filter signal */
    fix_t filtVec[FILTER_N_SAMPLES] = { 0 };
    fix_t uIn = 0;
    for (int c1 = 0; c1 < FILTER_N_SAMPLES; ++c1)
    {
        filtVec[c1] = fxl_lpf_process(&filtState, uIn);
    }
}

void fix_math_example_biquad(void)
{
    /* Matlab:
        fLp = 100; fs = 8000;
        wLp = 2*pi*fLp; qLp = 1;
        [bVec,aVec] = bilinear([wLp^2],[1 wLp/qLp wLp^2],fs);
    */
    fxl_biquad_t biquadState = { 0 };
    fix_t b0 = DOUBLE_TO_FIX_EXP( 0.00148165628135044, FXL_BIQUAD_EXP);
    fix_t b1 = DOUBLE_TO_FIX_EXP( 0.00296331256270133, FXL_BIQUAD_EXP);
    fix_t b2 = DOUBLE_TO_FIX_EXP( 0.00148165628135022, FXL_BIQUAD_EXP);
    fix_t a1 = DOUBLE_TO_FIX_EXP(-1.91861324010977000, FXL_BIQUAD_EXP);
    fix_t a2 = DOUBLE_TO_FIX_EXP( 0.92453986523517200, FXL_BIQUAD_EXP);
    /* Filter init */
    fxl_biquad_init(&biquadState, b0, b1, b2, a1, a2);
    fix_t filtVec[FILTER_N_SAMPLES] = { 0 };
    /* Make headroom for step response overshoot */
    /* uIn = 0.5 */
    fix_t uIn = FIXT_MAX >> 1;
    /* Filter signal */
    for (int c1 = 0; c1 < FILTER_N_SAMPLES; ++c1)
    {
        filtVec[c1] = fxl_biquad_process(&biquadState, uIn);
    }
}
#endif
