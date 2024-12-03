/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_sentinel.h"
/*${header:end}*/

#ifdef ENABLE_RAM_VECTOR_TABLE
AT_QUICKACCESS_SECTION_CODE(void WDOG_IRQHandler(void));
#endif

/*${function:start}*/
void BOARD_InitHardware(void)
{
    uint16_t Soc_id  = 0;
    uint16_t Soc_rev = 0;
    uint32_t ret;

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot();
        SDK_DelayAtLeastUs(4000000U, SystemCoreClock); /* wait 4 seconds for uboot */
    }
    else                                               /* low power boot type */
    {
        BOARD_SetTrdcGlobalConfig();
    }

#ifdef ENABLE_RAM_VECTOR_TABLE
    InstallIRQHandler(WDOG0_IRQn, (uint32_t)WDOG_IRQHandler);
#endif
    NVIC_EnableIRQ(WDOG0_IRQn);

    ret = SENTINEL_GetSocInfo(&Soc_id, &Soc_rev);
    if ((IMX8ULP_SOC_ID == Soc_id && IMX8ULP_SOC_REV_A0 == Soc_rev) || (ret == BASELINE_FAILURE_IND))
    {
        /* bugfix: After rtd reset, upower will be hung
         * NOTE: temporary workaround for current silicon revision.
         *       It will be deleted in next revision with bugfix.*/
        SIM_AD->DGO_GP3 = SIM_AD_DGO_GP3_RESET1_B_CONFIG_DFD(0x3f) | SIM_AD_DGO_GP3_RESET1_B_CONFIG_DFCS(1) |
                          SIM_AD_DGO_GP3_RESET1_B_CONFIG_DFE(1) | SIM_AD_DGO_GP3_RESET1_B_CONFIG_ODE(1) |
                          SIM_AD_DGO_GP3_RESET1_B_CONFIG_PFE(1) | SIM_AD_DGO_GP3_RESET1_B_CONFIG_PE(1) |
                          SIM_AD_DGO_GP3_RESET1_B_CONFIG_PS(1);
        SIM_AD->DGO_CTRL0 |= SIM_AD_DGO_CTRL0_UPDATE_DGO_GP3(1);
    }
}
/*${function:end}*/
