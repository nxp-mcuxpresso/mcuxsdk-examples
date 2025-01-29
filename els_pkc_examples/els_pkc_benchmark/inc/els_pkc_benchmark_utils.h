/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_BENCHMARK_UTILS_H_
#define _ELS_PKC_BENCHMARK_UTILS_H_

#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include <board.h>
#include <mcuxClAes_Constants.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClEls.h>
#include <mcuxClEls_Rng.h>
#include <mcuxClKey.h>
#include <mcuxClMemory.h>
#include <mcuxClRandom.h>
#include <mcuxClRandomModes.h>
#include <mcuxClSession.h>
#include <mcuxCsslFlowProtection.h>
#include <platform_specific_headers.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SINGLE_BLOCK 1U
#define MULTIPLE_BLOCKS 1024U
#define BM_STATUS_ERROR false
#define BM_STATUS_SUCCESS true

/** Macro function adapted from existing mbedtls benchmark */
#define COMPUTE_CYCLES(CODE, AMOUNT, ITERATION_AMOUNT)                         \
  ({                                                                           \
    uint32_t jj;                                                               \
    uint64_t tsc1;                                                             \
    tsc1 = benchmark_timing_hardclock();                                       \
    for (jj = 0U; jj < ITERATION_AMOUNT; ++jj) {                               \
      CODE;                                                                    \
    }                                                                          \
    (double)(((double)(benchmark_timing_hardclock() - tsc1)) /                 \
             (ITERATION_AMOUNT * AMOUNT));                                     \
  })

/** Macro function adapted from existing mbedtls benchmark */
#define KB_S(CODE, AMOUNT)                                                     \
  ({                                                                           \
    uint64_t ii;                                                               \
    uint64_t tsc1;                                                             \
    uint64_t tsc2;                                                             \
    benchmark_set_alarm(0x1U);                                                 \
    tsc1 = benchmark_timing_hardclock();                                       \
    for (ii = 1U; !g_BenchmarkTimingAlarmed; ++ii) {                           \
      CODE;                                                                    \
      benchmark_poll_alarm();                                                  \
    }                                                                          \
    tsc2 = benchmark_timing_hardclock();                                       \
    (double)(((ii * AMOUNT) / 1024U) /                                         \
             (((double)(tsc2 - tsc1)) / CLOCK_GetCoreSysClkFreq()));           \
  })

/** Macro function adapted from existing mbedtls benchmark */
#define TIME_PUBLIC(CODE)                                                      \
  ({                                                                           \
    uint64_t ii;                                                               \
    uint64_t tsc;                                                              \
    benchmark_set_alarm(2U);                                                   \
    tsc = benchmark_timing_hardclock();                                        \
    for (ii = 1U; !g_BenchmarkTimingAlarmed; ii++) {                           \
      CODE;                                                                    \
      benchmark_poll_alarm();                                                  \
    }                                                                          \
    (double)(((double)ii) / ((benchmark_timing_hardclock() - tsc) /            \
                             CLOCK_GetCoreSysClkFreq()));                      \
  })

#define PRINT_RESULT(result)                                                   \
  do {                                                                         \
    PRINTF("\tCODE: %s", result.code);                                         \
    PRINTF("\tDATA: %s", result.data);                                         \
    PRINTF("\tKB/S: %6.2f", result.kbPerS);                                    \
    PRINTF("\tCYCLES/BLOCK: %6.2f", result.cyclesPerBlock);                    \
    PRINTF("\tCYCLES/BYTE: %6.2f", result.cyclesPerByte);                      \
    PRINTF("\tWARMED UP: %s", result.cached);                                  \
    PRINTF("\tEXECUTION: %s", result.execution);                               \
    PRINTF("\r\n");                                                            \
  } while (0)

#define PRINT_SIGNATURE_RESULT(result)                                         \
  do {                                                                         \
    PRINTF("\tCODE: %s", result.code);                                         \
    PRINTF("\tDATA: %s", result.data);                                         \
    PRINTF("\tSIGN/S: %6.2f", result.signPerS);                                \
    PRINTF("\t\tVERIFY/S: %6.2f", result.verifyPerS);                          \
    PRINTF("\tEXECUTION: %s", result.execution);                               \
    PRINTF("\r\n");                                                            \
  } while (0)

