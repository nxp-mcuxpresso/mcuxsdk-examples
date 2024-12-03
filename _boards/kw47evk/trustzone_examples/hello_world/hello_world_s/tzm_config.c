/*
 * Copyright 2021-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "tzm_config.h"

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/* SAU region boundaries */
#define SAU_REGION_0_BASE 0x00010000U
#define SAU_REGION_0_END  0x000FFFFFU
#define SAU_REGION_1_BASE 0x20004000U
#define SAU_REGION_1_END  0x2001BFFFU
#define SAU_REGION_2_BASE 0x1000FE00U
#define SAU_REGION_2_END  0x1000FFFFU
#define SAU_REGION_3_BASE 0x40000000U
#define SAU_REGION_3_END  0x4FFFFFFFU

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitTrustZone
 * Description   :
 *
 * END ****************************************************************************************************************/

/***********************************************************************************************************************
 * BOARD_InitTrustZone function
 **********************************************************************************************************************/
void BOARD_InitTrustZone()
{
    /* SAU / IDAU configuration */

    /* Set SAU Control register: Disable SAU and All Secure */
    SAU->CTRL = 0;

    /* Set SAU region number */
    SAU->RNR = 0;
    /* Region base address */
    SAU->RBAR = SAU_REGION_0_BASE & SAU_RBAR_BADDR_Msk;
    /* Region end address */
    SAU->RLAR = (SAU_REGION_0_END & SAU_RLAR_LADDR_Msk) | ((0U << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk) |
                ((1U << SAU_RLAR_ENABLE_Pos) & SAU_RLAR_ENABLE_Msk);

    /* Set SAU region number */
    SAU->RNR = 1;
    /* Region base address */
    SAU->RBAR = SAU_REGION_1_BASE & SAU_RBAR_BADDR_Msk;
    /* Region end address */
    SAU->RLAR = (SAU_REGION_1_END & SAU_RLAR_LADDR_Msk) | ((0U << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk) |
                ((1U << SAU_RLAR_ENABLE_Pos) & SAU_RLAR_ENABLE_Msk);

    /* Set SAU region number */
    SAU->RNR = 2;
    /* Region base address */
    SAU->RBAR = SAU_REGION_2_BASE & SAU_RBAR_BADDR_Msk;
    /* Region end address */
    SAU->RLAR = (SAU_REGION_2_END & SAU_RLAR_LADDR_Msk) | ((1U << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk) |
                ((1U << SAU_RLAR_ENABLE_Pos) & SAU_RLAR_ENABLE_Msk);

    /* Set SAU region number */
    SAU->RNR = 3;
    /* Region base address */
    SAU->RBAR = SAU_REGION_3_BASE & SAU_RBAR_BADDR_Msk;
    /* Region end address */
    SAU->RLAR = (SAU_REGION_3_END & SAU_RLAR_LADDR_Msk) | ((0U << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk) |
                ((1U << SAU_RLAR_ENABLE_Pos) & SAU_RLAR_ENABLE_Msk);

    /* Force memory writes before continuing */
    __DSB();
    /* Flush and refill pipeline with updated permissions */
    __ISB();

    /* Set IDAU Control register: enable IDAU */
    TRDC->TRDC_IDAU_CR = (TRDC->TRDC_IDAU_CR & 0xFFFFFFFEU) | 0x00000001U;

    /* Set SAU Control register: Enable SAU and All Secure (applied only if disabled) */
    SAU->CTRL = ((0U << SAU_CTRL_ALLNS_Pos) & SAU_CTRL_ALLNS_Msk) | ((1U << SAU_CTRL_ENABLE_Pos) & SAU_CTRL_ENABLE_Msk);

    /* TODO: TRDC configuration */
}

/***********************************************************************************************************************
 * BOARD_InitBootTEE function
 **********************************************************************************************************************/
void BOARD_InitBootTEE()
{
    BOARD_InitTrustZone();
}
