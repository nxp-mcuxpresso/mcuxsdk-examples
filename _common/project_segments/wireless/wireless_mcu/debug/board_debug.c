/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/

#include <stdint.h>
#include "board_platform.h"
#include "fsl_common.h"
#include "pin_mux.h"

#if defined(BOARD_DBG_FAULT_SINK_ENABLE) && (BOARD_DBG_FAULT_SINK_ENABLE != 0)
#include "board_debug.h"
#include "board_dbg_logger.h"
#include "fwk_fault_handlers_common.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/************************************************************************************
 * Private memory declarations
 ************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions and macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

#if defined(BOARD_DBG_FAULT_SINK_ENABLE) && (BOARD_DBG_FAULT_SINK_ENABLE != 0)
/* Fault-handler output sink: forwards the framework fault-report text to the
 * shared debug log engine so it lands on the same port the coredump uses. Runs
 * in fault (HardFault) context: synchronous, no allocation, no blocking. */
static int board_dbg_fault_output(const char *buf, uint16_t len)
{
    return BOARD_DbgLoggerLogImmediate((const uint8_t *)buf, len);
}
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void BOARD_InitSWO(void)
{
    BOARD_InitPinSWO();
}

void BOARD_DeInitSWO(void)
{
    BOARD_UnInitPinSWO();
}

#if defined(BOARD_DBG_FAULT_SINK_ENABLE) && (BOARD_DBG_FAULT_SINK_ENABLE != 0)
void BOARD_DbgFaultSinkInit(void)
{
    /* Route the framework fault-handler diagnostic text through the debug log
     * engine so it shares the port used by the coredump backend. This is
     * independent of whether the coredump feature is enabled. The log engine
     * must already have been initialized (BOARD_DbgLoggerInit) at this point. */
    FWK_FaultHandlerSetOutput(board_dbg_fault_output);
}
#endif


/*******************************************************************************
 * EOF
 ******************************************************************************/
