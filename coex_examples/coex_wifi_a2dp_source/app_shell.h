/*
 * Copyright 2020 - 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_SHELL_H__
#define __APP_SHELL_H__

#include "fsl_shell.h"

/*******************************************************************************
 * API
 ******************************************************************************/

/* Creates the single app-owned fsl_shell and registers the "bt" and "wifi"
 * commands. Called from the edgefast_open a2dp_source task via bt_ready(). */
void app_shell_init(void);

#endif /* __APP_SHELL_H__ */
