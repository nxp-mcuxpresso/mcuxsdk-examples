/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _TZM_CONFIG_H_
#define _TZM_CONFIG_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/* SAU region boundaries */
#define SAU_REGION_0_BASE 0x20023000U
#define SAU_REGION_0_END  0x2003FFFFU
#define SAU_REGION_1_BASE 0x0FFE0000U
#define SAU_REGION_1_END  0x0FFFFFFFU
#define SAU_REGION_2_BASE 0x1FFDFE00U
#define SAU_REGION_2_END  0x1FFDFFFFU
/***********************************************************************************************************************
 * Initialize TrustZone
 **********************************************************************************************************************/
void BOARD_InitTrustZone(void);

#if defined(__cplusplus)
}
#endif

#endif /* _TZM_CONFIG_H_ */
