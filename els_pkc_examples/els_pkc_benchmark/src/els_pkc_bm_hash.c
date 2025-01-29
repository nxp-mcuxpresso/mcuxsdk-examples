/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "els_pkc_bm_hash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HASH(session, mode, block_amount, hash, data_from_ram)                 \
  uint32_t hash_output_size = 0U;                                              \
  do {                                                                         \
    if ((data_from_ram)) {                                                     \
      MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                        \
          result, token2,                                                      \
          mcuxClHash_compute(                                                  \
              (session), (mode),                                               \
              (block_amount) == SINGLE_BLOCK ? s_DataSingleBlock : s_Data,     \
              (block_amount) == SINGLE_BLOCK ? sizeof(s_DataSingleBlock)       \
                                             : sizeof(s_Data),                 \
              (hash), &hash_output_size));                                     \
      if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHash_compute) != token2) ||      \
          (MCUXCLHASH_STATUS_OK != result)) {                                  \
        PRINTF("[Error] Hashing with SHA failed\r\n");                         \
        return BM_STATUS_ERROR;                                                \
      }                                                                        \
      MCUX_CSSL_FP_FUNCTION_CALL_END();                                        \
    } else {                                                                   \
      MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(                                        \
          result, token2,                                                      \
          mcuxClHash_compute(                                                  \
              (session), (mode),                                               \
              (block_amount) == SINGLE_BLOCK ? s_DataFlashSingleBlock          \
                                             : s_DataFlash,                    \
              (block_amount) == SINGLE_BLOCK ? sizeof(s_DataFlashSingleBlock)  \
                                             : sizeof(s_DataFlash),            \
              (hash), &hash_output_size));                                     \
      if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHash_compute) != token2) ||      \
          (MCUXCLHASH_STATUS_OK != result)) {                                  \
        PRINTF("[Error] Hashing with SHA failed\r\n");                         \
        return BM_STATUS_ERROR;                                                \
      }                                                                        \
      MCUX_CSSL_FP_FUNCTION_CALL_END();                                        \
    }                                                                          \
  } while (0)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Test vectors for SHA */
static uint8_t s_Data[128U * MULTIPLE_BLOCKS];
static uint8_t s_DataSingleBlock[128U];

/* Test vectors for SHA stored in flash */
static const uint8_t s_DataFlash[128U * MULTIPLE_BLOCKS];
static const uint8_t s_DataFlashSingleBlock[128U];

/*******************************************************************************
 * Code
 ******************************************************************************/
bool exec_sha(mcuxClHash_Algo_t mode, uint8_t hash[], char *data_from,
              uint32_t block_amount, algorithm_result *a_result,
              const bool cache_enable) {
  /**************************************************************************/
  /* Preparation                                                            */
  /**************************************************************************/
  bool data_from_ram = !strcmp(data_from, "RAM");

  /* Initialize session */
  mcuxClSession_Descriptor_t session_desc;
  mcuxClSession_Handle_t session = &session_desc;

  /* Allocate and initialize session */
  ALLOCATE_AND_INITIALIZE_SESSION(session,
                                  MCUXCLHASH_MAX_CPU_WA_BUFFER_SIZE +
                                      MCUXCLRANDOMMODES_NCINIT_WACPU_SIZE,
                                  0U);

  /* Initialize the PRNG */
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(prngInit_result, prngInit_token,
                                   mcuxClRandom_ncInit(session));
  if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClRandom_ncInit) != prngInit_token) ||
      (MCUXCLRANDOM_STATUS_OK != prngInit_result)) {
    return BM_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  /**************************************************************************/
  /* Hash computation                                                       */
  /**************************************************************************/
  const uint32_t iteration_amount = cache_enable ? 1024U : 1U;
  a_result->cyclesPerBlock =
      COMPUTE_CYCLES(HASH(session, mode, block_amount, hash, data_from_ram),
                     block_amount, iteration_amount);
  a_result->cyclesPerByte = a_result->cyclesPerBlock / 128U;
  a_result->kbPerS =
      KB_S(HASH(session, mode, block_amount, hash, data_from_ram),
           block_amount * 128U);

  /**************************************************************************/
  /* Session clean-up                                                       */
  /**************************************************************************/
  /* Clean-up and destroy the session. */
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
      sessionCleanup_result, sessionCleanup_token,
      mcuxClSession_cleanup(
          /* mcuxClSession_Handle_t           pSession: */ session));
  if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_cleanup) !=
          sessionCleanup_token ||
      MCUXCLSESSION_STATUS_OK != sessionCleanup_result) {
    PRINTF("[Error] Session cleaning failed\r\n");
    return BM_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
      sessionDestroy_result, sessionDestroy_token,
      mcuxClSession_destroy(
          /* mcuxClSession_Handle_t           pSession: */ session));
  if (MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_destroy) !=
          sessionDestroy_token ||
      MCUXCLSESSION_STATUS_OK != sessionDestroy_result) {
    PRINTF("[Error] Session destroy failed\r\n");
    return BM_STATUS_ERROR;
  }
  MCUX_CSSL_FP_FUNCTION_CALL_END();

  return BM_STATUS_SUCCESS;
}

