/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PROJECT_SETUP_K22_H_
#define _PROJECT_SETUP_K22_H_

#include "safety_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @name Project setup functions
 * @{
 */
/*******************************************************************************
 * API
 ******************************************************************************/

void WatchdogEnable(uint32_t wd_setup_value);
void WatchdogDisable(void);
void SystickInit(uint32_t reload_value);
void ReferenceTimerInit(void);
void ClockInit(void);
void PortInit(SIUL2_Type *base, uint32_t pin, uint8_t pinDir);
void SerialInit(void);

#ifdef __cplusplus
}
#endif

#endif /* _PROJECT_SETUP_KV31_H_ */
