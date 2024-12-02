/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_BM_HASH_H_
#define _ELS_PKC_BM_HASH_H_
#include <mcuxClHash.h> /* Interface to the entire mcuxClHash component */
#include <mcuxClHashModes.h>
#include "els_pkc_benchmark_utils.h"
#include <mcuxClHashModes.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Run all hash-algorithm tests.
 */
void run_tests_hashing(void);

/*!
 * @brief Execute SHA algorithm.
 *
 * @param mode SHA mode(256, 384, 512).
 * @param hash Output array for the hash of size of respective mode.
 * @param data_from String "RAM" or "FLASH".
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param a_result Struct for the algorithm result.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_sha(mcuxClHash_Algo_t mode,
              uint8_t hash[],
              char *data_from,
              uint32_t block_amount,
              algorithm_result *a_result,
              const bool cache_enable);

/*!
 * @brief Performance test for SHA.
 *
 * @param mode SHA mode(256, 384, 512).
 * @param hash Output array for the hash of size of respective mode.
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 */
void test_sha(mcuxClHash_Algo_t mode,
              uint8_t hash[],
              char *code_from,
              char *data_from,
              uint32_t block_amount,
              const bool cache_enable);

#endif /* _ELS_PKC_BM_HASH_H_ */
