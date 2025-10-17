/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_cmu_fm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void cmu_fm_callback(uint32_t flags);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint32_t s_meteredClk  = 0U;
static volatile bool s_meteredCompleted = false;
static volatile bool s_errorOccurred    = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

void cmu_fm_callback(uint32_t flags)
{
    if (0U != (flags & (uint32_t)kCMU_FM_MeterComplete))
    {
        s_meteredClk = CMU_FM_GetMeteredClkCnt(DEMO_CMU);
        s_meteredCompleted = true;
    }
    else
    {
        s_errorOccurred = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    cmu_fm_config_t cmufmConfig;
    uint32_t freq;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("CMU_FM interrupt example.\r\n");
    PRINTF("Clock frequency to be measured: %d Hz.\r\n", CMU_FM_MONITORED_CLK_FREQ);
    /*
     *  config.refClockCount = CMU_FM_RCCR_REF_CNT_MASK;
     *  config.enableInterrupt = true;
     */
    CMU_FM_GetDefaultConfig(&cmufmConfig);
    /*  Set reference clock cycles as the minimum one for shorter metering window */
    cmufmConfig.refClockCount = CMU_FM_CalcMinRefClkCnt(CMU_FM_REF_CLK_FREQ, CMU_FM_BUS_CLK_FREQ, CMU_FM_MONITORED_CLK_FREQ);
    
    /* Init CMU_FM module*/
    CMU_FM_Init(DEMO_CMU, &cmufmConfig);
    /* Register interrupt callback*/
    CMU_FM_RegisterCallBack(DEMO_CMU, cmu_fm_callback);

    EnableIRQ(DEMO_CMU_IRQ);
    
    CMU_FM_StartFreqMetering(DEMO_CMU);
    /* Waiting for metering completed */
    while (!s_meteredCompleted)
    {
    }
    s_meteredCompleted = false;
    
    if (s_errorOccurred)
    {
        PRINTF("Error Occurred!\r\n");
        s_errorOccurred = false;
    }
    else
    {
        /* Calculate metered clock frequency */
        freq = CMU_FM_CalcMeteredClkFreq(s_meteredClk, cmufmConfig.refClockCount, CMU_FM_REF_CLK_FREQ);
        PRINTF("Metered clock frequency: %d Hz.\r\n", freq);
        
        if ((((float)freq) <= (((float)CMU_FM_MONITORED_CLK_FREQ) * (1.0f + CMU_FM_MONITORED_CLK_UP_DEVIATION))) && \
           (((float)freq) >= (((float)CMU_FM_MONITORED_CLK_FREQ) * (1.0f - CMU_FM_MONITORED_CLK_DOWN_DEVIATION)))) 
        {
            PRINTF("Deviation is within the scope.\r\n");
        }
        else
        {
            PRINTF("Error: Deviation exceeds the scope.\r\n");
        }
    }

    while (true)
    {
    }
}
