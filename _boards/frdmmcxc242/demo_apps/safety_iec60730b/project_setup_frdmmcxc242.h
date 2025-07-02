/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PROJECT_SETUP_H_
#define _PROJECT_SETUP_H_

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
void ReferenceTimerInit(uint8_t call_type);
void ClockInit(void);
void PortInit(uint32_t gpio, uint32_t pcr, uint8_t pinNum, uint8_t pinDir, uint8_t pinMux);
void AdcInit(void);
void SerialInit(void);

#ifdef __cplusplus
}
#endif

#endif /* _PROJECT_SETUP_H_ */
