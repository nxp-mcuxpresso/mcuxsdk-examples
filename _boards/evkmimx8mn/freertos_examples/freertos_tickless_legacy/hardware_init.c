/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_uart.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "app.h"
#if configUSE_TICKLESS_IDLE == 2
#include "fsl_gpt.h"
#endif
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
#if configUSE_TICKLESS_IDLE == 2
    gpt_config_t gptConfig;
#endif

    /* M7 has its local cache and enabled by default,
     * need to set smart subsystems (0x28000000 ~ 0x3FFFFFFF)
     * non-cacheable before accessing this address region */
    BOARD_InitMemory();

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
#if configUSE_TICKLESS_IDLE == 2
    CLOCK_SetRootMux(kCLOCK_RootGpt1, kCLOCK_GptRootmuxSysPll1Div20); /* Set GPT1 source to SysPLL1 Div20 40MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootGpt1, 1U, 5U);                    /* Set root clock to 40MHZ / 5 = 8MHZ */
    GPT_GetDefaultConfig(&gptConfig);

    /* Initialize GPT module */
    GPT_Init(GPT1, &gptConfig);

    /* Divide GPT clock source frequency by 1 inside GPT module */
    GPT_SetClockDivider(GPT1, 1);

    /* Enable GPT Output Compare1 interrupt */
    GPT_EnableInterrupts(GPT1, kGPT_OutputCompare1InterruptEnable);

    /* Enable at the Interrupt */
    EnableIRQ(GPT1_IRQn);
#endif
}

#if configUSE_TICKLESS_IDLE == 2
/*!
 * @brief Interrupt service fuction of GPT timer.
 *
 * This function to call low power timer ISR
 */
void GPT1_IRQHandler(void)
{
    vPortGptIsr();
}

/*!
 * @brief Fuction of GPT timer.
 *
 * This function to return GPT timer base address
 */

GPT_Type *vPortGetGptBase(void)
{
    return GPT1;
}

/*!
 * @brief Fuction of GPT timer.
 *
 * This function to return GPT timer interrupt number
 */

IRQn_Type vPortGetGptIrqn(void)
{
    return GPT1_IRQn;
}
#endif
/*${function:end}*/
