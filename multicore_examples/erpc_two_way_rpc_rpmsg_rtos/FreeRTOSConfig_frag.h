/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FREERTOSCONFIG_FRAG_H_
#define _FREERTOSCONFIG_FRAG_H_

#define configASSERT_BOOL(x) if((x) == 0) {taskDISABLE_INTERRUPTS(); for (;;);}

#endif /*_FREERTOSCONFIG_FRAG_H_*/
