/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gui_guider.h"
#include "gg_utils.h"
#include "gg_event.h"

void setup_layer_top(gg_ui_t * ui)
{
    /* Setup lv_layer_top() */

    /* layer_top custom code */

    /* Update layer_top layout. */
    lv_obj_update_layout(lv_layer_top());

    /* Init events of layer_top. */
    gg_event_init_layer_top(ui);
}
