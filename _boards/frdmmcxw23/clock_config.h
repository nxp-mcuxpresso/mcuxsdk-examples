/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

/*!
 * @brief This function executes default configuration of clocks.
 *
 */
void BOARD_InitBootClocks(void);

/*!
 * @brief This function configures 12 MHz system FRO clock.
 */
void BOARD_BootClockFRO12M(void);

/*!
 * @brief This function configures 24 MHz system FRO clock.
 */
void BOARD_BootClockFRO24M(void);

/*!
 * @brief This function configures 32 MHz system FRO clock.
 */
void BOARD_BootClockFRO32M(void);

/*!
 * @brief This function configures 32 MHz system XTAL clock.
 */
void BOARD_BootClockXTAL32M(void);

/*!
 * @brief This function configures the 32kHz oscillator.
 */
void BOARD_Configure32kOsc(void);

#endif /* _CLOCK_CONFIG_H_ */
