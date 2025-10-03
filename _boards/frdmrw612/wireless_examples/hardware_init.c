/*
 * Copyright 2024-2025 NXP
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
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
#include "psa/crypto.h"
#else
#include "els_pkc_mbedtls.h"
#endif /* CONFIG_BT_SMP */
#else
#include "mcux_psa_els_pkc_common_init.h"
#endif
#include "fsl_debug_console.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

/* On FRDM-RW612 boards, the Xtal32k is present but not connected by default
 * An hardware rework is needed to connect the Xtal32k, so disable this flag with caution */
#ifndef gBoardUseFro32k_d
#define gBoardUseFro32k_d 1
#endif

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
void bt_psa_crypto_init(void)
{
    psa_status_t status;

    status = psa_crypto_init();
    if (status != PSA_SUCCESS) {
        PRINTF("Failed to initialize PSA crypto");
    }
    assert(status == PSA_SUCCESS);
}
#endif /* CONFIG_BT_SMP */

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
#if defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0)
    DbgConsole_Init(0, 0, kSerialPort_BleWu, 0);
#else
    BOARD_InitDebugConsole();
#endif
#if !(defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
    (void)CRYPTO_InitHardware();
#endif /* not CONFIG_BT_SMP */
#if defined(gBoardUseFro32k_d) && (gBoardUseFro32k_d > 0)
    CLOCK_AttachClk(kRC32K_to_CLK32K);
#else
    CLOCK_EnableXtal32K(true);
    CLOCK_AttachClk(kXTAL32K_to_CLK32K);
#endif
}
