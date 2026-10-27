/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

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

// ---------------------------------------------------------------------------
// Large-model PSRAM path. Models larger than PTE_SRAM_THRESHOLD are streamed
// into the XSPI2 HyperRAM instead of the on-chip pte_buffer; the ExecuTorch
// runtime parses the .pte in place (CPU reads through the XSPI2 AHB window).
// The Neutron NPU cannot read PSRAM directly, so NPU models on this path MUST
// be converted with --fetch_constants_to_sram: at run time the NeutronBackend
// EXTERNAL_MEM support copies weight segments from PSRAM into SRAM (the
// scratchWeights allocation inside the method pool) while the NPU computes.
//
// PSRAM map: [0, kPsramModelMax) model bytes; a 1 MB receive area for all
// other command payloads (LOAD_INPUT etc.) sits behind it at kPsramPayloadBase
// followed by the CPU-only RESULT response buffer — moving both off-chip
// frees on-chip SRAM for a 384K pte_buffer and the enlarged temp pool.
// ---------------------------------------------------------------------------
constexpr uintptr_t kPsramBase        = 0x60000000;
constexpr size_t kPsramModelMax      = 16 * 1024 * 1024;
constexpr uintptr_t kPsramPayloadBase = kPsramBase + 16 * 1024 * 1024;
constexpr size_t kPsramPayloadSize   = 1 * 1024 * 1024;
constexpr uintptr_t kPsramResultBase = kPsramPayloadBase + kPsramPayloadSize;

// Models up to this size take the fast all-SRAM path (weights directly
// NPU-accessible in the NonCacheable pte_buffer). Override at build time
// (e.g. -DPTE_SRAM_THRESHOLD=65536) to force the PSRAM path for smaller
// models when validating it.
#ifndef PTE_SRAM_THRESHOLD
#define PTE_SRAM_THRESHOLD (384 * 1024)
#endif

#endif  // PROTOCOL_H_
