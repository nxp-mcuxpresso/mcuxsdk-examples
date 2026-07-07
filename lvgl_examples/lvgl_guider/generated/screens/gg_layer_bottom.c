/* Copyright 2026 NXP */

#include "gui_guider.h"
#include "gg_utils.h"
#include "gg_event.h"

void setup_layer_bottom(gg_ui_t * ui)
{
    // Setup lv_layer_bottom()

    // layer_bottom custom code

    // Update layer_bottom layout.
    lv_obj_update_layout(lv_layer_bottom());

    // Init events of layer_bottom.
    gg_event_init_layer_bottom(ui);
}
