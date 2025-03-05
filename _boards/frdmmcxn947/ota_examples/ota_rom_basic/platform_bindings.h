/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PLATFORM_BINDINGS_H__
#define __PLATFORM_BINDINGS_H__

#include "fsl_common.h"
#include "fsl_component_serial_manager.h"

int xmodem_putc(int);
int xmodem_getc(void);
int xmodem_canread(void);
#define xmodem_canread_retries (SystemCoreClock/16)

#endif /*__PLATFORM_BINDINGS_H__*/
