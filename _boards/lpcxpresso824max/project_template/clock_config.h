/*
 * Copyright 2018 NXP
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
#define BOARD_XTAL0_CLK_HZ 12000000U /*!< Board xtal0 frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ 32768U  /*!< Board xtal32K frequency in Hz */

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

/*******************************************************************************
 ******************** Configuration BOARD_BootClockIRC12M **********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockIRC12M configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCKIRC12M_CORE_CLOCK 12000000U /*!< Core clock frequency: 12000000Hz */

/*******************************************************************************
 * API for BOARD_BootClockIRC12M configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockIRC12M(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ******************** Configuration BOARD_BootClockPll24M **********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockPll24M configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCKPLL24M_CORE_CLOCK 24000000U /*!< Core clock frequency: 24000000Hz */

/*******************************************************************************
 * API for BOARD_BootClockPll24M configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockPll24M(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */
