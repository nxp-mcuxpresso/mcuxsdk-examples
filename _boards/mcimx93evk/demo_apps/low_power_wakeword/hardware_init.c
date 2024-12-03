/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_iomuxc.h"
#include "fsl_lpuart.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "app_srtm.h"
#include "lpm.h"
#include "fsl_mu.h"
#include "rsc_table.h"
#include "fsl_edma.h"
#include "fsl_sai.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    /* clang-format off */
    /* 250MHz DMA clock */
    const clock_root_config_t dmaClkCfg = {
        .clockOff = false,
	.mux = kCLOCK_WAKEUPAXI_ClockRoot_MuxSysPll1Pfd0, // 1000MHz
	.div = 4
    };
    const clock_root_config_t lpi2cClkCfg = {
        .clockOff = false,
	.mux = 0, // 24MHz oscillator source
	.div = 1
    };
    const clock_root_config_t saiClkCfg = {
        .clockOff = false,
        .mux = 1, // select audiopll1out source(393216000 Hz)
        .div = 32 // output 12288000 Hz
    };
    const clock_root_config_t pdmClkCfg = {
        .clockOff = false,
        .mux = 1,
        .div = 32
    };
    sai_master_clock_t saiMasterCfg = {
        .mclkOutputEnable = true,
     };

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(kCLOCK_Root_WakeupAxi, &dmaClkCfg);
    CLOCK_SetRootClock(kCLOCK_Root_Pdm, &pdmClkCfg);
    CLOCK_SetRootClock(kCLOCK_Root_Lpi2c1, &lpi2cClkCfg);
    CLOCK_SetRootClock(kCLOCK_Root_Sai3, &saiClkCfg);
    CLOCK_SetRootClock(BOARD_ADP5585_I2C_CLOCK_ROOT, &lpi2cClkCfg);

    CLOCK_EnableClock(kCLOCK_Lptmr1);
    CLOCK_EnableClock(kCLOCK_Lpuart2);
    CLOCK_EnableClock(kCLOCK_Lpi2c1);
    CLOCK_EnableClock(kCLOCK_Sai3);

    /* Select PDM/SAI signals */
    adp5585_handle_t handle;
    BOARD_InitADP5585(&handle);
    ADP5585_SetDirection(&handle, (1 << BOARD_ADP5585_PDM_MQS_SEL) | (1 << BOARD_ADP5585_EXP_SEL), kADP5585_Output);
    ADP5585_ClearPins(&handle, (1 << BOARD_ADP5585_PDM_MQS_SEL) | (1 << BOARD_ADP5585_EXP_SEL));

    /* Select SAI3 signals */
    ADP5585_SetDirection(&handle, (1 << BOARD_ADP5585_EXP_SEL), kADP5585_Output);
    ADP5585_ClearPins(&handle, (1 << BOARD_ADP5585_EXP_SEL));

    /* select MCLK direction(Enable MCLK clock) */
    saiMasterCfg.mclkSourceClkHz = CLOCK_GetIpFreq(kCLOCK_Root_Sai3);  /* setup source clock for MCLK */
    saiMasterCfg.mclkHz          = saiMasterCfg.mclkSourceClkHz;       /* setup target clock of MCLK */
    SAI_SetMasterClockConfig(SAI3, &saiMasterCfg);

    /* copy resource table to destination address(TCM) */
    copyResourceTable();

    /* Config OSCPLL LPM setting for M33 SUSPEND */
    for (unsigned int i = OSCPLL_LPM_START; i <= OSCPLL_LPM_END; i++)
    {
        CCM_CTRL->OSCPLL[i].LPM0 |= CCM_OSCPLL_LPM0_LPM_SETTING_D2(3);
    }
}
