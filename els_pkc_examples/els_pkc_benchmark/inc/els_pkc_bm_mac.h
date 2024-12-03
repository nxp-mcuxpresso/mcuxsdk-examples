/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_BM_MAC_H_
#define _ELS_PKC_BM_MAC_H_
#include "els_pkc_benchmark_utils.h"
#include <mcuxClMac.h>      /* Interface to the entire mcuxClMac component */
#include <mcuxClMacModes.h> /* Interface to the entire mcuxClMacModes component */
#include <mcuxClHmac.h>     /* Interface to the entire mcuxClHmac component */
#include <mcuxClAes.h>      /* Interface to AES-related definitions and types */
#include <mcuxClHash.h>
#include <mcuxClHashModes.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Run all MAC tests.
 */
void run_tests_mac(void);

/*!
 * @brief Execute CMAC algorithm.
 *
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param data_from String "RAM" or "FLASH".
 * @param a_result Struct for the algorithm result.
 * @param key_size Size of the key in bytes.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_cmac(uint32_t block_amount,
               char *data_from,
               algorithm_result *a_result,
               const uint16_t key_size,
               const bool cache_enable);

/*!
 * @brief Execute HMAC algorithm.
 *
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param data_from String "RAM" or "FLASH".
 * @param a_result Struct for the algorithm result.
 * @param sha_type Specifies if SHA-256 or SHA-512.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_hmac(uint32_t block_amount,
               char *data_from,
               algorithm_result *a_result,
               const uint16_t sha_type,
               const bool cache_enable);

/*!
 * @brief Performance test for CMAC.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param key_size Size of the key in bytes.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 */
void test_cmac(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable);

/*!
 * @brief Performance test for HMAC.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param sha_type Specifies if SHA-256 or SHA-512.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 */
void test_hmac(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t sha_type, const bool cache_enable);

#endif /* _ELS_PKC_BM_MAC_H_ */
