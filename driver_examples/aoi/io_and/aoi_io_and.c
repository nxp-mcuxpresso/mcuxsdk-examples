/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "app.h"
#include "board.h"
#include "fsl_aoi.h"
#include "fsl_debug_console.h"
/*
 * This example demos the AOI uses two IO. The AOI_OUT= (IO0 & IO1).
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void AOI_Configuration(void);
extern void IO_Configuration(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    /* Initializes board hardware */
    BOARD_InitHardware();

    IO_Configuration();
    AOI_Configuration();

    PRINTF("aoi_io_and project.\r\n");

    while (1)
    {
    }
}

/*
 * Function description:
 * This function initializes the AOI module
 */
static void AOI_Configuration(void)
{
    aoi_event_config_t aoiEventLogicStruct;

    /* Configure the AOI event */
    aoiEventLogicStruct.PT0AC = kAOI_InputSignal; /* IO0 input */
    aoiEventLogicStruct.PT0BC = kAOI_InputSignal; /* IO1 input */
    aoiEventLogicStruct.PT0CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT0DC = kAOI_LogicOne;

    aoiEventLogicStruct.PT1AC = kAOI_LogicZero;
    aoiEventLogicStruct.PT1BC = kAOI_LogicOne;
    aoiEventLogicStruct.PT1CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT1DC = kAOI_LogicOne;

    aoiEventLogicStruct.PT2AC = kAOI_LogicZero;
    aoiEventLogicStruct.PT2BC = kAOI_LogicOne;
    aoiEventLogicStruct.PT2CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT2DC = kAOI_LogicOne;

    aoiEventLogicStruct.PT3AC = kAOI_LogicZero;
    aoiEventLogicStruct.PT3BC = kAOI_LogicOne;
    aoiEventLogicStruct.PT3CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT3DC = kAOI_LogicOne;

    /* Initializes AOI module. */
    AOI_Init(DEMO_AOI_BASEADDR);

    /* AOI_OUT = IO0 & IO1 */
    AOI_SetEventLogicConfig(DEMO_AOI_BASEADDR, kAOI_Event0, &aoiEventLogicStruct);
}
