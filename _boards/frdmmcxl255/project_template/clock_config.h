/*
 * Copyright 2025 NXP
 * All rights reserved.
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

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes default configuration of clocks.
 *
 */
void BOARD_InitBootClocks(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function configures 12 MHz system clock.
 */
void BOARD_BootClockFRO12M(void);

/*!
 * @brief This function configures 24 MHz system clock.
 */
void BOARD_BootClockFRO24M(void);

/*!
 * @brief This function configures 48 MHz system clock.
 */
void BOARD_BootClockFRO48M(void);

/*!
 * @brief This function configures 64 MHz system clock.
 */
void BOARD_BootClockFRO64M(void);

/*!
 * @brief This function configures 96 MHz system clock.
 */
void BOARD_BootClockFRO96M(void);

#endif /* _CLOCK_CONFIG_H_ */
