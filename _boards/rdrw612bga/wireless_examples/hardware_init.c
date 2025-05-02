/*
 * Copyright 2020-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#if !defined(gSecLibUsePsa_d) || (gSecLibUsePsa_d == 0)
#include "els_pkc_mbedtls.h"
#else
#include "mcux_psa_els_pkc_common_init.h"
#endif
#include "fsl_debug_console.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

/* On RD-RW612-BGA boards, the Xtal32k is present and connected by default
 * so we use it by default. If an hardware rework done on the board disconnects
 * the Xtal32k, this flag should be set to 1 to fall back to the Fro32k */
#ifndef gBoardUseFro32k_d
#define gBoardUseFro32k_d 0
#endif

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
#if defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0)
    DbgConsole_Init(0, 0, kSerialPort_BleWu, 0);
#else
    BOARD_InitDebugConsole();
#endif
    (void)CRYPTO_InitHardware();
#if defined(gBoardUseFro32k_d) && (gBoardUseFro32k_d > 0)
    CLOCK_AttachClk(kRC32K_to_CLK32K);
#else
    CLOCK_EnableXtal32K(true);
    CLOCK_AttachClk(kXTAL32K_to_CLK32K);
#endif
}
