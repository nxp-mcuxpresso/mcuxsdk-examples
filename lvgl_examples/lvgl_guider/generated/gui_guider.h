/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
    lv_obj_t *blueCounter;
    lv_obj_t *blueCounter_cont_1;
    lv_obj_t *blueCounter_plus;
    lv_obj_t *blueCounter_plus_label;
    lv_obj_t *blueCounter_minus;
    lv_obj_t *blueCounter_minus_label;
    lv_obj_t *blueCounter_counter;
    lv_obj_t *blueCounter_logo;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void setup_ui(lv_ui *ui);

extern lv_ui guider_ui;

void setup_scr_screen(lv_ui *ui);
LV_IMAGE_DECLARE(_btn_RGB565A8_65x65);
LV_IMAGE_DECLARE(_NXP_Logo_RGB565A8_60x29);

#ifdef __cplusplus
}
#endif
#endif
