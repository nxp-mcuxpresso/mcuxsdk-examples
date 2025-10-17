/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_cmu_fc.h"

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
    cmu_fc_config_t cmufcConfig;
    bool destructiveResetEnable;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

#ifdef IS_DESTRUCTIVE_RESET
    destructiveResetEnable = true;

    if (IS_DESTRUCTIVE_RESET)
    {
        /* Clear Flag for Destructive Reset */
        CLR_DESTRUCTIVE_RESET_FLAG;
        destructiveResetEnable = false;
        PRINTF("Destructive reset occurred!\r\n");
    }
#else
    destructiveResetEnable = false;
#endif

    PRINTF("\r\n--- %s test start ---\r\n", (destructiveResetEnable) ? "Destructive reset" : "CMU FC");

    /*
     *  config->refClockCount = CMU_FC_RCCR_REF_CNT_MASK;
     *  config->interruptEnable = kCMU_FC_LowerThanLowThrAsyncInterruptEnable | \
     *                                kCMU_FC_HigherThanHighThrAsyncInterruptEnable;
     */
    CMU_FC_GetDefaultConfig(&cmufcConfig);

    /*  Set reference clock cycles as the minimum one for shorter metering window */
    cmufcConfig.refClockCount =
        CMU_FC_CalcMinRefClkCnt(CMU_FC_REF_CLK_FREQ, CMU_FC_BUS_CLK_FREQ, CMU_FC_MONITORED_CLK_FREQ);

    /* Calculate optimum high frequency reference threshold and low frequency reference threshold */
    CMU_FC_CalcOptimumThreshold(&cmufcConfig, CMU_FC_MONITORED_CLK_FREQ, CMU_FM_MONITORED_CLK_DEVIATION,
                                CMU_FC_REF_CLK_FREQ, CMU_FM_REF_CLK_DEVIATION);

    if (destructiveResetEnable)
    {
        /* Elevate low threshold to trigger lower than low frequency reference threshold event which will cause a
         * destructive reset */
        cmufcConfig.lowThresholdCnt = cmufcConfig.highThresholdCnt;
    }
    /* Initialize CMU_FC module with given config*/
    CMU_FC_Init(DEMO_CMU, &cmufcConfig);

    /* Start frequency check */
    CMU_FC_StartFreqChecking(DEMO_CMU);

    /* Wait 1 second to make sure the frequency check is complete. */
    SDK_DelayAtLeastUs(1000000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Check status */
    if (0U != (CMU_FC_GetStatusFlags(DEMO_CMU) & (uint32_t)kCMU_FC_HigherThanHighThr))
    {
        PRINTF("Frequency Higher than High Frequency Reference Threshold!\r\n");
    }
    else if (0U != (CMU_FC_GetStatusFlags(DEMO_CMU) & (uint32_t)kCMU_FC_LowerThanLowThr))
    {
        PRINTF("Frequency Lower than Low Frequency Reference Threshold!\r\n");
    }
    else
    {
        PRINTF("Monitored clock works fine!\r\n");
    }

    while (true)
    {
    }
}
