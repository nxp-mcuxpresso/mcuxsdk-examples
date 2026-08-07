/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * FreeMASTER Communication Driver - User Configuration File
 */

#ifndef __FREEMASTER_CFG_H
#define __FREEMASTER_CFG_H

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define FMSTR_PLATFORM_CORTEX_M 1 /* Cortex-M platform (see freemaster.h for list of all supported platforms) */

#define FMSTR_DEMO_ENOUGH_ROM 1
#define FMSTR_DEMO_LARGE_ROM  1
#define FMSTR_DEMO_SUPPORT_I64 1
#define FMSTR_DEMO_SUPPORT_FLT 1
#define FMSTR_DEMO_SUPPORT_DBL 1

#define FMSTR_DISABLE 0

#define FMSTR_LONG_INTR  0
#define FMSTR_SHORT_INTR 0
#define FMSTR_POLL_DRIVEN 1

#define FMSTR_TRANSPORT FMSTR_SERIAL
#define FMSTR_SERIAL_DRV FMSTR_SERIAL_MCUX_LPUART

#define FMSTR_SERIAL_BASE (HSP__LPUART_1)

#define FMSTR_USE_RECORDER        1
#define FMSTR_REC_BUFF_SIZE       4096
#define FMSTR_USE_SCOPE           1
#define FMSTR_MAX_SCOPE_VARS      8

#define FMSTR_USE_TSA             1
#define FMSTR_USE_TSA_INROM       1
#define FMSTR_USE_TSA_SAFETY      0
#define FMSTR_USE_TSA_DYNAMIC     1

#define FMSTR_USE_PIPES           0
#define FMSTR_USE_APPCMD          1

#define FMSTR_COMM_BUFFER_SIZE    240

#endif /* __FREEMASTER_CFG_H */
