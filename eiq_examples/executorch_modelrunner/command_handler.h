#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

#include <cstddef>
#include <cstdint>
#include "executorch_engine.h"

// Handle one received command frame; emits the response frame(s) via send_frame.
// Implements the board-side state machine:
//   IDLE -> (LOAD_MODEL) -> MODEL_LOADED -> (LOAD_INPUT) -> INPUT_READY -> (RUN) -> MODEL_LOADED
void handle_command(uint8_t cmd, const uint8_t *payload, size_t len, Engine &engine);

#endif  // COMMAND_HANDLER_H_
