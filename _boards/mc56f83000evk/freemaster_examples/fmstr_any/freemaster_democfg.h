/*
 * Copyright 2020-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Example Application - Demo code configuration file
 */

#ifndef __FREEMASTER_DEMO_H
#define __FREEMASTER_DEMO_H

/* These options are used in freemaster_example.c to configure features of FreeMASTER example application */

/* make sure the cpu.h is included and DATA_IN_XFLASH section is defined */
#include "fsl_common.h"

/* We want to use as much of the TSA "Active Content" as possible in the demo application. 
 * The features introduced in MCUXpresso SDK 2.10 and also a new __far compiler directive support 
 * in SDM enables us to put most of the data into X:flash and save RAM. The following condition 
 * is here to maintain compatibility with older versions of the SDK and CodeWarrior tools.
 */
#ifdef DATA_IN_XFLASH  /* The SDK does support the X:flash section */

/* Put the TSA tables and all Active Content into X:flash */
#ifdef FMSTR_TSA_CDECL
/* If this is defined, it means this freemaster_democfg.h was included too late and cannot
 * re-define the TSA declarations. Please set the FreeMASTER configuration in the ConfigTools 
 * ti include the freemaster_democfg.h in the generated freemaster_cfg.h file 
 * (use "custom includes" section) */
#warning FMSTR_TSA_CDECL is already defined. Please make sure the freemaster_democfg.h is included inside the freemaster_cfg.h. 
#else
#define FMSTR_TSA_CDECL const __declspec(DATA_IN_XFLASH) __attribute__((far))
#endif
#define FMSTR_DEMO_CONTENT const __declspec(DATA_IN_XFLASH) __attribute__((far))
#define FMSTR_DEMO_ENOUGH_ROM  1    /* Basic Active Content like hyperlinks. */
#define FMSTR_DEMO_LARGE_ROM   1    /* Extended Active Content like embedded files, project file, etc. */

#else
/* Without the X:flash support, the TSA tables and Active Content goes to RAM - we have enough on this platform. */
#define FMSTR_DEMO_ENOUGH_ROM  1
#define FMSTR_DEMO_LARGE_ROM   1
#endif

#define FMSTR_DEMO_SUPPORT_I64 0 /* Support for long long type */
#define FMSTR_DEMO_SUPPORT_FLT 0 /* Support for float type */
#define FMSTR_DEMO_SUPPORT_DBL 0 /* Support for double type */

#endif /* __FREEMASTER_DEMO_H */
