/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "fsl_itrc.h"

#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void ITRC_Demo_Status_Print(void);
/*******************************************************************************
 * Code
 ******************************************************************************/

void ITRC0_DriverIRQHandler(void)
{
    NVIC_DisableIRQ(ITRC0_IRQn);
    PRINTF("ITRC IRQ Reached!\r\n");

    ITRC_Demo_Status_Print();

    PRINTF("Clear ITRC IRQ and SW Event 0 STATUS\r\n\r\n");
    ITRC_ClearStatus(ITRC, ((uint32_t)kITRC_Irq | (uint32_t)APP_ITRC_IN));

    NVIC_EnableIRQ(ITRC0_IRQn);
}

void ITRC_Demo_Status_Print(void)
{
    uint32_t status_word = ITRC_GetStatus(ITRC);

    /* Mapping to input/output signals can be found in reference manual */
    PRINTF("ITRC STATUS:\r\n");
    /* Check Input Event signal 14 - mapped to SW Event0 */
    if (ITRC_STATUS_IN14_STATUS_MASK & status_word)
        PRINTF("Input event IN14 occured!\r\n");

    PRINTF("\r\n");
}

/*!
 * @brief Main function.
 */
int main(void)
{
    status_t result = kStatus_Fail;

    /* Init hardware */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("ITRC Peripheral Driver Example\r\n\r\n");

    /* Enable ITRC IRQ */
    result = ITRC_Init(ITRC);
    if (result != kStatus_Success)
    {
        PRINTF("Error while ITRC Init.\r\n");
        return 1;
    }

    /* Clear all possible pending Event/Action statuses */
    result = ITRC_ClearAllStatus(ITRC);
    if (result != kStatus_Success)
    {
        PRINTF("Error while ITRC STATUS Clear.\r\n");
        return 1;
    }

    /* Test if event or action already occured */
    if ((ITRC_GetStatus(ITRC) & (IN_0_15_EVENTS_MASK | OUT_ACTIONS_MASK)) == 0u)
    {
        PRINTF("Pass: No Event/Action triggered in STATUS after Init\r\n\r\n");
    }
    else
    {
        PRINTF("Fail: Action Triggered after Init!!\r\n\r\n");
    }

    /* Set ITRC IRQ action upon SW Event 0 */
    PRINTF("Enable ITRC IRQ Action response to SW Event 0\r\n\r\n");
    ITRC_SetActionToEvent(ITRC, kITRC_Irq, APP_ITRC_IN, kITRC_Unlock, kITRC_Enable);
    if (result != kStatus_Success)
    {
        PRINTF("Error seting ITRC.\r\n");
        return 1;
    }

    /* Trigger SW Event 0 */
    PRINTF("Trigger SW Event 0\r\n\r\n");
    ITRC_SetSWEvent0(ITRC);

    /* Wait a few tics for IRQ */
    __NOP();
    __NOP();

    /* Clear all possible pending Event/Action statuses */
    result = ITRC_ClearAllStatus(ITRC);
    if (result != kStatus_Success)
    {
        PRINTF("Error while ITRC STATUS Clear.\r\n");
        return 1;
    }

    /* Disable ITRC IRQ action upon SW Event 0 */
    PRINTF("Disable ITRC IRQ Action response to SW Event 0\r\n\r\n");
    ITRC_SetActionToEvent(ITRC, kITRC_Irq, APP_ITRC_IN, kITRC_Unlock, kITRC_Disable);
    if (result != kStatus_Success)
    {
        PRINTF("Error seting ITRC.\r\n");
        return 1;
    }

    /* Trigger SW Event 0 when action is disabled */
    PRINTF("Trigger SW Event 0\r\n\r\n");
    ITRC_SetSWEvent0(ITRC);

    /* Wait a few tics for IRQ */
    __NOP();
    __NOP();

    /* Test if event occured after disabling */
    if ((ITRC_GetStatus(ITRC) & (OUT_ACTIONS_MASK)) == 0u)
    {
        PRINTF("Pass: No Action triggered\r\n\r\n");
    }
    else
    {
        PRINTF("Error: Action triggered by Event even if not selected!!\r\n\r\n");
    }

    /* Deinit ITRC by disable IRQ */
    ITRC_Deinit(ITRC);

    PRINTF("End of example\r\n");
    /* End of example */
    while (1)
    {
    }
}
