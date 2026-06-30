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

static pcal6524_handle_t s_handle;
volatile bool g_pcal6524IntFlag = false;

void APP_InputPinCallback(uint8_t pin, bool pinState, void *userData)
{
    (void)userData;

    /* Input is pull-up + probe-to-GND, so active-LOW: pinState == false
     * means the user is currently holding TP12 against ground. Output and
     * LED track that level — held = HIGH/ON, released = LOW/OFF. */
    bool active = !pinState;

    if (active)
    {
        (void)PCAL6524_SetPins(&s_handle, 1U << APP_OUTPUT_PIN);
        USER_LED_ON();
    }
    else
    {
        (void)PCAL6524_ClearPins(&s_handle, 1U << APP_OUTPUT_PIN);
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

    status = PCAL6524_SetDirection(&s_handle, inMask, kPCAL6524_Input);
    if (kStatus_Success != status) return status;
    status = PCAL6524_SetPullSelection(&s_handle, inMask, true);
    if (kStatus_Success != status) return status;
    status = PCAL6524_SetPullEnable(&s_handle, inMask, true);
    if (kStatus_Success != status) return status;
    status = PCAL6524_SetPinInterruptConfig(&s_handle, APP_INPUT_PIN, kPCAL6524_IntEdgeAny);
    if (kStatus_Success != status) return status;

    status = PCAL6524_SetDirection(&s_handle, outMask, kPCAL6524_Output);
    if (kStatus_Success != status) return status;
    (void)PCAL6524_ClearPins(&s_handle, outMask);

    /* Drop any interrupt latched during pin setup — write-only to the
     * Interrupt Clear register, no input-port read. */
    return PCAL6524_ClearInterruptPins(&s_handle, inMask);
}

int main(void)
{
    BOARD_InitHardware();
    BOARD_InitPCAL6524(&s_handle);

    s_handle.lock = APP_PCAL6524_Lock;

    (void)PCAL6524_InstallPinCallback(&s_handle, APP_INPUT_PIN, APP_InputPinCallback, NULL);

    PRINTF("\r\n=== I/O Expander Interrupt Demo ===\r\n\r\n");

    status_t status = APP_ConfigureDemoPins();
    if (kStatus_Success != status)
    {
        PRINTF("Error: demo pin config failed (%d)\r\n", (int)status);
        while (1) {}
    }

    PRINTF("Input pin %d:  pull-up, any-edge interrupt (active-LOW)\r\n", APP_INPUT_PIN);
    PRINTF("Output pin %d: follows input — HIGH while input is held LOW\r\n", APP_OUTPUT_PIN);
    PRINTF("Waiting for interrupts...\r\n\r\n");

    while (1)
    {
        if (g_pcal6524IntFlag)
        {
            g_pcal6524IntFlag = false;

            status = PCAL6524_InterruptHandler(&s_handle);
            if (kStatus_Success != status)
            {
                PRINTF("Error: PCAL6524_InterruptHandler failed (%d)\r\n", (int)status);
            }
        }
    }
}
