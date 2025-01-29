/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "els_pkc_benchmark_utils.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/** Ms counter */
static volatile uint64_t s_MsCount = 0U;

/** Timeout value for polling */
uint64_t g_Timeout = 0U;

/** Value for poll alarm, either 0 or 1 */
volatile uint8_t g_BenchmarkTimingAlarmed = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
void benchmark_set_alarm(uint64_t seconds) {
  g_BenchmarkTimingAlarmed = 0U;
  g_Timeout =
      benchmark_timing_hardclock() + (seconds * CLOCK_GetCoreSysClkFreq());
}

void benchmark_poll_alarm(void) {
  if (benchmark_timing_hardclock() > g_Timeout) {
    g_BenchmarkTimingAlarmed = 1U;
  }
}

/*!
 * @brief Milliseconds counter since last POR/reset.
 * Function taken from existing mbedtls benchmark.
 */
void SysTick_Handler(void) { s_MsCount++; }

uint64_t benchmark_timing_hardclock(void) {
  uint64_t curr_ms_count;
  uint32_t curr_tick;
  uint64_t load_tick;

  do {
    curr_ms_count = s_MsCount;
    curr_tick = SysTick->VAL;
  } while (curr_ms_count != s_MsCount);

  load_tick = CLOCK_GetCoreSysClkFreq() / 1000U;
  return (((uint64_t)curr_ms_count) * load_tick) + load_tick - curr_tick;
}

bool cl_key_init_and_load(mcuxClSession_Handle_t session,
                          mcuxClKey_Handle_t key, mcuxClKey_Type_t type,
                          uint8_t *data, uint32_t key_data_length,
                          mcuxClEls_KeyProp_t *key_properties, uint32_t *dst,
                          uint8_t key_loading_option) {
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
      result, token,
      mcuxClKey_init(
          /* mcuxClSession_Handle_t session        */ session,
          /* mcuxClKey_Handle_t key                */ key,
          /* mcuxClKey_Type_t type                 */ type,
          /* uint8_t * pKeyData                    */ data,
          /* uint32_t keyDataLength                */ key_data_length));

  if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != token) ||
      (MCUXCLKEY_STATUS_OK != result)) {
    PRINTF("[Error] Key init failed\r\n");
    return BM_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  /* Set the key properties. */
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
      result, token, mcuxClKey_setKeyproperties(key, key_properties));

  if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_setKeyproperties) != token) ||
      (MCUXCLKEY_STATUS_OK != result)) {
    PRINTF("[Error] Key load set key props failed\r\n");
    return BM_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  if (key_loading_option == 0U) {
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClKey_loadMemory(
            /* mcuxClSession_Handle_t pSession:  */ session,
            /* mcuxClKey_Handle_t key:           */ key,
            /* uint32_t * dstData:               */ dst));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_loadMemory) != token) ||
        (MCUXCLKEY_STATUS_OK != result)) {
      PRINTF("[Error] Key load memory failed\r\n");
      return BM_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
  } else {
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClKey_loadCopro(
            /* mcuxClSession_Handle_t pSession:  */ session,
            /* mcuxClKey_Handle_t key:           */ key,
            /* uint32_t dstSlot:                 */ *dst));

    if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_loadCopro) != token) ||
        (MCUXCLKEY_STATUS_OK != result)) {
      PRINTF("[Error] Key load co-pro failed\r\n");
      return BM_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();
  }
  return BM_STATUS_SUCCESS;
}
