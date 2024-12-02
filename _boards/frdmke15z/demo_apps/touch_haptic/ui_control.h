/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _UI_CONTROL_H
#define _UI_CONTROL_H

#include "ui_buttons.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
int32_t previousPos = 0;       /* Position from last rotary/slider haptic event execution */
uint8_t rotaryPosInsens = 5;   /* Minimal position delta to execute rotary touch event */
uint8_t sliderPosInsens = 7;   /* Minimal position delta to execute slider touch event */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Initialize user interface module */
void ui_control_init( void );

/* Process user interface keypad event */
void ui_control_keypad_event( ui_buttons_keypad_event_en inputEvent, uint32_t index, uint16_t nScans );

/* Process user interface slider event */
void ui_control_slider_event( ui_buttons_slider_event_en inputEvent, uint32_t position, uint16_t nScans );

/* Process user interface rotary event */
void ui_control_rotary_event( ui_buttons_rotary_event_en inputEvent, uint32_t position, uint16_t nScans );

/* Main user interfac process */
void ui_control_main( void );

#endif
