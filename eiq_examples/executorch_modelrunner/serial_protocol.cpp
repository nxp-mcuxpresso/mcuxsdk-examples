#include "serial_protocol.h"
#include "fsl_lpuart.h"

// The protocol reuses the SAME LPUART the debug console uses. The LPUART
// hardware itself is binary-transparent; the \r\n translation is added by the
// SDK DbgConsole software layer (PRINTF/GETCHAR). By driving the LPUART
// directly via LPUART_*Blocking we bypass DbgConsole and stay binary-safe.
//
// For MIMXRT798S cm33_core0, BOARD_DEBUG_UART_BASEADDR == LPUART0 (LP_FLEXCOMM0,
// pins N4/N4 FC0_RXD/TXD configured in pin_mux.c). The clock/pins are already
// set up by BOARD_InitDebugConsole() called from hardware_init.c, so we do NOT
// re-initialize here.
//
// LPUART0 expands to ((LPUART_Type *)LPUART0_BASE) -> already a typed pointer.
#define PROTOCOL_LPUART_BASE  (LPUART0)

void serial_write_blocking(const uint8_t *data, size_t len) {
    LPUART_WriteBlocking(PROTOCOL_LPUART_BASE, data, len);
}

size_t serial_read_blocking(uint8_t *buf, size_t len) {
    // LPUART_ReadBlocking returns kStatus_Success once all len bytes arrived;
    // it blocks inside the driver until then. We mirror the requested length.
    LPUART_ReadBlocking(PROTOCOL_LPUART_BASE, buf, len);
    return len;
}

// CRC range contract (CRITICAL for the PC side):
//   crc16_ccitt is computed over CMD + LEN(4B) + PAYLOAD only.
//   The two SYNC bytes (0x55,0xAA) and the trailing 2 CRC bytes are NOT in the
//   CRC input. The CRC is appended little-endian. The PC-side encoder MUST use
//   the exact same range, or frames will be rejected with kErrCrc.
void send_frame(uint8_t status, const uint8_t *payload, size_t payload_len) {
    uint8_t header[kHeaderLen] = {kSync0, kSync1, status};
    header[3] = (uint8_t)(payload_len & 0xFF);
    header[4] = (uint8_t)((payload_len >> 8) & 0xFF);
    header[5] = (uint8_t)((payload_len >> 16) & 0xFF);
    header[6] = (uint8_t)((payload_len >> 24) & 0xFF);

    // CRC over CMD+LEN+PAYLOAD. header[2..6] holds CMD+LEN, but payload is a
    // separate buffer — a single crc16_ccitt() call over the header would read
    // past it into unrelated stack memory and never cover the real payload. So
    // seed with CMD+LEN, then fold in each payload byte with the same
    // CCITT-FALSE polynomial.
    uint16_t crc = crc16_ccitt(header + 2, kHeaderLen - 2);  // CMD+LEN
    for (size_t i = 0; i < payload_len; ++i) {
        crc ^= (uint16_t)payload[i] << 8;
        for (int b = 0; b < 8; ++b) {
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : (crc << 1);
        }
    }

    serial_write_blocking(header, kHeaderLen);
    if (payload_len) {
        serial_write_blocking(payload, payload_len);
    }
    uint8_t crc_buf[2] = {(uint8_t)(crc & 0xFF), (uint8_t)((crc >> 8) & 0xFF)};
    serial_write_blocking(crc_buf, kCrcLen);
}

// Read exactly one byte (helper that returns the byte value).
static uint8_t read1(void) {
    uint8_t b = 0;
    serial_read_blocking(&b, 1);
    return b;
}

bool recv_frame(uint8_t *cmd, uint8_t *payload, size_t *payload_len,
                size_t max_payload, uint8_t *err) {
    // 1) Hunt for SYNC: read bytes until kSync0,kSync1 in sequence.
    //    Any non-SYNC byte is discarded (re-sync).
    bool have_sync0 = false;
    for (;;) {
        uint8_t b = read1();
        if (!have_sync0) {
            if (b == kSync0) {
                have_sync0 = true;
            }
            continue;
        }
        // have_sync0 == true
        if (b == kSync1) {
            break;  // full sync acquired
        }
        // b == kSync0 again: stay in sync0 state (allow back-to-back 0x55).
        // any other byte: resync.
        have_sync0 = (b == kSync0);
    }

    // 2) Read CMD (1B) and LEN (4B LE).
    uint8_t local_cmd = read1();
    uint32_t len = 0;
    uint8_t len_bytes[4];
    for (int i = 0; i < 4; ++i) {
        len_bytes[i] = read1();
    }
    len = (uint32_t)len_bytes[0]
        | ((uint32_t)len_bytes[1] << 8)
        | ((uint32_t)len_bytes[2] << 16)
        | ((uint32_t)len_bytes[3] << 24);

    if (len > max_payload) {
        // Frame too large for caller's buffer: framing/protocol error.
        *err = kErrProtocol;
        return false;
    }

    // 3) Read PAYLOAD (LEN bytes).
    if (len) {
        serial_read_blocking(payload, len);
    }

    // 4) Read CRC (2B LE) and verify over CMD+LEN+PAYLOAD.
    uint8_t crc_lo = read1();
    uint8_t crc_hi = read1();
    uint16_t recv_crc = (uint16_t)crc_lo | ((uint16_t)crc_hi << 8);

    // Recompute CRC over {CMD, LEN[0..3], PAYLOAD} in a single pass.
    // crc16_ccitt is stateless, so build a small prefix buffer and chain the
    // payload via the running algorithm by calling crc16_ccitt on the prefix
    // then continuing over the payload bytes manually with the same polynomial.
    uint8_t crc_input[1 + 4] = {local_cmd, len_bytes[0], len_bytes[1],
                                len_bytes[2], len_bytes[3]};
    uint16_t calc_crc = crc16_ccitt(crc_input, sizeof(crc_input));
    // Continue the CRC over the payload bytes (same init-free continuation:
    // feed each payload byte into the running crc).
    for (size_t i = 0; i < len; ++i) {
        calc_crc ^= (uint16_t)payload[i] << 8;
        for (int b = 0; b < 8; ++b) {
            calc_crc = (calc_crc & 0x8000) ? (calc_crc << 1) ^ 0x1021 : (calc_crc << 1);
        }
    }

    if (recv_crc != calc_crc) {
        *err = kErrCrc;
        return false;
    }

    // 5) Success.
    *cmd = local_cmd;
    *payload_len = len;
    return true;
}
