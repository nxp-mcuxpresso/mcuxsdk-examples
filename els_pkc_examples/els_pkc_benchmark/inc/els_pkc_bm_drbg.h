/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_BM_DRBG_H_
#define _ELS_PKC_BM_DRBG_H_
#include "els_pkc_benchmark_utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Run all drbg tests.
 */
void run_tests_drbg(void);

/*!
 * @brief Performance test for CTR-DRBG.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param security_strength Constant defining either if security strength 128 or 256.
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If error in algorithm happens.
 */
void test_drbg(
    char *code_from, char *data_from, uint32_t security_strength, uint32_t block_amount, const bool cache_enable);

/*!
 * @brief Wrapper function for executing a CTR-DRBG algorithm.

 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @param security_strength Constant defining either if security strength 128 or 256.
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param a_result Struct for the algorithm result. Setting the cycles/byte and kb/s.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_ctr_drbg(char *data_from,
                   uint32_t security_strength,
                   uint32_t block_amount,
                   algorithm_result *a_result,
                   const bool cache_enable);

#endif /* _ELS_PKC_BM_DRBG_H_ */
