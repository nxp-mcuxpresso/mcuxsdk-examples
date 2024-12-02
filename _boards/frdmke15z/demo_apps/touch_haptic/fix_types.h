/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*--------------------------------------- Multiple Inclusion Protection */
#ifndef _FIX_TYPES_
#define _FIX_TYPES_

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

/************************************************************************/
/*                                                                      */
/*  Definitions                                                         */
/*                                                                      */
/************************************************************************/
/* Generic ABS / MIN / MAX / CLIP implementations to be used in macros */
#define FIX_ABS(aVal)      ( ( (aVal) <   0  )   ? ( -(aVal) ) : (aVal) )
#define FIX_MIN(aVal,bVal) ( ( (aVal) < (bVal) ) ?    (aVal)   : (bVal) )
#define FIX_MAX(aVal,bVal) ( ( (aVal) > (bVal) ) ?    (aVal)   : (bVal) )
#define FIX_CLIP(aVal, aMin, aMax)  FIX_MIN( FIX_MAX( (aVal), (aMin) ), (aMax) )

/* Type representation casts */
#define  TO_FIXT(aVal) (  (fix_t)(aVal) )
#define TO_DFIXT(aVal) ( (dfix_t)(aVal) )

#define FIX_W_SZ     32  /* fix_t word size in bits */
typedef int          fix_t;
#define FIXT_MIN   ( (fix_t)((fix_t)1 << (FIX_W_SZ-1)) )
#define FIXT_MAX   ( ~FIXT_MIN )

#define DFIX_W_SZ    64  /* dfix_t word size in bits */
typedef long long    dfix_t;
#define DFIXT_MIN   ( (dfix_t)((dfix_t)1 << (DFIX_W_SZ-1)) )
#define DFIXT_MAX   (~DFIXT_MIN)

#define DF2F_HEADROOM  ( DFIX_W_SZ -  FIX_W_SZ )

/* Integer / Float to fix_t / dfix_t conversions */
#define FIX_ROUNDING(aVal)((aVal >= 0)?(0.5):(-0.49))

/* double to fix_t */
#define  DOUBLE_TO_FIX_EXP(aVal,aExp) (  TO_FIXT((dfix_t)FIX_CLIP(( ( (double)(aVal) * ((dfix_t)1 << ( FIX_W_SZ-1-(aExp))) ) + FIX_ROUNDING(aVal) ),\
                                                                  (                   -((dfix_t)1 << ( FIX_W_SZ-1))      ),\
                                                                  (                    ((dfix_t)1 << ( FIX_W_SZ-1))-1    )\
                                                                 )\
                                               )\
                                       )
/* double to dfix_t */
#define DOUBLE_TO_DFIX_EXP(aVal,aExp) ( TO_DFIXT((dfix_t)FIX_CLIP(( ( (double)(aVal) * ((dfix_t)1 << (DFIX_W_SZ-1-(aExp))) ) + FIX_ROUNDING(aVal) ),\
                                                                  (                   -((dfix_t)1 << (DFIX_W_SZ-1))      ),\
                                                                  (                    ((dfix_t)1 << (DFIX_W_SZ-1))-1    )\
                                                                 )\
                                                )\
                                       )
/* Multiply fix_t and store result in dfix_t */
#define MUL_FIX(a,b)                                 ( ( (dfix_t)( (fix_t)(a) ) * (fix_t)(b) ) << 1 )
/* Multiply fix_t and store result in fix_t with specified exponent */
#define MUL_FIX_EXP(a,aExp,b,bExp,outExp) ( (fix_t)( ( ( (dfix_t)( (fix_t)(a) ) * (fix_t)(b) ) << 1 ) \
                                                     >> (DF2F_HEADROOM - (aExp) - (bExp) + (outExp) ) \
                                                   ) \
                                          )

/* Divide fix_t and store result in dfix_t */

/* Exponent manipulation */
/* dfix_t to fix_t with specified exponent */
#define DF2F_EXP(aVal,aExp,outExp)( (fix_t)( (dfix_t)(aVal) >> (DF2F_HEADROOM - (aExp) + (outExp) ) ) )
/* fix_t to dfix_t with specified exponent */
#define F2DF_EXP(aVal,aExp,outExp)  ( (dfix_t)(aVal) << ( DF2F_HEADROOM + (aExp) - (outExp) ) )

/* integer to fix_t with specified exponent */
#define I2F_EXP(aVal,outExp) ( ( fix_t)(aVal) << (  FIX_W_SZ-1-(outExp)) )
/* integer to dfix_t with specified exponent */
#define I2DF_EXP(aVal,outExp)( (dfix_t)(aVal) << ( DFIX_W_SZ-1-(outExp)) )

/* Convert fix_t a with exponent aExp to double */
#define FIX_TO_DOUBLE(aVal,aExp) ( (double) aVal/( (dfix_t)(1) << ( FIX_W_SZ-1-(aExp) ) ) )
/* Convert dfix_t a with exponent aExp to double */
#define DFIX_TO_DOUBLE(aVal,aExp)( (double) aVal/( (dfix_t)(1) << (DFIX_W_SZ-1-(aExp) ) ) )

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
#endif /* _FIX_TYPES_ */
