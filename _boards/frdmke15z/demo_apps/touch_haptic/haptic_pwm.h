/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HAPTIC_FTM_PWM_H_
#define _HAPTIC_FTM_PWM_H_

#include "haptic.h"
#include "fsl_ftm.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Init function*/
void haptic_pwm_init( haptic_callback_t callback );

void haptic_pwm_send( uint8_t *pData, size_t dataBytes );

void haptic_pwm_trigger( void );

size_t haptic_pwm_callback_ready( void );

void haptic_pwm_callback_clear( void );

#endif
