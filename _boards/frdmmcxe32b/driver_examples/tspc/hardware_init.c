/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void DEMO_Pad1Init()
{
    SIUL2_SetPinDirection(SIUL2, 72u, kPORT_IN_OUT);
    SIUL2_SetPinOutputBuffer(SIUL2, 72u, true, kPORT_MUX_AS_GPIO);
    SIUL2_SetPinPullSel(SIUL2, 72u, kPORT_INTERNAL_PULL_UP_ENABLED);
}

void DEMO_Pad2Init()
{
    const  siul2_pin_settings_t pin_ptc9_config =
    {
        .base                        = SIUL2,
        .pinPortIdx                  = 73u,
        .mux                         = kPORT_MUX_AS_GPIO,
        .safeMode                    = kPORT_SAFE_MODE_DISABLED,
        .inputFilter                 = kPORT_INPUT_FILTER_NOT_AVAILABLE,
        .driveStrength               = kPORT_DRIVE_STRENTGTH_DISABLED,
        .pullConfig                  = kPORT_INTERNAL_PULL_UP_ENABLED,
        .slewRateCtrlSel             = kPORT_SLEW_RATE_SLOWEST,
        .pullKeep                    = kPORT_PULL_KEEP_DISABLED,
        .invert                      = kPORT_INVERT_DISABLED,
        .inputBuffer                 = kPORT_INPUT_BUFFER_ENABLED,
        .outputBuffer                = kPORT_OUTPUT_BUFFER_ENABLED,
        .adcInterleaves              = { kMUX_MODE_NOT_AVAILABLE, kMUX_MODE_NOT_AVAILABLE },
        .inputMuxReg                 = {
                                         33u
                                       },
        .inputMux                    = {
                                         kPORT_INPUT_MUX_ALT2,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT,
                                         kPORT_INPUT_MUX_NO_INIT
                                       },
        .initValue                   = 2u
    };
    SIUL2_PinInit(&pin_ptc9_config);
    SIUL2_SetGlitchFilterPrescaler(SIUL2, 1U);
    SIUL2_EnableExtInterrupt(SIUL2, 17U, kSIUL2_InterruptBothEdge, 2U);
    EnableIRQ(SIUL2_2_IRQn);
}

void DEMO_ClearPad2InterruptStatusFlags()
{
    SIUL2_ClearExtDmaInterruptStatusFlags(SIUL2, 1U << 17U);
}

void DEMO_DisablePad1OutputBuffer()
{
    SIUL2_SetPinOutputBuffer(SIUL2, 72u, false, kPORT_MUX_AS_GPIO);
}
/*${function:end}*/
