/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_erm.h"
#include "fsl_eim.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_ram_single_error = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_ERM_SINGLE_BIT_ERROR_IRQ_HANDLER(void)
{
    uint32_t status;

    status = ERM_GetInterruptStatus(APP_ERM, APP_ERM_MEMORY_CHANNEL);
    ERM_ClearInterruptStatus(APP_ERM, APP_ERM_MEMORY_CHANNEL, status);

    if (status & kERM_SingleBitCorrectionIntFlag)
    {
        s_ram_single_error = true;
    }

    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    bool errorFlag    = false;
    uint32_t *ramAddr = (uint32_t *)APP_ERM_RAM_CHECK_ADDR;
    uint32_t temp;
    uint32_t errorAddress;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    EIM_Init(APP_EIM);
    ERM_Init(APP_ERM);

    /* Initialize the memory to a known value so that the correct corresponding ECC codeword is stored. */
    BOARD_MemoryInit();

    PRINTF("\r\nERM error recording example.\r\n");

    /* Inject databit mask. */
    EIM_InjectDataBitError(APP_EIM, APP_EIM_MEMORY_CHANNEL, 0x01UL);
    /* Enable error injection channel. */
    EIM_EnableErrorInjectionChannels(APP_EIM, APP_EIM_MEMORY_CHANNEL_EN);
    /* Enable global error injection. */
    EIM_EnableGlobalErrorInjection(APP_EIM, true);

    /* Enable ERM interrupt. */
    ERM_EnableInterrupts(APP_ERM, APP_ERM_MEMORY_CHANNEL, kERM_SingleCorrectionIntEnable);
    /* Enable ERM IRQ */
    EnableIRQ(APP_ERM_IRQ);

    /* Read original data to trigger ERM single correction interrupt. */
    temp = *ramAddr;

    /* Check ram data */
    if (APP_ERM_MAGIC_NUMBER == temp)
    {
        PRINTF("\r\nOriginal ram data correct.\r\n");
    }
    else
    {
        errorFlag = true;
        PRINTF("\r\nOriginal ram data incorrect.\r\n");
    }

    /* Wait for error detected */
    while (s_ram_single_error == false)
    {
    }

    s_ram_single_error = false;

#if (defined(APP_ERM_MEMORY_RECODE_OFFSET) && APP_ERM_MEMORY_RECODE_OFFSET)
    errorAddress = ERM_GetMemoryErrorAddr(APP_ERM, APP_ERM_MEMORY_CHANNEL) + APP_ERM_RAM_START_ADDR;
#else
    errorAddress = ERM_GetMemoryErrorAddr(APP_ERM, APP_ERM_MEMORY_CHANNEL);
#endif /* (defined(APP_ERM_MEMORY_RECODE_OFFSET) && APP_ERM_MEMORY_RECODE_OFFSET) */
    if (APP_ERM_RAM_CHECK_ADDR == errorAddress)
    {
        PRINTF("\r\nERM error recording address is 0x%x.\r\n", errorAddress);
    }
    else
    {
        errorFlag = true;
        PRINTF("\r\nError recording address is wrong.\r\n");
    }

    if (errorFlag)
    {
        PRINTF("\r\nERM error recording example finished with error.\r\n");
    }
    else
    {
        PRINTF("\r\nERM error recording example finished successfully.\r\n");
    }

    while (1)
    {
    }
}
