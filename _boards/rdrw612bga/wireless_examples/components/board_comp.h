/*!
 * Copyright 2024 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file board_comp.h
 * \brief Collection of helper to init and configure HW resources for the application such as buttons, serial interfaces
 * , IOs, ...
 *
 */

#ifndef _BOARD_COMP_H_
#define _BOARD_COMP_H_

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "board.h"

#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)
#include "fsl_component_button.h"
#endif /* defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0) */

/* -------------------------------------------------------------------------- */
/*                             Public definitions                             */
/* -------------------------------------------------------------------------- */

#ifndef BOARD_BUTTON0_GPIO_PIN_DEFAULT_STATE
#define BOARD_BUTTON0_GPIO_PIN_DEFAULT_STATE 1U
#endif

#ifndef BOARD_BUTTON0_GPIO_PORT_INSTANCE
#define BOARD_BUTTON0_GPIO_PORT_INSTANCE BOARD_SW4_GPIO_PORT
#endif

#ifndef BOARD_BUTTON0_GPIO_PIN
#define BOARD_BUTTON0_GPIO_PIN BOARD_SW4_GPIO_PIN
#endif

/* -------------------------------------------------------------------------- */
/*                              Public prototypes                             */
/* -------------------------------------------------------------------------- */

#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)
void BOARD_InitButton0(button_handle_t buttonHandle);
#endif /* defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0) */

#endif /* _BOARD_COMP_H_ */
