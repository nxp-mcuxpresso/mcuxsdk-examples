/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_virt_wrapper.h"
#include "fsl_siul2.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define APP_PORT_PIN_INDEX ((APP_SIUL2_GPIO_PORT * 32u) + APP_SIUL2_GPIO_PIN)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_InitVirtWrapper(bool init_or_deinit);
static void APP_InitSiul2Pin(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_irqFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_SIUL2_EIRQ_IRQHandler(void)
{
    uint32_t flags = SIUL2_GetExtDmaInterruptStatusFlags(APP_SIUL2);

    SIUL2_ClearExtDmaInterruptStatusFlags(APP_SIUL2, flags);

    if ((flags & (1u << APP_SIUL2_EIRQ)) != 0u)
    {
        s_irqFlag = true;
    }
}

static void APP_InitVirtWrapper(bool init_or_deinit)
{
    virt_wrapper_slot_t slot = init_or_deinit ? APP_VIRT_WRAPPER_PDAC_SLOT : kVIRT_WRAPPER_PDAC1;
    /* Map the SIUL2 pin to slot APP_VIRT_WRAPPER_PDAC_SLOT. */
    VIRT_WRAPPER_MapPins(APP_VIRT_WRAPPER, APP_SIUL2_GPIO_PORT, 1u << APP_SIUL2_GPIO_PIN, slot);
    VIRT_WRAPPER_MapInputMux(APP_VIRT_WRAPPER, APP_SIUL2_GPIO_PIN_INPUTMUX_REG, slot);

    /* Map the SIUL2 interrupt registers to APP_VIRT_WRAPPER_PDAC_SLOT. */
    VIRT_WRAPPER_MapInterrupt(APP_VIRT_WRAPPER, slot);
}

static void APP_InitSiul2Pin(void)
{
    /*
     * The PIN has been initialized as GPIO input in function BOARD_InitEXTIRQPins,
     * only need to enable the output here.
     */
    SIUL2_SetPinOutputBuffer(APP_SIUL2, APP_PORT_PIN_INDEX, true, kPORT_MUX_AS_GPIO);

    SIUL2_SetGlitchFilterPrescaler(APP_SIUL2, 1U);

    SIUL2_PortPinWrite(APP_SIUL2, APP_SIUL2_GPIO_PORT, APP_SIUL2_GPIO_PIN, 0);

    /* Wait to make sure the input status really change. */
    while (0 != SIUL2_PortPinRead(APP_SIUL2, APP_SIUL2_GPIO_PORT, APP_SIUL2_GPIO_PIN))
    {
    }

    SIUL2_ClearExtDmaInterruptStatusFlags(APP_SIUL2, (1u << APP_SIUL2_EIRQ));

    SIUL2_EnableExtInterrupt(APP_SIUL2, APP_SIUL2_EIRQ, kSIUL2_InterruptRisingEdge, 2U);

    EnableIRQ(APP_SIUL2_EIRQ_IRQn);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("VIRT_WRAPPER SIUL2 example started\r\n");

    APP_InitVirtWrapper(true);

    APP_InitSiul2Pin();

    /*
     * The pin is configured as input and output, when toggle the output,
     * the read input will be changed accordingly. So can toggle the output
     * to trigger the interrupt on the same pin.
     */
    s_irqFlag = false;

    SIUL2_PortPinWrite(APP_SIUL2, APP_SIUL2_GPIO_PORT, APP_SIUL2_GPIO_PIN, 1);

    /* Wait to make sure the input status really change. */
    while (0 == SIUL2_PortPinRead(APP_SIUL2, APP_SIUL2_GPIO_PORT, APP_SIUL2_GPIO_PIN))
    {
    }

    APP_InitVirtWrapper(false);

    if (s_irqFlag == false)
    {
        PRINTF("Example finished failed\r\n");
    }
    else
    {
        PRINTF("Example finished passed\r\n");
    }

    while (1)
    {
    }
}