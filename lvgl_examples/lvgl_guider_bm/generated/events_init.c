/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

static unsigned int counter = 0;
static char buf[4];

static void blueCounter_plus_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        counter++;
        sprintf(buf,  "%d", counter);
        lv_label_set_text(guider_ui.blueCounter_counter, buf);
        break;
    }
    case LV_EVENT_LONG_PRESSED_REPEAT:
    {
        counter++;
        sprintf(buf,  "%d", counter);
        lv_label_set_text(guider_ui.blueCounter_counter, buf);
        break;
    }
    default:
        break;
    }
}

static void blueCounter_minus_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        if(counter) counter--;
        sprintf(buf,  "%d", counter);
        lv_label_set_text(guider_ui.blueCounter_counter, buf);
        break;
    }
    case LV_EVENT_LONG_PRESSED_REPEAT:
    {
        if(counter) counter--;
        sprintf(buf,  "%d", counter);
        lv_label_set_text(guider_ui.blueCounter_counter, buf);
        break;
    }
    default:
        break;
    }
}

void events_init_blueCounter (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->blueCounter_plus, blueCounter_plus_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->blueCounter_minus, blueCounter_minus_event_handler, LV_EVENT_ALL, ui);
}

/* Create a lvgl periodic timer, to monitor the project is running. */
#define DEMO_LVGL_TIMER_PERIOD_MS 500

static void timer_callback(lv_timer_t *timer)
{
    (void)timer;
    LV_LOG("Project timer callback");
}

static void setup_timer(void)
{
    lv_timer_create(timer_callback, DEMO_LVGL_TIMER_PERIOD_MS, NULL);
}

void events_init(lv_ui *ui)
{
    (void)ui;
    setup_timer();
}
