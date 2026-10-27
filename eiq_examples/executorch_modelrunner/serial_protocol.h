/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SERIAL_PROTOCOL_H_
#define SERIAL_PROTOCOL_H_

#include <cstddef>
#include <cstdint>
#include "protocol.h"

// Low-level raw byte I/O over the LPUART used for the protocol.
// MUST be binary-transparent (no \r\n translation) — do NOT use PRINTF/GETCHAR.
void serial_write_blocking(const uint8_t *data, size_t len);
size_t serial_read_blocking(uint8_t *buf, size_t len);   // blocks until len bytes read

// Frame-level helpers. send_frame computes CRC, emits SYNC+CMD+LEN+PAYLOAD+CRC.
// recv_frame reads one frame; on CRC mismatch returns false and sets *err=kErrCrc.
// On protocol/framing error returns false and sets *err=kErrProtocol.
//
// Payload routing: LOAD_MODEL frames larger than PTE_SRAM_THRESHOLD are
// streamed chunk-by-chunk into the PSRAM at kPsramBase (no single RAM buffer
// can hold them); on success *payload is set to the PSRAM address. Every other
// payload is read into the caller-provided buffer (max_payload bounds it) and
// *payload keeps pointing at that buffer.
bool recv_frame(uint8_t *cmd, uint8_t **payload, size_t *payload_len,
                size_t max_payload, uint8_t *err);
void send_frame(uint8_t status, const uint8_t *payload, size_t payload_len);

#endif
