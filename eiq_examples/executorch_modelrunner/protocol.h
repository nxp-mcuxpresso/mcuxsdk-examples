#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <cstdint>
#include <cstddef>

// Frame: [SYNC0][SYNC1][CMD][LEN:4B LE][PAYLOAD:LEN][CRC16:2B LE]
constexpr uint8_t kSync0 = 0x55;
constexpr uint8_t kSync1 = 0xAA;
constexpr size_t kHeaderLen = 2 + 1 + 4;        // SYNC+CMD+LEN
constexpr size_t kCrcLen   = 2;

// PC -> board commands
constexpr uint8_t kCmdLoadModel = 0x01;
constexpr uint8_t kCmdLoadInput = 0x02;
constexpr uint8_t kCmdRun       = 0x03;
constexpr uint8_t kCmdGetInfo   = 0x04;

// board -> PC status
constexpr uint8_t kStOk     = 0x81;
constexpr uint8_t kStInfo   = 0x82;
constexpr uint8_t kStResult = 0x83;
constexpr uint8_t kStError  = 0xC0;

// error codes (payload of kStError = err_code:1B + msg:varlen)
constexpr uint8_t kErrOk          = 0x00;
constexpr uint8_t kErrCrc         = 0x01;
constexpr uint8_t kErrProtocol    = 0x02;
constexpr uint8_t kErrModelTooBig = 0x03;
constexpr uint8_t kErrModelLoad   = 0x04;
constexpr uint8_t kErrInputSize   = 0x05;
constexpr uint8_t kErrNpuInit     = 0x06;
constexpr uint8_t kErrExecute     = 0x07;
constexpr uint8_t kErrNoModel     = 0x08;
// Fine-grained LoadModel stage codes (debugging aid; the PC-side tooling may
// print them raw). 0x10..0x15 map to the failure points in
// Engine::LoadModel in call order.
constexpr uint8_t kErrDbgProgramLoad  = 0x10;  // Program::load (flatbuffer parse)
constexpr uint8_t kErrDbgMethodName   = 0x11;  // get_method_name
constexpr uint8_t kErrDbgMethodMeta   = 0x12;  // method_meta
constexpr uint8_t kErrDbgPlannedBuf   = 0x13;  // planned buffer allocation
constexpr uint8_t kErrDbgLoadMethod   = 0x14;  // load_method (op registry etc.)

// CRC-16/CCITT-FALSE over CMD+LEN+PAYLOAD (poly=0x1021, init=0xFFFF)
uint16_t crc16_ccitt(const uint8_t *data, size_t len);

#endif  // PROTOCOL_H_
