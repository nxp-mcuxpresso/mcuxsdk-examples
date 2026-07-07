/* Copyright 2026 NXP */

#include "gui_guider.h"
#include "gg_utils.h"
#include "gg_event.h"

void setup_layer_sys(gg_ui_t * ui)
{
    // Setup lv_layer_sys()

    // layer_sys custom code

    // Update layer_sys layout.
    lv_obj_update_layout(lv_layer_sys());

    // Init events of layer_sys.
    gg_event_init_layer_sys(ui);
}
