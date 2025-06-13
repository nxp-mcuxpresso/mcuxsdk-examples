/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "widgets_init.h"

extern void setup_scr_screen(lv_ui *ui);

void setup_bottom_layer(void)
{
    lv_theme_apply(lv_layer_bottom());
}

void setup_ui(lv_ui *ui)
{
    setup_bottom_layer();
    setup_scr_screen(ui);
    lv_screen_load(ui->blueCounter);
}
