/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "app.h"
#include "fsl_otp.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief OTP example.
 *
 * @details Just prints version of driver in ROM API.
 */

int main(void)
{
    uint32_t version;

    /* Init hardware*/
    BOARD_InitHardware();

    PRINTF("OTP Peripheral Driver Example\r\n\r\n");

    /* Get version of driver in ROM */
    version = OTP_GetDriverVersion();
    PRINTF("OTP ROM API driver version: 0x%X\r\n\r\n", version);

    while (1)
    {
    }
}
