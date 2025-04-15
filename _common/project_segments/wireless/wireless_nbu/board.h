/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdint.h>
#include "fsl_device_registers.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Return the current clock select
 *
 */
uint32_t BOARD_GetSystemCoreClockSel(void);

/**
 * @brief Updates SystemCoreClock global variable
 *
 */
void BOARD_SystemCoreClockUpdate(void);

/**
 * @brief Updates and returns current SystemCoreClock frequency
 *
 * @return uint32_t SystemCoreClock frequency
 */
uint32_t BOARD_GetSystemCoreClockFreq(void);

/**
 * @brief Returns current FRO post divider clock select
 *
 * @return uint32_t FRO post divider clock select
 */
uint32_t BOARD_GetSystemCoreClockSel(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
