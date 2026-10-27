/*
 * Copyright 2026 NXP
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
#include "sys_info/sys_info.h"
#include "timer.h"
#include <executorch/backends/nxp/runtime/NeutronDriver.h>
#include <executorch/runtime/platform/runtime.h>

extern "C" void BOARD_Init(void);

// All command payloads (LOAD_INPUT etc.) are received into the 1 MB PSRAM
// window at kPsramPayloadBase — keeping this buffer off-chip freed SRAM for
// the enlarged NonCacheable work pools (see protocol.h). Large LOAD_MODEL
// frames above PTE_SRAM_THRESHOLD bypass this area entirely and stream
// straight to kPsramBase. The engine copies bytes it must keep (pte_buffer /
// input_buffer), so overwriting the receive window on later commands is fine.

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
    // System info text goes out before any frame traffic, same as the banner
    // below: the PC side drains these bytes before sending the first frame.
    SYS_INFO_PrintAll();
    PRINTF("\r\nExecuTorch Modelrunner Ready\r\n");
    executorch::runtime::runtime_init();
    Engine engine;
    uint8_t *payload = reinterpret_cast<uint8_t *>(kPsramPayloadBase);
    for (;;) {
        uint8_t cmd = 0, err = kErrOk;
        size_t plen = 0;
        uint8_t *p = payload;
        if (recv_frame(&cmd, &p, &plen, kPsramPayloadSize, &err)) {
            handle_command(cmd, p, plen, engine);
        } else if (err == kErrCrc || err == kErrModelTooBig) {
            send_frame(kStError, &err, 1);
        }
    }
    return 0;
}
