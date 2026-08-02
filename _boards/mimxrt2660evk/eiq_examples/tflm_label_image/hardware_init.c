/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "board_init.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "fsl_power.h"

void cleanCache_by_Addr(uint32_t addr, uint32_t size)
{
    if (SCB->CCR & SCB_CCR_DC_Msk) {
        SCB_CleanDCache_by_Addr((uint32_t *)addr, size);
    }
    /* Do memory barrier */
    __DSB();
    __ISB();
}

void BOARD_Init()
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();

    // TRDC config for NPU
    CMPT__TRDC->MDA_DFMT1[kTRDC_CMPT_MasterNPU].MDA_W_DFMT1[0] = TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    POWER_SetDomainRunMode(kPOWER_DomainNpu, kPDCON_EventNoneOrActive);
    CLOCK_EnableClock(kCLOCK_CMPT_npu_core);
    CLOCK_EnableClock(kCLOCK_CMPT_npu_mem);

    ARM_MPU_SetRegion(10U, ARM_MPU_RBAR(0x20900000, ARM_MPU_SH_NON, 0U, 0U, 0U), ARM_MPU_RLAR(0x20900FFF, 0U));
}
