/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdlib.h>
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_kbi.h"
/*
 * This case tests the necessary API to enable the ENET transmission. It would call the functions sequentially in
 * polling mode.
 *
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_keypress = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void KBI0_IRQHandler(void)
{
    if (KBI_IsInterruptRequestDetected(EXAMPLE_KBI))
    {
        /* Disable interrupts. */
        KBI_DisableInterrupts(EXAMPLE_KBI);
        /* Clear status. */
        KBI_ClearInterruptFlag(EXAMPLE_KBI);
        g_keypress = true;
    }
    SDK_ISR_EXIT_BARRIER;
}

int main(void)
{
    kbi_config_t kbiConfig;

    BOARD_InitHardware();

    PRINTF("\r\n KBI Driver Example Start.\r\n");

    kbiConfig.mode        = kKBI_EdgesDetect;
    kbiConfig.pinsEnabled = 1 << EXAMPLE_KBI_PINS;
    kbiConfig.pinsEdge    = 1 << EXAMPLE_KBI_PINS; /* Raising edge.*/
    KBI_Init(EXAMPLE_KBI, &kbiConfig);

    while (1)
    {
        /* Toggle to generate the edge which will trigger the KBI interrupt. */
        GPIO_PortToggle(EXAMPLE_KBI_SIGNAL_INPUT_REF_GPIO, 1u << EXAMPLE_KBI_SIGNAL_INPUT_REF_GPIO_INDEX);

        if (g_keypress)
        {
            PRINTF("\r\n The KBI keyboard interrupt has happened!\r\n");
            break;
        }
    }

    /* Deinitialize the module. */
    KBI_Deinit(EXAMPLE_KBI);

    PRINTF("\r\n KBI Driver Example End.\r\n");

    while (1)
    {
    }
}
