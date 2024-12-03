/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_mscan.h"
#include "board.h"

#include "freemaster.h"
#include "freemaster_mscan_dsc.h"

#include "freemaster_example.h"

////////////////////////////////////////////////////////////////////////////////
// Local macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Note: All global variables accessed by FreeMASTER are defined in a shared
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void init_freemaster_can(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    /* Board initialization */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    CLOCK_EnableClock(kCLOCK_MSCAN);

    /* FreeMASTER communication layer initialization */
    init_freemaster_can();

    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    while (1)
    {
        /* FreeMASTER example increments several variables periodically,
           use the FreeMASTER PC Host tool to visualize the variables */
        FMSTR_Example_Poll();
    }
}

/*!
 * @brief CAN Module initialization
 */
static void init_freemaster_can(void)
{
    mscan_config_t mscanConfig;

    /* Get MSCAN module default Configuration. */
    /*
     * mscanConfig.baudRate = 500000U;
     * mscanConfig.enableTimer = false;
     * mscanConfig.enableWakeup = false;
     * mscanConfig.clkSrc = kMSCAN_ClkSrcOsc;
     * mscanConfig.enableLoopBack = false;
     * mscanConfig.enableListen = false;
     * mscanConfig.busoffrecMode = kMSCAN_BusoffrecAuto;
     * mscanConfig.filterConfig.filterMode = kMSCAN_Filter32Bit;
     */
    MSCAN_GetDefaultConfig(&mscanConfig, CLOCK_GetFreq(kCLOCK_BusClk));

    mscanConfig.baudRate = 500000U;

    /* Acceptance filter configuration. */
    mscanConfig.filterConfig.u32IDAR0 = 0;
    mscanConfig.filterConfig.u32IDAR1 = 0;
    /* All bits are don't care. */
    mscanConfig.filterConfig.u32IDMR0 = 0xffffffff;
    mscanConfig.filterConfig.u32IDMR1 = 0xffffffff;

    /* Initialize MSCAN module. */
    MSCAN_Init(CAN0, &mscanConfig);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_CanSetBaseAddress(CAN0);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable FlexCAN interrupts. */
    EnableIRQWithPriority(CAN_ERROR_IRQn, 1);
    EnableIRQWithPriority(CAN_TX_WARN_IRQn, 1);
    EnableIRQWithPriority(CAN_RX_WARN_IRQn, 1);
    EnableIRQWithPriority(CAN_WAKEUP_IRQn, 1);
    SetIRQBasePriority(0);
    EnableGlobalIRQ(1);
#endif
}

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR

/* All interrupt handlers call FreeMASTER Interrupt routine handler */
#pragma interrupt alignsp saveall
void ivINT_CAN_ERROR(void);
void ivINT_CAN_ERROR(void)
{
    FMSTR_CanIsr();
}
#pragma interrupt off

#pragma interrupt alignsp saveall
void ivINT_CAN_TX_WARN(void);
void ivINT_CAN_TX_WARN(void)
{
    FMSTR_CanIsr();
}
#pragma interrupt off

#pragma interrupt alignsp saveall
void ivINT_CAN_RX_WARN(void);
void ivINT_CAN_RX_WARN(void)
{
    FMSTR_CanIsr();
}
#pragma interrupt off

#pragma interrupt alignsp saveall
void ivINT_CAN_WAKEUP(void);
void ivINT_CAN_WAKEUP(void)
{
    FMSTR_CanIsr();
}
#pragma interrupt off

#endif
