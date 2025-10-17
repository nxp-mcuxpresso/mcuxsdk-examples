/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_stm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void stm_match_callback(uint32_t flags);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* channel 0 compare match value */
static uint32_t s_ch0CompareValue;
/* channel 1 compare match value */
static uint32_t s_ch1CompareValue;
/*******************************************************************************
 * Code
 ******************************************************************************/

void stm_match_callback(uint32_t flags)
{
    volatile uint32_t cnt = STM_GetTimerCount(STM);
    if(flags & kSTM_Channel_Match_Msk_0)
    {
        STM_SetCompare(STM, STM_CHANNEL_0, s_ch0CompareValue + cnt);
        PRINTF("Channel 0 match.\r\n");
    }
    if(flags & kSTM_Channel_Match_Msk_1)
    {
        STM_SetCompare(STM, STM_CHANNEL_1, s_ch1CompareValue + cnt);
        PRINTF("Channel 1 match.\r\n");
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    stm_config_t config;

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("STM match example.\r\n");
    /*
     * config.enableRunInDebug = true;
     * config.enableIRQ = true;
     * config.prescale = 0U;
    */
    STM_GetDefaultConfig(&config);
    STM_Init(STM, &config);
    STM_RegisterCallBack(STM, stm_match_callback);

    /* channel 0 period is set to 1s */
    s_ch0CompareValue = STM_CLK_FREQ/((uint32_t)config.prescale + 1U);
    STM_SetCompare(STM, STM_CHANNEL_0, s_ch0CompareValue);

    /* channel 1 period is set to 0.5s */
    s_ch1CompareValue = (STM_CLK_FREQ >> 1)/((uint32_t)config.prescale + 1U);
    STM_SetCompare(STM, STM_CHANNEL_1, s_ch1CompareValue);

    STM_StartTimer(STM);

    while (true)
    {
    }
}
