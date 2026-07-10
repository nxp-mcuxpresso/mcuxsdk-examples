/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gg_utils.h"

void setup_ui(gg_ui_t * ui)
{
    /* Setup layers and default screen */
    setup_layer_sys(ui);
    setup_layer_top(ui);
    setup_layer_bottom(ui);
    setup_screen(ui);

    /* Load active screen */
    lv_screen_load(ui->screen.screen);
}

static void gg_screen_deleted_cb(lv_event_t * e)
{
    lv_obj_t ** scr_ptr = lv_event_get_user_data(e);
    *scr_ptr = NULL;
}

void gg_load_screen_animation(gg_ui_t * ui, const gg_screen_load_cfg_t * cfg)
{
    if(*cfg->scr == NULL) {
        cfg->setup_fn(ui);
        if(*cfg->scr == NULL) {
            return;
        }
    }
    if(cfg->auto_del) {
        lv_obj_t * old_scr = lv_screen_active();
        lv_obj_t ** old_scr_ptr = lv_obj_get_user_data(old_scr);
        if(old_scr_ptr != NULL) {
            lv_obj_add_event_cb(old_scr, gg_screen_deleted_cb, LV_EVENT_DELETE, old_scr_ptr);
        }
    }
    lv_screen_load_anim(*cfg->scr, cfg->anim_type, cfg->time, cfg->delay, cfg->auto_del);
}
