/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#if (IMXRTL3459AAA_cm55_core1_SERIES)
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
 * CM55 core1 (NSS) has no access to the CGU; clocks are brought up by the
 * CSS host (core0) before core1 is released, so this is a no-op kept only to
 * satisfy the shared board clock component build.
 */
void BOARD_InitBootClocks(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif
#endif /* _CLOCK_CONFIG_H_ */
