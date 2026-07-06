/*
 * Copyright 2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board_dbg_logger_port.h"
#include "fsl_component_serial_manager.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/************************************************************************************
 * Private memory declarations
 ************************************************************************************/
#if defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0)
static SERIAL_MANAGER_WRITE_HANDLE_DEFINE(s_dbgLoggerPortWriteHandle);
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
int BOARD_DbgLoggerPortInit(void)
{
    int ret = -1;
#if defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0)
    /* Open a write handle on the main serial interface used by the application */
    if (SerialManager_OpenWriteHandle((serial_handle_t)gSerMgrIf,
                                      (serial_write_handle_t)s_dbgLoggerPortWriteHandle) == kStatus_SerialManager_Success)
    {
        ret = 0;
    }
#endif

    return ret;
}

int BOARD_DbgLoggerPortWrite(const uint8_t *data, uint16_t len)
{
    int ret = -1;
#if defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0)
    if (SerialManager_WriteBlocking((serial_write_handle_t)s_dbgLoggerPortWriteHandle, (uint8_t *)data, len) ==
        kStatus_SerialManager_Success)
    {
        ret = 0;
    }
#endif

    return ret;
}

int BOARD_DbgLoggerPortReinit(void)
{
    /* The main serial port is managed by the application/serial manager and is
     * restored on its own after low power exit, so no extra action is needed. */
    return 0;
}