void test_sha(mcuxClHash_Algo_t mode, uint8_t hash[], char *code_from,
              char *data_from, uint32_t block_amount, const bool cache_enable) {
  if (!strcmp(code_from, "RAM") && !strcmp(data_from, "FLASH"))
    return;
  algorithm_result a_result;
  strcpy(a_result.execution,
         block_amount == SINGLE_BLOCK ? "SINGLE BLOCK" : "MULTIPLE BLOCKS");
  strcpy(a_result.code, code_from);
  strcpy(a_result.data, data_from);
  exec_sha(mode, hash, data_from, block_amount, &a_result, cache_enable);
  strcpy(a_result.cached, cache_enable ? "YES" : "NO");

  PRINT_RESULT(a_result);
}

void run_tests_hashing(void) {
  char code_from[6U];
  strcpy(code_from, BOARD_IS_XIP() ? "FLASH" : "RAM");

  uint8_t hash32[32U];
  PRINTF("SHA-256:\r\n");
  test_sha(mcuxClHash_Algorithm_Sha256, hash32, code_from, "FLASH",
           MULTIPLE_BLOCKS, false);
  test_sha(mcuxClHash_Algorithm_Sha256, hash32, code_from, "FLASH",
           MULTIPLE_BLOCKS, true);
  test_sha(mcuxClHash_Algorithm_Sha256, hash32, code_from, "RAM",
           MULTIPLE_BLOCKS, false);
  test_sha(mcuxClHash_Algorithm_Sha256, hash32, code_from, "RAM",
           MULTIPLE_BLOCKS, true);
  test_sha(mcuxClHash_Algorithm_Sha256, hash32, code_from, "FLASH",
           SINGLE_BLOCK, true);
  test_sha(mcuxClHash_Algorithm_Sha256, hash32, code_from, "RAM", SINGLE_BLOCK,
           true);
  PRINTF("\r\n");

  uint8_t hash48[48U];
  PRINTF("SHA-384:\r\n");
  test_sha(mcuxClHash_Algorithm_Sha384, hash48, code_from, "FLASH",
           MULTIPLE_BLOCKS, false);
  test_sha(mcuxClHash_Algorithm_Sha384, hash48, code_from, "FLASH",
           MULTIPLE_BLOCKS, true);
  test_sha(mcuxClHash_Algorithm_Sha384, hash48, code_from, "RAM",
           MULTIPLE_BLOCKS, false);
  test_sha(mcuxClHash_Algorithm_Sha384, hash48, code_from, "RAM",
           MULTIPLE_BLOCKS, true);
  test_sha(mcuxClHash_Algorithm_Sha384, hash48, code_from, "FLASH",
           SINGLE_BLOCK, true);
  test_sha(mcuxClHash_Algorithm_Sha384, hash48, code_from, "RAM", SINGLE_BLOCK,
           true);
  PRINTF("\r\n");

  uint8_t hash64[64U];
  PRINTF("SHA-512:\r\n");
  test_sha(mcuxClHash_Algorithm_Sha512, hash64, code_from, "FLASH",
           MULTIPLE_BLOCKS, false);
  test_sha(mcuxClHash_Algorithm_Sha512, hash64, code_from, "FLASH",
           MULTIPLE_BLOCKS, true);
  test_sha(mcuxClHash_Algorithm_Sha512, hash64, code_from, "RAM",
           MULTIPLE_BLOCKS, false);
  test_sha(mcuxClHash_Algorithm_Sha512, hash64, code_from, "RAM",
           MULTIPLE_BLOCKS, true);
  test_sha(mcuxClHash_Algorithm_Sha512, hash64, code_from, "FLASH",
           SINGLE_BLOCK, true);
  test_sha(mcuxClHash_Algorithm_Sha512, hash64, code_from, "RAM", SINGLE_BLOCK,
           true);
  PRINTF("\r\n");
}