#define PRINT_KEY_GEN_RESULT(result)                                           \
  do {                                                                         \
    PRINTF("\tCODE: %s", result.code);                                         \
    PRINTF("\tDATA: %s", result.data);                                         \
    PRINTF("\tKEY-GEN/S: %6.2f", result.signPerS);                             \
    PRINTF("\r\n");                                                            \
  } while (0)

#define PRINT_KDF_RESULT(result)                                               \
  do {                                                                         \
    PRINTF("\tCODE: %s", result.code);                                         \
    PRINTF("\tDATA: %s", result.data);                                         \
    PRINTF("\tKB/S: %6.2f", result.kbPerS);                                    \
    PRINTF("\tCYCLES/BYTE: %6.2f", result.cyclesPerByte);                      \
    PRINTF("\tCYCLES/KEY: %6.2f", result.cyclesPerBlock);                      \
    PRINTF("\tEXECUTION: %s", result.execution);                               \
    PRINTF("\r\n");                                                            \
  } while (0);

#define PRINT_DH_RESULT(result)                                                \
  do {                                                                         \
    PRINTF("\tCODE: %s", result.code);                                         \
    PRINTF("\tDATA: %s", result.data);                                         \
    PRINTF("\tPOINT-MULT./S: %6.2f", result.signPerS);                         \
    PRINTF("\tSHARED-SECRET CALC./S: %6.2f", result.verifyPerS);               \
    PRINTF("\r\n");                                                            \
  } while (0)

#define ALLOCATE_AND_INITIALIZE_SESSION(session, cpu_wa_length, pkc_wa_length) \
  uint32_t cpu_wa_buffer[(cpu_wa_length)];                                     \
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                            \
      si_status, token,                                                        \
      mcuxClSession_init((session), cpu_wa_buffer, (cpu_wa_length),            \
                         (uint32_t *)PKC_RAM_ADDR, (pkc_wa_length)));          \
  if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_init) != token) ||           \
      (MCUXCLSESSION_STATUS_OK != si_status)) {                                \
    return BM_STATUS_ERROR;                                                    \
  }                                                                            \
  MCUX_CSSL_FP_FUNCTION_CALL_END()

/*!
 * @brief Value for poll alarm, either 0 or 1.
 *
 * Note: Global variable defined in header file, because needed in
 * the macro functions.
 */
extern volatile uint8_t g_BenchmarkTimingAlarmed;

/** Struct representing the algorithm result, which gets printed ultimately */
typedef struct _algorithm_result {
  char execution[16U];
  char code[6U];
  char data[6U];
  double cyclesPerBlock;
  double cyclesPerByte;
  double kbPerS;
  char cached[4U];
} algorithm_result;

/** Struct representing the signature algorithm result, which gets printed
 * ultimately */
typedef struct _signature_algorithm_result {
  char execution[25U];
  char code[6U];
  char data[6U];
  double signPerS;
  double verifyPerS;
} signature_algorithm_result;

/*!
 * @brief Measure current TSC for performance testing.
 * Function taken from existing mbedtls benchmark.
 *
 * @retval The current TSC.
 */
uint64_t benchmark_timing_hardclock(void);

/*!
 * @brief Check if timeout exceeded, if yes set g_BenchmarkTimingAlarmed to 1.
 * Function taken from existing mbedtls benchmark.
 */
void benchmark_poll_alarm();

/*!
 * @brief Set timeout value to specific amount of second.
 * Function taken from existing mbedtls benchmark.
 *
 * @param seconds Amount of seconds.
 */
void benchmark_set_alarm(uint64_t seconds);

/*!
 * @brief Initialize key and load.
 *
 * @param session Session handle to initialize.
 * @param key Key handle to init. and load.
 * @param data Key data.
 * @param key_data_length Key data length.
 * @param key_properties Properties of the key.
 * @param dst Destination key slot if internal key used.
 * @param key_loading_option If external or internal key.
 * @retval STATUS_SUCCESS If initialization was successful.
 * @retval STATUS_ERROR_GENERIC If initialization was unsuccessful.
 */
bool cl_key_init_and_load(mcuxClSession_Handle_t session,
                          mcuxClKey_Handle_t key, mcuxClKey_Type_t type,
                          uint8_t *data, uint32_t key_data_length,
                          mcuxClEls_KeyProp_t *key_properties, uint32_t *dst,
                          uint8_t key_loading_option);

#endif /* _ELS_PKC_BENCHMARK_UTILS_H_ */
