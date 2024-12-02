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

#if defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)
#include "fsl_component_led.h"
#endif

/* -------------------------------------------------------------------------- */
/*                             Public definitions                             */
/* -------------------------------------------------------------------------- */

#ifndef BOARD_BUTTON0_GPIO_PIN_DEFAULT_STATE
#define BOARD_BUTTON0_GPIO_PIN_DEFAULT_STATE 1U
#endif

#ifndef BOARD_BUTTON0_GPIO_PORT_INSTANCE
#define BOARD_BUTTON0_GPIO_PORT_INSTANCE BOARD_SW2_GPIO_PORT
#endif

#ifndef BOARD_BUTTON0_GPIO_PIN
#define BOARD_BUTTON0_GPIO_PIN BOARD_SW2_GPIO_PIN
#endif

#define BOARD_RGB_LED_COUNT 1

#define BOARD_RGB_GREEN_GPIO_PORT_INSTANCE     0U
#define BOARD_RGB_GREEN_GPIO_PIN               12U
#define BOARD_RGB_GREEN_GPIO_PIN_DEFAULT_STATE 0U

#define BOARD_RGB_BLUE_GPIO_PORT_INSTANCE     0U
#define BOARD_RGB_BLUE_GPIO_PIN               0U
#define BOARD_RGB_BLUE_GPIO_PIN_DEFAULT_STATE 0U

#define BOARD_RGB_RED_GPIO_PORT_INSTANCE     0U
#define BOARD_RGB_RED_GPIO_PIN               1U
#define BOARD_RGB_RED_GPIO_PIN_DEFAULT_STATE 0U

/* -------------------------------------------------------------------------- */
/*                              Public prototypes                             */
/* -------------------------------------------------------------------------- */

#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)
/*!
 * \brief Initializes the Button0 (HW mapping depends on the board used)
 *
 * \param[in] buttonHandle the button handle
 */
void BOARD_InitButton0(button_handle_t buttonHandle);
#endif /* defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0) */

#if defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)
/*!
 * \brief This function is used to initialize RGB LED.
 *
 * \param ledHandle The led handle
 */
void BOARD_InitRgbLed(led_handle_t ledHandle);
#endif /* defined(gAppLedCnt_c) && (gAppLedCnt_c > 0) */

#endif /* _BOARD_COMP_H_ */
