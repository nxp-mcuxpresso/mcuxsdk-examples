/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*--------------------------------------- Multiple Inclusion Protection */
#ifndef _FIX_LIB_
#define _FIX_LIB_

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
#include "fix_types.h"

/************************************************************************/
/*                                                                      */
/*  Definitions                                                         */
/*                                                                      */
/************************************************************************/
/* Include examples for testing */
#define FXL_FORCE_INLINE __attribute__((always_inline)) inline
/* #define FXL_FORCE_INLINE */

/*#define FXL_EXAMPLES */

/*  First order low-pass filter definitions.*/
#define FXL_LPF_ALPHA_EXP (0)

/*  Biquad filter definitions.*/
#define FXL_BIQUAD_EXP (3)

/* Sine-wave definitions*/
#define FXL_SIN_PHASE_INC(F,FS) DOUBLE_TO_FIX_EXP( ((double)2.0 * (F) ) / (FS), 0)

/* Phase [-1 1], pi included internally */
#define FXL_SIN_PHASE_PI    ( FIXT_MAX )
#define FXL_SIN_PHASE_PI_2  ( FXL_SIN_PHASE_PI >> 1 )

/************************************************************************/
/*                                                                      */
/*  Types                                                               */
/*                                                                      */
/************************************************************************/
/* Sine wave generator state */
typedef struct fxl_sin_t
{
    fix_t phase;
    fix_t phaseInc;
    fix_t phaseInit;
    fix_t amp;
} fxl_sin_t;

/* First order filter */
typedef struct fxl_lpf_t
{
    fix_t alpha0; /*     exp( - f / fs ) */
    fix_t alpha1; /* 1 - exp( - f / fs ) */
    fix_t y1;     /* y[k-1] */
} fxl_lpf_t;

/* First order asymmetric smoothing filter */
typedef struct fxl_asym_smooth_t
{
    fix_t alphaUp0;   /* Envelope up:   u[k]   */
    fix_t alphaUp1;   /* Envelope up:   y[k-1] */
    fix_t alphaDown0; /* Envelope down: u[k]   */
    fix_t alphaDown1; /* Envelope down: y[k-1] */
    fix_t y1;         /* y[k-1] */
} fxl_asym_smooth_t;

/* Second order biquad filter 
 * y[k] = b0 * x[k] + b1 * x[k-1] + b2 * x[k-2] - a1 * y[k-1] - a2 * y[k-2]
 */
typedef struct fxl_biquad_t
{
    fix_t b0, b1, b2 , a1, a2;
    fix_t u1, u2;
    fix_t y1, y2;
} fxl_biquad_t;

/************************************************************************/
/*                                                                      */
/*  Functions                                                           */
/*                                                                      */
/************************************************************************/

/* Initialize first order low-pass filter with:
   alpha = exp( - F / FS )
   y[k-1] = initValue
   y[k] = u[k]*(1-alpha) + y[k-1]*alpha
*/
void fxl_lpf_init(fxl_lpf_t * pS, fix_t alpha, fix_t initVal);

/* First order low-pass filter process */
fix_t fxl_lpf_process(fxl_lpf_t* pS, fix_t uIn);

/* Initialize first order asymmetric smoothing filter with:
 * alphaUp = exp( - F / FS ), alphaDown = exp( - F / FS ), and y[k-1] = initValue
 * if envelope goes up alpha = alphaUp
 * if envelope goes down alpha = alphaDown
 * y[k] = u[k]*(1-alpha) + y[k-1]*alpha
 */
void fxl_asym_smooth_init( fxl_asym_smooth_t * pS, fix_t alphaUp , fix_t alphaDown, fix_t initVal );

/* First order asymmetric smoothing filter process */
fix_t fxl_asym_smooth_process( fxl_asym_smooth_t* pS, fix_t u0 );

/* Biquad filter init
 * y[k] = b0 * x[k] + b1 * x[k-1] + b2 * x[k-2] - a1 * y[k-1] - a2 * y[k-2]
 */
void fxl_biquad_init(fxl_biquad_t* pS, fix_t b0, fix_t b1, fix_t b2, fix_t a1, fix_t a2);

/* Biquad filter process */
fix_t fxl_biquad_process(fxl_biquad_t* pS, fix_t uIn);

/* Initialize sine wave generator with given fixed-point phase increment 
   and initial phase. 
   Enables to calculate phaseInc at compile time. */
void fxl_sin_init(fxl_sin_t* pS, fix_t phaseInc, fix_t amp, fix_t phaseInit );

/* Sine wave generator process: 
   Calculate sine-wave value and increment phase.
   Process should be called at each fs. */
fix_t fxl_sin_process(fxl_sin_t* pS);

/* Reset sine wave generator to initially configured state. */
void fxl_sin_reset(fxl_sin_t* pS);

/* Update sine wave frequency based on phase increment */
void fxl_sin_update_freq(fxl_sin_t* pS, fix_t phaseInc);

/* Inverse fix_t number and clip inversion to 2^invExp */
fix_t inv_fix(fix_t aVal, int aExp, int invExp);

#if (defined(FXL_EXAMPLES))
void fix_math_example(void);
void fix_math_example_sin(void);
void fix_math_example_lpf(void);
void fix_math_example_biquad(void);
#endif

#endif /* _FIX_LIB_ */
