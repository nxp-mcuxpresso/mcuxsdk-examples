/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_pit.h"
#include "fsl_xbara.h"

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
static void DEMO_InitXBARA(void)
{
    /* Structure of initialize XBARA. */
    xbara_control_config_t xbaraConfig;

    /* Init xbara module. */
    XBARA_Init(DEMO_XBARA_BASEADDR);

    /* Configure the XBARA signal connections. */
    XBARA_SetSignalsConnection(DEMO_XBARA_BASEADDR, DEMO_XBARA_USER_CHANNEL_INPUT, DEMO_XBARA_USER_CHANNEL_OUTPUT);
}

/*!
 * @brief Main function
 */
int main(void)
{
    pit_config_t pitConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Init hardware peripheral */
    DEMO_InitXBARA();

    /*
     * psConfig->eClockSource = kPIT_CountClockSource0;
     * psConfig->bEnableTimer = false;
     * psConfig->bEnableSlaveMode = false;
     * psConfig->ePrescaler = kPIT_PrescalerDivBy1;
     * psConfig->bEnableInterrupt = false;
     * psConfig->u32PeriodCount = 0xFFFFFFFFU;
     * psConfig->bEnableNegativeEdge = false;
     * psConfig->sPresetFilter.u16FilterSamplePeriod = 0x0U;
     * psConfig->sPresetFilter.u16FilterSampleCount = 0x0U;
     * psConfig->sPresetFilter.bFilterClock = true;
     * psConfig->sPresetFilter.eFilterPrescalerPeripheral = kPIT_PrescalerDivBy1;
     * psConfig->sSyncSource.u8StretchCount = 0x0U;
     * psConfig->sSyncSource.eSyncOutSel = kPIT_Syncout_Default;
     */

    PIT_GetDefaultConfig(&pitConfig);
    pitConfig.eClockSource = PIT_CLOCK_SOURCE;
    pitConfig.ePrescaler   = PIT_CLOCK_SOURCE_DIVIDER;
#if defined(FSL_FEATURE_PIT_32BIT_COUNTER) && FSL_FEATURE_PIT_32BIT_COUNTER
    pitConfig.u32PeriodCount = PIT_PERIOD_COUNT;
#else
    pitConfig.u16PeriodCount = PIT_PERIOD_COUNT;
#endif
    pitConfig.sSyncSource.eSyncOutSel = kPIT_Syncout_Toggle;

    /* Initialize pit module */
    PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);

    /* Start PIT timer */
    PRINTF("\r\nStarting PIT scout demo ...\r\n");
    PIT_StartTimer(DEMO_PIT_BASEADDR);

    while (true)
    {
    }
}
