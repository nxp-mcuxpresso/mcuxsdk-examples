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
#include "fsl_flexcan.h"
#include "board.h"

#include "freemaster.h"
#include "freemaster_flexcan_dsc.h"

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

    CLOCK_EnableClock(kCLOCK_FLEXCAN);

    /* FreeMASTER communication layer initialization */
    init_freemaster_can();

    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    while(1)
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
    flexcan_config_t flexcanConfig;

    /* Init FlexCAN module. */
    FLEXCAN_GetDefaultConfig(&flexcanConfig, CLOCK_GetFreq(kCLOCK_BusClk));

    flexcanConfig.eClkSrc = kFLEXCAN_ClkSrc1;
    flexcanConfig.u32BaudRateBps = 500000U;

    FLEXCAN_Init(CAN0, &flexcanConfig);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_CanSetBaseAddress(CAN0);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable FlexCAN interrupts. */
    EnableIRQWithPriority(CAN_MB_OR_IRQn, 1);
    EnableIRQWithPriority(CAN_BUS_OFF_IRQn, 1);
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
void ivINT_CAN_MB_OR(void);
void ivINT_CAN_MB_OR(void)
{
    FMSTR_CanIsr();
}
#pragma interrupt off

#pragma interrupt alignsp saveall
void ivINT_CAN_BUS_OFF(void);
void ivINT_CAN_BUS_OFF(void)
{
    FMSTR_CanIsr();
}
#pragma interrupt off

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
