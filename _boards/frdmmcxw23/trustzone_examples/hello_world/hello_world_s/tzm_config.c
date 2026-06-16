/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "tzm_config.h"

//********************************************************************
//*** Definitions ****************************************************
//********************************************************************
/* SAU region boundaries */
#define REGION_0_BASE 0
#define REGION_0_END  0x0FFFFFFFU
#define REGION_1_BASE 0x20004000U
#define REGION_1_END  0xEFFFFFFFU
#define REGION_2_BASE 0x1000FE00U
#define REGION_2_END  0x1000FFFFU
/*!
 * @brief TrustZone initialization
 *
 * The function configures SAU and AHB.
 */
void BOARD_InitTrustZone()
{
    //####################################################################
    //### SAU configuration ##############################################
    //####################################################################

    /* Set SAU Control register: Disable SAU and All Secure */
    SAU->CTRL = 0;

    /* Set SAU region number */
    SAU->RNR = 0;
    /* Region base address */
    SAU->RBAR = REGION_0_BASE & SAU_RBAR_BADDR_Msk;
    /* Region end address */
    SAU->RLAR = ((REGION_0_END & SAU_RLAR_LADDR_Msk) | ((0U << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk)) |
                ((1U << SAU_RLAR_ENABLE_Pos) & SAU_RLAR_ENABLE_Msk);

    /* Set SAU region number */
    SAU->RNR = 0x00000001U;
    /* Region base address */
    SAU->RBAR = REGION_1_BASE & SAU_RBAR_BADDR_Msk;
    /* Region end address */
    SAU->RLAR = ((REGION_1_END & SAU_RLAR_LADDR_Msk) | ((0U << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk)) |
                ((1U << SAU_RLAR_ENABLE_Pos) & SAU_RLAR_ENABLE_Msk);

    /* Set SAU region number */
    SAU->RNR = 0x00000002U;
    /* Region base address */
    SAU->RBAR = REGION_2_BASE & SAU_RBAR_BADDR_Msk;
    /* Region end address */
    SAU->RLAR = ((REGION_2_END & SAU_RLAR_LADDR_Msk) | ((1U << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk)) |
                ((1U << SAU_RLAR_ENABLE_Pos) & SAU_RLAR_ENABLE_Msk);

    /* Force memory writes before continuing */
    __DSB();
    /* Flush and refill pipeline with updated permissions */
    __ISB();
    /* Set SAU Control register: Enable SAU and All Secure (applied only if disabled) */
    SAU->CTRL = 0x00000001U;

    //####################################################################
    //### AHB Configurations #############################################
    //####################################################################

    //--------------------------------------------------------------------
    //--- AHB Security Level Configurations ------------------------------
    //--------------------------------------------------------------------
    /* Configuration of AHB Secure Controller
     * Possible values for every memory sector or peripheral rule:
     *  0    Non-secure, user access allowed.
     *  1    Non-secure, privileged access allowed.
     *  2    Secure, user access allowed.
     *  3    Secure, privileged access allowed. */

    //--- Security level configuration of memories -----------------------
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[0] = 0x00000033U;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[0]   = 0;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[1]   = 0;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[2]   = 0;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[3]   = 0;
    AHB_SECURE_CTRL->SEC_CTRL_RAMX[0].MEM_RULE[0]                     = 0;
    AHB_SECURE_CTRL->SEC_CTRL_RAM0[0].MEM_RULE[0]                     = 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_RAM1[0].MEM_RULE[0]                     = 0;
    AHB_SECURE_CTRL->SEC_CTRL_RAM2[0].MEM_RULE[0]                     = 0;

    //--- Security level configuration of peripherals --------------------
    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL0 = 0x00000033U;
    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL1 = 0;
    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL2 = 0;
    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL0 = 0;
    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL1 = 0;
    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL2 = 0;
    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL3 = 0;
    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT7_SLAVE0_RULE                        = 0x03000000U;
    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT7_SLAVE1_RULE                        = 0;
    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE0_RULE                        = 0;
    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE1_RULE                        = 0;
    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9[0].SLAVE0_RULE                     = 0;
    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9[0].SLAVE1_RULE                     = 0;

    //--- Security level configuration of masters ------------------------
    AHB_SECURE_CTRL->MASTER_SEC_LEVEL        = 0;
    AHB_SECURE_CTRL->MASTER_SEC_ANTI_POL_REG = 0x3FFFFFFFU;

    //--------------------------------------------------------------------
    //--- Pins: Reading GPIO state ---------------------------------------
    //--------------------------------------------------------------------
    // Possible values for every pin:
    //  0b0    Deny
    //  0b1    Allow
    //--------------------------------------------------------------------
    AHB_SECURE_CTRL->SEC_GPIO_MASK0 = 0xFFFFFFFFU;

    //--------------------------------------------------------------------
    //--- Interrupts: Interrupt security configuration -------------------
    //--------------------------------------------------------------------
    // Possible values for every interrupt:
    //  0b0    Secure
    //  0b1    Non-secure
    //--------------------------------------------------------------------
    NVIC->ITNS[0] = 0;
    NVIC->ITNS[1] = 0;

    //--------------------------------------------------------------------
    //--- Global Options -------------------------------------------------
    //--------------------------------------------------------------------
    SCB->AIRCR = (SCB->AIRCR & 0x000009FF7U) | 0x005FA0000U;
    SCB->SCR &= 0x0FFFFFFF7U;
    SCB->SHCSR &= 0x0FFF7FFFFU;
    SCB->NSACR                               = 0x00000C03U;
    SCnSCB->CPPWR                            = 0;
    AHB_SECURE_CTRL->SEC_MASK_LOCK           = 0x00000AAAU;
    AHB_SECURE_CTRL->MASTER_SEC_LEVEL        = (AHB_SECURE_CTRL->MASTER_SEC_LEVEL & 0x03FFFFFFFU) | 0x080000000U;
    AHB_SECURE_CTRL->MASTER_SEC_ANTI_POL_REG = (AHB_SECURE_CTRL->MASTER_SEC_ANTI_POL_REG & 0x03FFFFFFFU) | 0x080000000U;
    AHB_SECURE_CTRL->CPU0_LOCK_REG           = 0x800002AAU;
    AHB_SECURE_CTRL->MISC_CTRL_REG           = (AHB_SECURE_CTRL->MISC_CTRL_REG & 0x0FFFF0003U) | 0x00000AAA4U;
    AHB_SECURE_CTRL->MISC_CTRL_DP_REG        = 0x0000AAA5U;
}
