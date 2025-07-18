/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PROJECT_SETUP_IMXRT_H_
#define _PROJECT_SETUP_IMXRT_H_

#include "MIMX8ML8_cm7.h"
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
void ReferenceTimerInit(uint32_t clkSource, uint32_t compare, uint32_t prescaler);
void QTMR1_Init(uint32_t compare);
void SystickInit(uint32_t compare);
void ClockInit(void);
void PortSetup(uint32_t gpio, uint8_t pinNum, uint8_t pinDir, uint32_t *muxAddr);

#ifdef __cplusplus
}
#endif

#endif /* _PROJECT_SETUP_IMXRT_H_ */
