/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app_core0.h"

#include "fsl_mailbox.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define START_EVENT 1234
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_msg = 1;
/* For the flow control */
volatile bool g_secondary_core_started = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* When the secondary core writes to the primary core mailbox register it causes call of this irq handler,
   in which the received value is read, incremented and sent again to the secondary core */
void MAILBOX_IRQHandler()
{
    if (!g_secondary_core_started)
    {
        if (START_EVENT == MAILBOX_GetValue(MAILBOX, PRIMARY_CORE_MAILBOX_CPU_ID))
        {
            g_secondary_core_started = true;
        }
        MAILBOX_ClearValueBits(MAILBOX, PRIMARY_CORE_MAILBOX_CPU_ID, 0xffffffff);
    }
    else
    {
        g_msg = MAILBOX_GetValue(MAILBOX, PRIMARY_CORE_MAILBOX_CPU_ID);
        MAILBOX_ClearValueBits(MAILBOX, PRIMARY_CORE_MAILBOX_CPU_ID, 0xffffffff);
        PRINTF("Read value from the primary core mailbox register: %d\r\n", g_msg);
        g_msg++;
        PRINTF("Write to the secondary core mailbox register: %d\r\n", g_msg);
        MAILBOX_SetValue(MAILBOX, SECONDARY_CORE_MAILBOX_CPU_ID, g_msg);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware.*/
    BOARD_InitHardware();

    PRINTF("Mailbox interrupt example\r\n");

    /* Init Mailbox */
    MAILBOX_Init(MAILBOX);

    /* Enable mailbox interrupt */
    NVIC_EnableIRQ(MAILBOX_IRQn);

#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* Calculate size of the image */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    PRINTF("Copy CORE1 image to address: 0x%x, size: %d\r\n", CORE1_BOOT_ADDRESS, core1_image_size);

    /* Copy application from FLASH to RAM */
    memcpy((void *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);
#endif

    /* Start the secondary core */
    start_secondary_core(CORE1_BOOT_ADDRESS);

    /* Wait for start and initialization of secondary core */
    while (!g_secondary_core_started)
        ;

    PRINTF("Write to the secondary core mailbox register: %d\r\n", g_msg);
    /* Write g_msg to the secondary core mailbox register - it causes interrupt on the secondary core */
    MAILBOX_SetValue(MAILBOX, SECONDARY_CORE_MAILBOX_CPU_ID, g_msg);

    while (1)
    {
        __WFI();
    }
}
