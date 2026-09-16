/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_pcal6524.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

pcal6524_handle_t s_ioExpHandle;

status_t APP_PCAL6524_Lock(bool lock)
{
    if (lock)
    {
        DisableIRQ(BOARD_PCAL6524_INT_IRQ);
    }
    else
    {
        EnableIRQ(BOARD_PCAL6524_INT_IRQ);
    }
    return kStatus_Success;
}

void APP_InputPinCallback(uint8_t pin, bool pinState, void *userData)
{
    (void)userData;

    /* Input is pull-up, so active-LOW: pinState == false means the user is
     * currently driving the input pin LOW (see the board readme for how to
     * do this on your board). Output and LED track that level —
     * driven LOW = HIGH/ON, released = LOW/OFF. */

    bool active = !pinState;

    if (active)
    {
        (void)PCAL6524_SetPins(&s_ioExpHandle, 1U << APP_OUTPUT_PIN);
        USER_LED_ON();
    }
    else
    {
        (void)PCAL6524_ClearPins(&s_ioExpHandle, 1U << APP_OUTPUT_PIN);
        USER_LED_OFF();
    }

    PRINTF("  In pin %2u -> %s | Out pin %2u <- %s\r\n",
           (unsigned)pin, pinState ? "HIGH" : "LOW",
           (unsigned)APP_OUTPUT_PIN, active ? "HIGH" : "LOW");
}

static status_t APP_ConfigureDemoPins(void)
{
    status_t status;
    uint32_t inMask  = 1U << APP_INPUT_PIN;
    uint32_t outMask = 1U << APP_OUTPUT_PIN;

    status = PCAL6524_SetDirection(&s_ioExpHandle, inMask, kPCAL6524_Input);
    if (kStatus_Success != status) return status;
    status = PCAL6524_SetPullSelection(&s_ioExpHandle, inMask, true);
    if (kStatus_Success != status) return status;
    status = PCAL6524_SetPullEnable(&s_ioExpHandle, inMask, true);
    if (kStatus_Success != status) return status;
    status = PCAL6524_SetPinInterruptConfig(&s_ioExpHandle, APP_INPUT_PIN, kPCAL6524_IntEdgeAny);
    if (kStatus_Success != status) return status;

    status = PCAL6524_SetDirection(&s_ioExpHandle, outMask, kPCAL6524_Output);
    if (kStatus_Success != status) return status;
    (void)PCAL6524_ClearPins(&s_ioExpHandle, outMask);

    /* Drop any interrupt latched during pin setup — write-only to the
     * Interrupt Clear register, no input-port read. */
    return PCAL6524_ClearInterruptPins(&s_ioExpHandle, inMask);
}

int main(void)
{
    BOARD_InitHardware();
    BOARD_InitPCAL6524(&s_ioExpHandle);

    s_ioExpHandle.lock = APP_PCAL6524_Lock;

    PRINTF("\r\n=== I/O Expander Interrupt Demo ===\r\n\r\n");

    (void)PCAL6524_InstallPinCallback(&s_ioExpHandle, APP_INPUT_PIN, APP_InputPinCallback, NULL);

    status_t status = APP_ConfigureDemoPins();
    if (kStatus_Success != status)
    {
        PRINTF("Error: demo pin config failed (%d)\r\n", (int)status);
        while (1) {}
    }

    EnableIRQ(BOARD_PCAL6524_INT_IRQ);

    PRINTF("Input pin %d:  pull-up, any-edge interrupt (active-LOW)\r\n", APP_INPUT_PIN);
    PRINTF("Output pin %d: follows input — HIGH while input is held LOW\r\n", APP_OUTPUT_PIN);
    PRINTF("Waiting for interrupts...\r\n\r\n");

    while (1)
    {
    }
}
