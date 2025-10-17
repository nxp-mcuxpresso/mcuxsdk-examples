/*
 * Copyright 2017-2020 NXP
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_qtmr.h"
#include "fsl_xbar.h"

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
 * @brief Main function
 */
int main(void)
{
    qtmr_config_t qtmrConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    XBAR_Init(kXBAR_DSC1);
    BLK_CTRL_WAKEUPMIX->XBAR_DIR_CTRL1 |= BLK_CTRL_WAKEUPMIX_XBAR_DIR_CTRL1_IOMUXC_XBAR_DIR_SEL_8(1);
    XBAR_SetSignalsConnection(kXBAR1_InputQtimer1Timer1, kXBAR1_OutputIomuxXbarOut08);

    PRINTF("\r\n****Output PWM example.****\n");
    PRINTF("\r\n*********Make sure to connect an oscilloscope.*********\n");
    PRINTF("\r\n****A 50% duty cycle PWM wave is observed on an oscilloscope.****\n");

    /*
     * qtmrConfig.debugMode = kQTMR_RunNormalInDebug;
     * qtmrConfig.enableExternalForce = false;
     * qtmrConfig.enableMasterMode = false;
     * qtmrConfig.faultFilterCount = 0;
     * qtmrConfig.faultFilterPeriod = 0;
     * qtmrConfig.primarySource = kQTMR_ClockDivide_2;
     * qtmrConfig.secondarySource = kQTMR_Counter0InputPin;
     */
    QTMR_GetDefaultConfig(&qtmrConfig);

    /* Initial the output channel. */
    qtmrConfig.primarySource = QTMR_PRIMARY_SOURCE;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, &qtmrConfig);

    /* Generate a 50Khz PWM signal with 50% dutycycle */
    QTMR_SetupPwm(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, QTMR_PWM_OUTPUT_FREQUENCY, QTMR_DUTYCYCLE_PERCENT, false,
                  QTMR_SOURCE_CLOCK);

    /* Start the counter */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_PWM_CHANNEL, kQTMR_PriSrcRiseEdge);

    while (1)
    {
    }
}
