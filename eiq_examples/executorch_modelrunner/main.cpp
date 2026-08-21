/*
 * Copyright 2024,2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board_init.h"
#include "executorch_engine.h"
#include "command_handler.h"
#include "serial_protocol.h"
#include "protocol.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "timer.h"
#include <executorch/backends/nxp/runtime/NeutronDriver.h>
#include <executorch/runtime/platform/runtime.h>

extern "C" void BOARD_Init(void);

// Single receive buffer for ALL commands. 1 MB so LOAD_MODEL (.pte, up to ~1 MB)
// fits. NOTE: engine keeps its OWN copy (pte_buffer) because a later LOAD_INPUT
// will overwrite this payload_buffer — the model bytes must survive across
// commands.
static uint8_t payload_buffer[1024 * 1024] __attribute__((aligned(32)));

int main(void) {
    BOARD_Init();
    TIMER_Init();  // start SysTick so Engine::Run() can measure inference time
    // Enable the DWT cycle counter used by Engine::Run() to time execute():
    // a free-running CPU cycle counter, exact even while IRQs are masked.
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    // Bring the NPU online before any model load: load_method() registers the
    // Neutron delegate, which requires the NPU to be initialized first.
    neutronInit();
    // Print the "ready" banner ONCE at startup, before any frame traffic. The
    // PC side drains these text bytes before sending the first frame;
    // recv_frame's SYNC-hunt also discards non-SYNC bytes. After this point we
    // MUST NEVER call PRINTF/GETCHAR — the protocol LPUART0 is shared with the
    // debug console and text bytes would corrupt binary framing.
    PRINTF("\r\nExecuTorch Modelrunner Ready\r\n");
    executorch::runtime::runtime_init();
    Engine engine;
    for (;;) {
        uint8_t cmd = 0, err = kErrOk;
        size_t plen = 0;
        if (recv_frame(&cmd, payload_buffer, &plen, sizeof(payload_buffer), &err)) {
            handle_command(cmd, payload_buffer, plen, engine);
        } else if (err == kErrCrc) {
            uint8_t code = kErrCrc;
            send_frame(kStError, &code, 1);
        }
    }
    return 0;
}
