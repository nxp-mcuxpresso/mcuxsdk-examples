/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gui_guider.h"
#include "gg_utils.h"
#include "gg_event.h"

void setup_screen(gg_ui_t * ui)
{
    // Create screen: screen
    ui->screen.screen = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(ui->screen.screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_user_data(ui->screen.screen, &ui->screen.screen);

    // Create rect: ui->screen.rect
    ui->screen.rect = lv_obj_create(ui->screen.screen);
    lv_obj_align(ui->screen.rect, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(ui->screen.rect, LV_PCT(40), LV_PCT(40));
    lv_obj_set_style_bg_color(ui->screen.rect, lv_color_make(0, 0, 255), LV_PART_MAIN);

    // Create label: ui->screen.label
    ui->screen.label = lv_label_create(ui->screen.rect);
    lv_label_set_text(ui->screen.label, "Drag Me");
    lv_obj_center(ui->screen.label);

    // Update screen layout.
    lv_obj_update_layout(ui->screen.screen);

    // Init events of screen.
    gg_event_init_screen(ui);
}
