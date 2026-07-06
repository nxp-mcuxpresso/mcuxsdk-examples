/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/
#include <stdint.h>
#include <stddef.h>

#include "board_dbg_logger.h"
#include "board_dbg_logger_port.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* First-call-wins init guard: 0 = not initialized, 1 = initialized. */
static uint8_t s_initialized;

/*******************************************************************************
 * Code
 ******************************************************************************/
int BOARD_DbgLoggerInit(void)
{
    int ret = 0;

    /* First-call-wins: a repeated init is a no-op. */
    if (s_initialized == 0U)
    {
        ret = BOARD_DbgLoggerPortInit();
        if (ret >= 0)
        {
            s_initialized = 1U;
        }
    }

    return ret;
}

int BOARD_DbgLoggerLogImmediate(const uint8_t *buf, uint16_t len)
{
    int ret = -1;

    if ((buf != NULL) && (len != 0U))
    {
        ret = BOARD_DbgLoggerPortWrite(buf, len);
    }

    return ret;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
