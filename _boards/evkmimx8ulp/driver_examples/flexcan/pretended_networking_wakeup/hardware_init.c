/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_reset.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(EXAMPLE_CAN_CLOCK_NAME, EXAMPLE_CAN_CLOCK_SOURCE);
    RESET_PeripheralReset(kRESET_Flexcan);
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_Pcc1BusIpSrcSysOscDiv2);
    RESET_PeripheralReset(kRESET_Lpi2c0);

    pca6416a_handle_t handle;
    BOARD_InitPCA6416A(&handle);
    PCA6416A_SetDirection(&handle, 1 << 4U, kPCA6416A_Output);
    PCA6416A_ClearPins(&handle, 1 << 4U);

    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot(); /* Must handshake with uboot, unless will get issues(such as: SoC reset all the
                                       time) */
    }
    else                            /* low power boot type */
    {
        BOARD_SetTrdcGlobalConfig();
    }
}
void APP_SetLowerPowerConfig()
{
    uint32_t reg;
    /* Allows the RTD_PMCTRL[0] field to be configured for Sleep mode. */
    reg = CMC_RTD->RTD_PMPROT;
    reg &= ~(CMC_RTD_PMPROT_ADS_MASK | CMC_RTD_PMPROT_APD_MASK | CMC_RTD_PMPROT_ADPD_MASK | CMC_RTD_PMPROT_AHLD_MASK);
    reg |= CMC_RTD_PMPROT_AS_MASK;
    CMC_RTD->RTD_PMPROT = reg;
    /* Select Domain 0 to be FLEXCAN Stop and "Stop ACK" Domain Owner. */
    reg = PCC1->PCC_FLEXCAN;
    reg &= ~PCC1_PCC_FLEXCAN_SSADO_MASK;
    reg |= PCC1_PCC_FLEXCAN_SSADO(1);
    PCC1->PCC_FLEXCAN = reg;
}
void APP_EnterLowerPowerMode()
{
    /* Select Sleep mode when a core executes WFI or WFE instruction. */
    CMC_RTD->RTD_PMCTRL = CMC_RTD_PMCTRL_RTD_LPMODE(1U);
    /* Before executing WFI instruction read back the last register to
     * ensure all registers writes have completed. */
    (void)CMC_RTD->RTD_PMCTRL;
    __DSB();
    __WFI();
    __ISB();
}
/*${function:end}*/
