/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _UI_BUTTONS_H
#define _UI_BUTTONS_H

#include "haptic.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

typedef enum _ui_buttons_keypad_event_en
{
    UI_BUTTONS_KEYPAD_RELEASE     = 0, /*!< Release event */
    UI_BUTTONS_KEYPAD_TOUCH       = 1, /*!< Key-touch event */
    UI_BUTTONS_KEYPAD_AUTOREPEAT  = 2, /*!< Auto-repeat event */
    UI_BUTTONS_KEYPAD_MULTI_TOUCH = 3, /*!< Key-multi touch event */
} ui_buttons_keypad_event_en;

typedef enum _ui_buttons_slider_event_en
{
    UI_BUTTONS_SLIDER_RELEASE     = 0, /*!< Release event */
    UI_BUTTONS_SLIDER_TOUCH       = 1, /*!< Key-touch event */
} ui_buttons_slider_event_en;

typedef enum _ui_buttons_rotary_event_en
{
    UI_BUTTONS_ROTARY_RELEASE     = 0, /*!< Release event */
    UI_BUTTONS_ROTARY_TOUCH       = 1, /*!< Key-touch event */
} ui_buttons_rotary_event_en;

typedef enum _ui_buttons_type_en
{
    UI_BUTTONS_TYPE_NONE       = 0u,
    UI_BUTTONS_TYPE_NORMAL     = 1u,
} ui_buttons_type_en;

typedef struct _ui_button
{
    haptic_effect_t hapticEffect;   /*!< haptic effect */
    uint8_t type;                   /*!< none (0), normal (1) */
    uint8_t label;                  /*!< Label */
    uint16_t countPressMax;         /*!< Time interval for long press, counts at nt_trigger rate */
    uint16_t countPress;            /*!< Long press tick counter */
    uint8_t flagCountReached;       /*!< Counter limit detection flag */
    uint16_t countEvnetTriggered;   /*!< Flag to notify count event was triggered */
    uint16_t autorepeatTouchVal;    /*!< Value to count while button is pressed to execute haptic trigger */
} ui_button;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Initialize user interface button module */
uint16_t ui_buttons_init( ui_button*** pButtons );

#endif
