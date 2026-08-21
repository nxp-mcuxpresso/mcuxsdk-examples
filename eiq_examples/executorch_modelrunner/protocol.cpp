#include "protocol.h"

// CRC-16/CCITT-FALSE: init=0xFFFF, poly=0x1021, no final xor, MSB-first.
// Check value for ASCII "123456789" is 0x29B1. Verified via host Python.
uint16_t crc16_ccitt(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint16_t)data[i] << 8;
        for (int b = 0; b < 8; ++b) {
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : (crc << 1);
        }
    }
    return crc;
}
