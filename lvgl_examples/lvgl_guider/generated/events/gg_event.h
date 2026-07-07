/* Copyright 2026 NXP */

#ifndef GG_EVENT_H
#define GG_EVENT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"
#include "gg_utils.h"

/* Animation control helper functions */
static inline void gg_pause_anim(lv_obj_t * obj, lv_anim_exec_xcb_t exec_cb)
{
    lv_anim_t * anim = lv_anim_get(obj, exec_cb);
    if(anim != NULL) {
        lv_anim_pause(anim);
    }
}

static inline void gg_resume_anim(lv_obj_t * obj, lv_anim_exec_xcb_t exec_cb)
{
    lv_anim_t * anim = lv_anim_get(obj, exec_cb);
    if(anim != NULL) {
        lv_anim_resume(anim);
    }
}

void gg_event_init_layer_sys(gg_ui_t * ui);
void gg_event_init_layer_top(gg_ui_t * ui);
void gg_event_init_layer_bottom(gg_ui_t * ui);
void gg_event_init_screen(gg_ui_t * ui);

#ifdef __cplusplus
}
#endif
#endif
