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
void CTIMER_initialisation(void);
void second_timer_inicialization(void); /* Second timer for CLOKC TEST */
void SystickInit(uint32_t reload_value);
void ClockInit(void);
void PortSetup(uint8_t *pByte,
               uint32_t *pDir,
               uint32_t *pIocon,
               uint32_t pinDir,
               uint32_t pinNum,
               uint32_t pull,
               uint32_t clock_enable_shift);
void AdcInit(void);
void Tsi0SetupSelfCap(void);
void Tsi0SetupMutualCap(void);
void SerialInit(void);

#ifdef __cplusplus
}
#endif

#endif /* _PROJECT_SETUP_H_ */
