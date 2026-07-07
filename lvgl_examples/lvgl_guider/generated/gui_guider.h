/* Copyright 2026 NXP */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct {

} gg_layer_sys_t;

typedef struct {

} gg_layer_top_t;

typedef struct {

} gg_layer_bottom_t;

typedef struct {
    lv_obj_t * screen;
    lv_obj_t * rect;
    lv_obj_t * label;
} gg_screen_t;

typedef struct {
    gg_layer_sys_t layer_sys;
    gg_layer_top_t layer_top;
    gg_layer_bottom_t layer_bottom;
    gg_screen_t screen;
} gg_ui_t;

typedef struct {
    lv_obj_t ** scr;
    void (*setup_fn)(gg_ui_t * ui);
    lv_screen_load_anim_t anim_type;
    uint32_t time;
    uint32_t delay;
    bool auto_del;
} gg_screen_load_cfg_t;

extern gg_ui_t guider_ui;

void setup_layer_sys(gg_ui_t * ui);
void setup_layer_top(gg_ui_t * ui);
void setup_layer_bottom(gg_ui_t * ui);
void setup_screen(gg_ui_t * ui);

#ifdef __cplusplus
}
#endif
#endif
