/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GG_UTILS_H
#define GG_UTILS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

void setup_ui(gg_ui_t * ui);

void gg_load_screen_animation(gg_ui_t * ui, const gg_screen_load_cfg_t * cfg);

#ifdef __cplusplus
}
#endif
#endif
