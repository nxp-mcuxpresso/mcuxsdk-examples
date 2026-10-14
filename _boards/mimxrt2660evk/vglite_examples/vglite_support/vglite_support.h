/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _VGLITE_SUPPORT_H_
#define _VGLITE_SUPPORT_H_

#include "fsl_common.h"
#include "vg_lite_ext.h"

#define MAX_CONTIGUOUS_SIZE 0x10000

#if defined(__cplusplus)
extern "C" {
#endif

#define VG_LITE_COMMAND_BUFFER_SIZE (16 << 10)
#define DEFAULT_VG_LITE_TW_WIDTH  64
#define DEFAULT_VG_LITE_TW_HEIGHT 64

status_t BOARD_PrepareVGLiteController(void);

#if defined(__cplusplus)
}
#endif

#endif /* _VGLITE_SUPPORT_H_ */
