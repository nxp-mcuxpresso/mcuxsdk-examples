/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_evtg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void EVTG_Config(void);
extern void IO_Config(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initializes board hardware */
    BOARD_InitHardware();

    IO_Config();
    EVTG_Config();

    PRINTF("\r\nEvtg project \r\n");

    while (1)
    {
    }
}

/*
 * Function description:
 * This function initializes the EVTG module
 */
static void EVTG_Config(void)
{
    evtg_config_t evtgConfig;

    /* Initialize EVTG module */
    EVTG_GetDefaultConfig(&evtgConfig, kEVTG_FFModeBypass);
    evtgConfig.aoi0Config.productTerm0.aInput = kEVTG_InputDirectPass;
    evtgConfig.aoi0Config.productTerm0.bInput = kEVTG_InputDirectPass;
    evtgConfig.aoi0Config.productTerm0.cInput = kEVTG_InputLogicOne;
    evtgConfig.aoi0Config.productTerm0.dInput = kEVTG_InputLogicOne;
    EVTG_Init(DEMO_EVTG, DEMO_EVTG_INDEX, &evtgConfig);
}
