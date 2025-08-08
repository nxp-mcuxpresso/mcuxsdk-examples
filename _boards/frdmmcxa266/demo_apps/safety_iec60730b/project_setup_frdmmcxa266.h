/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PROJECT_SETUP_IMXRT_H_
#define _PROJECT_SETUP_IMXRT_H_

#include "safety_config.h"

#include "fsl_lpuart.h"

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
void CTIMER_initialisation(void);
void SystickInit(uint32_t compare);
void SerialInit(void);
void ClockInit(void);
void PortSetup(uint32_t gpio, uint8_t pinNum, uint8_t pinDir);
void AdcInit(void);

#ifdef __cplusplus
}
#endif

#endif /* _PROJECT_SETUP_IMXRT_H_ */
