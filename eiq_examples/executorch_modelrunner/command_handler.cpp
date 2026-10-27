/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Board-side command handler / state machine.
//
// Receives a single decoded command frame from main() and dispatches it to the
// Engine, emitting exactly the required response frame. The state
// machine guards against out-of-order use (e.g. RUN before any input loaded).
//
// IMPORTANT: this module MUST NOT use PRINTF/GETCHAR. The protocol LPUART is
// shared with the debug console; injecting text bytes corrupts binary framing.
// All board->PC output goes through send_frame.

#include "command_handler.h"

#include <cstring>

#include "protocol.h"
#include "serial_protocol.h"

// Response buffer caps. INFO serializes input/output tensor specs (small);
// RESULT carries timing + output tensor bytes. The mlperf anomaly_detection
// model reconstructs its full [98,640] float32 input (250,880 B + 23 B of
// framing), so the RESULT buffer must hold ~251 KB. The buffer is pure CPU
// traffic (serialize outputs, then the UART driver reads it back), so it
// lives in the PSRAM window behind the payload receive area - keeping it
// off-chip frees on-chip SRAM for a larger pte_buffer (see protocol.h).
constexpr size_t kInfoBufMax   = 256;
constexpr size_t kResultBufMax = 260 * 1024;
static uint8_t *g_result_buf = reinterpret_cast<uint8_t *>(kPsramResultBase);

// Board-side state machine.
enum class State : uint8_t {
    kIdle         = 0,  // no model loaded yet
    kModelLoaded  = 1,  // model loaded, no input
    kInputReady   = 2,  // model + input loaded, ready to run
};

// File-scope state survives across commands (single-engine, single-board design).
static State g_state = State::kIdle;

// Emit a kStError frame: payload = err_code(1B) + msg bytes (no NUL terminator).
static void send_err(uint8_t code, const char *msg) {
    uint8_t buf[1 + 48];
    buf[0] = code;
    size_t msg_len = 0;
    if (msg != nullptr) {
        msg_len = strlen(msg);
        if (msg_len > sizeof(buf) - 1) {
            msg_len = sizeof(buf) - 1;
        }
        memcpy(buf + 1, msg, msg_len);
    }
    send_frame(kStError, buf, 1 + msg_len);
}

// Debug hook called by Engine::Run on execute failure: re-emits the last
// error frame with the runtime Error enum value in the message, so the PC
// side prints e.g. "run code=35".
static int g_last_run_code = 0;
void engine_report_run_code(int code) {
    g_last_run_code = code;
}
int engine_last_run_code() {
    return g_last_run_code;
}

void handle_command(uint8_t cmd, const uint8_t *payload, size_t len, Engine &engine) {
    switch (cmd) {
        case kCmdLoadModel: {
            engine.Reset();
            uint8_t e = engine.LoadModel(payload, len);
            if (e == kErrOk) {
                g_state = State::kModelLoaded;
                send_frame(kStOk, nullptr, 0);
            } else {
                g_state = State::kIdle;
                send_err(e, "load_model");
            }
            break;
        }

        case kCmdGetInfo: {
            uint8_t buf[kInfoBufMax];
            size_t n = engine.GetInfo(buf, sizeof(buf));
            send_frame(kStInfo, buf, n);
            break;
        }

        case kCmdLoadInput: {
            if (g_state < State::kModelLoaded) {
                send_err(kErrNoModel, "no model");
                break;
            }
            uint8_t e = engine.LoadInput(payload, len);
            if (e == kErrOk) {
                g_state = State::kInputReady;
                send_frame(kStOk, nullptr, 0);
            } else {
                send_err(e, "load_input");
            }
            break;
        }

        case kCmdRun: {
            if (g_state < State::kInputReady) {
                send_err(kErrNoModel, "no input");
                break;
            }
            uint8_t err = kErrOk;
            // NOTE: g_result_buf is a pointer (PSRAM) — its sizeof() is 4;
            // the capacity is kResultBufMax.
            size_t n = engine.Run(g_result_buf, kResultBufMax, &err);
            if (err == kErrOk) {
                send_frame(kStResult, g_result_buf, n);
                // After a successful run the input is consumed; back to
                // MODEL_LOADED so the host can LOAD_INPUT + RUN again.
                g_state = State::kModelLoaded;
            } else {
                // "run code=N" with the runtime Error enum value (decimal).
                char msg[24];
                int code = engine_last_run_code();
                size_t p = 0;
                const char *prefix = "run code=";
                while (*prefix) msg[p++] = *prefix++;
                if (code < 0) { msg[p++] = '-'; code = -code; }
                char digits[8];
                int nd = 0;
                do { digits[nd++] = '0' + (code % 10); code /= 10; } while (code);
                while (nd--) msg[p++] = digits[nd];
                msg[p] = '\0';
                send_err(err, msg);
            }
            break;
        }

        default:
            send_err(kErrProtocol, "unknown cmd");
            break;
    }
}
