/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
#include "fsl_inputmux.h"
#include "fsl_spc.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Set the LDO_CORE VDD regulator to 1.0 V voltage level */
    spc_active_mode_core_ldo_option_t ldoOpt = {
        .CoreLDOVoltage       = kSPC_CoreLDO_MidDriveVoltage,
        .CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength,
    };
    SPC_SetActiveModeCoreLDORegulatorConfig(SPC0, &ldoOpt);

    /* Set the DCDC VDD regulator to 1.0 V voltage level */
    spc_active_mode_dcdc_option_t dcdcOpt = {
        .DCDCVoltage       = kSPC_DCDC_MidVoltage,
        .DCDCDriveStrength = kSPC_DCDC_NormalDriveStrength,
    };
    SPC_SetActiveModeDCDCRegulatorConfig(SPC0, &dcdcOpt);

    /* Enable clock for PCLK. */
    CLOCK_AttachClk(kMAIN_CLK_to_CLKOUT);
    CLOCK_SetClkDiv(kCLOCK_DivClkOut, 25U);

    /* Enable flexio clock */
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1u);
    CLOCK_AttachClk(kPLL0_to_FLEXIO);

    /* Enable RST/RS/CS pins' GPIO clock. */
    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio2);
    CLOCK_EnableClock(kCLOCK_Gpio4);

    /* Init camera I2C clock. */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM7);
    CLOCK_EnableClock(kCLOCK_LPFlexComm7);
    CLOCK_EnableClock(kCLOCK_LPI2c7);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom7Clk, 1u);

    /* Attach camera signals. P0_4/P0_11/P0_5 is set to EZH_CAMERA_VSYNC/EZH_CAMERA_HSYNC/EZH_CAMERA_PCLK. */
    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, 0, kINPUTMUX_GpioPort0Pin4ToSmartDma);
    INPUTMUX_AttachSignal(INPUTMUX0, 1, kINPUTMUX_GpioPort0Pin11ToSmartDma);
    INPUTMUX_AttachSignal(INPUTMUX0, 2, kINPUTMUX_GpioPort0Pin5ToSmartDma);
    /* Turn off clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX0);
}
/*${function:end}*/
