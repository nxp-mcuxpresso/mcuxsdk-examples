/*!
 * Copyright 2024 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file board_comp.c
 * \brief Collection of helper to init and configure HW resources for the application such as buttons, serial interfaces
 * , IOs, ...
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "pin_mux.h"
#include "board_comp.h"

#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)
#include "fsl_component_button.h"
#endif /* defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0) */

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)

static const button_config_t button0Config = {
    .gpio =
        {
            .pinStateDefault = BOARD_BUTTON0_GPIO_PIN_DEFAULT_STATE,
            .port            = BOARD_BUTTON0_GPIO_PORT_INSTANCE,
            .pin             = BOARD_BUTTON0_GPIO_PIN,
        },
};

#endif

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)

void BOARD_InitButton0(button_handle_t buttonHandle)
{
    button_status_t ret;
    button_config_t button_config = button0Config;

    /* Init Pin mux */
    BOARD_InitPinButton0();

    /* Init button module and Gpio module */
    ret = BUTTON_Init(buttonHandle, &button_config);
    assert(ret == kStatus_BUTTON_Success);
    (void)ret;
}

#endif /* defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0) */
