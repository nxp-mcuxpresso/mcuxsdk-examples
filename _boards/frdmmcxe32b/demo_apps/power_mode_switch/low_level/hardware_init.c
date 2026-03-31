/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_siul2.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitCMPPins();
    BOARD_InitDebugConsole();
}

void APP_DeinitDEBUG_UARTPins(void)
{
    const siul2_pin_settings_t pin_pte3_config = {.base           = SIUL2,
                                                  .pinPortIdx     = 131u,
                                                  .mux            = kPORT_MUX_AS_GPIO,
                                                  .safeMode       = kPORT_SAFE_MODE_DISABLED,
                                                  .inputFilter    = kPORT_INPUT_FILTER_NOT_AVAILABLE,
                                                  .driveStrength  = kPORT_DRIVE_STRENTGTH_DISABLED,
                                                  .pullConfig     = kPORT_INTERNAL_PULL_NOT_ENABLED,
                                                  .pullKeep       = kPORT_PULL_KEEP_DISABLED,
                                                  .invert         = kPORT_INVERT_DISABLED,
                                                  .inputBuffer    = kPORT_INPUT_BUFFER_DISABLED,
                                                  .outputBuffer   = kPORT_OUTPUT_BUFFER_DISABLED,
                                                  .adcInterleaves = {kMUX_MODE_NOT_AVAILABLE, kMUX_MODE_NOT_AVAILABLE},
                                                  .initValue      = 0u};
    SIUL2_PinInit(&pin_pte3_config);
    SIUL2_SetPinInputBuffer(SIUL2, 131u, false, 192, kPORT_INPUT_MUX_NO_INIT);

    const siul2_pin_settings_t pin_pte14_config = {.base           = SIUL2,
                                                   .pinPortIdx     = 142u,
                                                   .mux            = kPORT_MUX_AS_GPIO,
                                                   .safeMode       = kPORT_SAFE_MODE_DISABLED,
                                                   .inputFilter    = kPORT_INPUT_FILTER_NOT_AVAILABLE,
                                                   .pullConfig     = kPORT_INTERNAL_PULL_NOT_ENABLED,
                                                   .pullKeep       = kPORT_PULL_KEEP_DISABLED,
                                                   .invert         = kPORT_INVERT_DISABLED,
                                                   .inputBuffer    = kPORT_INPUT_BUFFER_DISABLED,
                                                   .outputBuffer   = kPORT_OUTPUT_BUFFER_DISABLED,
                                                   .adcInterleaves = {kMUX_MODE_NOT_AVAILABLE, kMUX_MODE_NOT_AVAILABLE},
                                                   .initValue      = 0u};
    SIUL2_PinInit(&pin_pte14_config);
}

void APP_DeinitDebugConsole(void)
{
    DbgConsole_Deinit();
    APP_DeinitDEBUG_UARTPins();
}
/*${function:end}*/