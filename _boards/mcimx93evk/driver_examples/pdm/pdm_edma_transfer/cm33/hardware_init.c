/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
#include "fsl_iomuxc.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
#include "fsl_codec_adapter.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    const clock_root_config_t pdmClkCfg = {
        .clockOff = false,
	.mux = 1, // select audiopll1out source(393216000 Hz)
	.div = 2 // output 196.608 Mhz
    };
    /* 250MHz DMA clock */
    const clock_root_config_t dmaClkCfg = {
        .clockOff = false,
        .mux = kCLOCK_WAKEUPAXI_ClockRoot_MuxSysPll1Pfd0, // 1000MHz
        .div = 4 // output 250 Mhz
    };

    const clock_root_config_t lpi2cClkCfg = {
        .clockOff = false,
	.mux = 0, // 24MHz oscillator source
	.div = 1
    };
    /* clang-format on */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(PDM_CLOCK_ROOT, &pdmClkCfg);
    CLOCK_EnableClock(PDM_CLOCK_GATE);
    PRINTF("PDM CLOCK FREQ %d", CLOCK_GetIpFreq(PDM_CLOCK_ROOT));
    CLOCK_SetRootClock(EXAMPLE_DMA_CLOCK_ROOT, &dmaClkCfg);
    CLOCK_EnableClock(EXAMPLE_DMA_CLOCK_GATE);
    PRINTF("dma CLOCK FREQ %d", CLOCK_GetIpFreq(EXAMPLE_DMA_CLOCK_ROOT));
    CLOCK_SetRootClock(BOARD_ADP5585_I2C_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(BOARD_ADP5585_I2C_CLOCK_GATE);

    /* Select PDM/SAI signals */
    adp5585_handle_t handle;
    BOARD_InitADP5585(&handle);
    ADP5585_SetDirection(&handle, (1 << BOARD_ADP5585_PDM_MQS_SEL), kADP5585_Output);
    ADP5585_ClearPins(&handle, (1 << BOARD_ADP5585_PDM_MQS_SEL));
}
/*${function:end}*/
