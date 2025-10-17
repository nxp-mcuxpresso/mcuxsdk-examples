/*
 * Copyright 2022-2023, 2025 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_erm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void APP_ecc_enable(void);
extern void APP_ecc_disable(void);

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
    uint32_t injectvalue;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    ERM_Init(APP_ERM);

    /* Initialize the memory to a known value so that the correct corresponding ECC codeword is stored. */
    BOARD_MemoryInit();

    PRINTF("\r\nERM error recording example.\r\n");

    /* Inject single bit data ecc error. */
    injectvalue = *ramAddr;
    injectvalue ^= (1 << APP_ERM_RAM_INJECT_BIT);
    
    /* Disable ECC to prevent it from updating of RAM data checksum */
    APP_ecc_disable();
    
    /* Store data with one flipped bit to RAM.*/
    *ramAddr = injectvalue;

    /* Enable ERM interrupt. */
    ERM_EnableInterrupts(APP_ERM, APP_ERM_MEMORY_CHANNEL, kERM_SingleCorrectionIntEnable);
    /* Enable ERM IRQ */
    EnableIRQ(APP_ERM_IRQ);
    
    APP_ecc_enable();
    /* Read data with ECC enabled. ECC will correct flipped bit and trigger interrupt. */
    temp = *ramAddr;

    /* Check if RAM data exuals to the original value */
    if (APP_ERM_MAGIC_NUMBER == temp)
    {
        PRINTF("\r\nFlipped bit was corrected.\r\n");
    }
    else
    {
        errorFlag = true;
        PRINTF("\r\nFlipped bit was not corrected.\r\n");
    }

    /* Wait for interrupt */
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
