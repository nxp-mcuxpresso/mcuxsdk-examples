/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gg_event.h"
#include "custom.h"

/* Handler for dragging the rect widget */
static void rect_event_handler(lv_event_t * e)
{
    lv_point_t point;
    lv_obj_t * obj;
    lv_indev_t * indev;
    int32_t x, y;

    obj = lv_event_get_target_obj(e);
    indev = lv_indev_active();

    if(indev == NULL)
    {
        return;
    }

    lv_indev_get_vect(indev, &point);

    x = lv_obj_get_x_aligned(obj) + point.x;
    y = lv_obj_get_y_aligned(obj) + point.y;
    lv_obj_set_pos(obj, x, y);
}

/* Periodic timer to confirm the project is running */
#define DEMO_LVGL_TIMER_PERIOD_MS 500

static void timer_callback(lv_timer_t * timer)
{
    (void)timer;
    LV_LOG("Project timer callback");
}

static void setup_timer(void)
{
    lv_timer_create(timer_callback, DEMO_LVGL_TIMER_PERIOD_MS, NULL);
}

void gg_event_init_screen(gg_ui_t * ui)
{
    lv_obj_add_event_cb(ui->screen.rect, rect_event_handler, LV_EVENT_PRESSING, NULL);
    setup_timer();
}
