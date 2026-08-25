/*
 * Copyright 2018, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "veneer_table.h"
#include "app.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * MACROs
 ******************************************************************************/
#define PRINTF_NSE(s)      \
    DbgConsole_Printf_NSE((s), sizeof(s) - 1U)
#define STRCMP_NSE(s1, s2) \
    StringCompare_NSE((s1), sizeof(s1) - 1U, (s2), sizeof(s2) - 1U)
#define GETSTRLEN_NSE(s, callback) \
    GetStringLength_NSE((s), sizeof(s) - 1U, (callback))
      
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/* NS callback — demonstrates correct ARM CMSE pattern for receiving
 * scalar results from Secure world. Scalar values are passed by value
 * in CPU registers — NS world never dereferences any Secure memory pointer. */
void ScalarResultCallback_NS(size_t value)
{
    PRINTF_NSE("NS callback received scalar result from Secure world: ");
    DbgConsole_PrintScalar_NSE(value);
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t len;
    BOARD_InitHardware();

    PRINTF_NSE("Welcome in normal world!\r\n");
    PRINTF_NSE("This is a text printed from normal world!\r\n");

    if (STRCMP_NSE("Test1\r\n", "Test2\r\n") == 0u)
    {
        PRINTF_NSE("Both strings are equal!\r\n");
    }
    else
    {
        PRINTF_NSE("Both strings are not equal!\r\n");
    }
    
    /* Demonstrate GetStringLength_NSE — Secure world validates the string,
     * calculates its length, and notifies NS world via scalar callback.
     * This shows the correct ARM CMSE pattern: scalar (by-value) arguments
     * are safe to pass to NS callbacks, unlike pointer arguments. */
    PRINTF_NSE("Testing NS callback with scalar argument...\r\n");
    PRINTF_NSE("Input string: 'Hello TrustZone!'\r\n");

    len = GETSTRLEN_NSE("Hello TrustZone!", &ScalarResultCallback_NS);

    /* ScalarResultCallback_NS already printed the length via Secure UART.
     * Cross-check: confirm return value matches what callback received. */
    PRINTF_NSE("GetStringLength_NSE return value: ");
    DbgConsole_PrintScalar_NSE(len);
    while (1)
    {
    }
}
