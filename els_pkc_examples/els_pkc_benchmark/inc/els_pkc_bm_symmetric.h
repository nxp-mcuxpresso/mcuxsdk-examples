/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_BM_SYMMETRIC_H_
#define _ELS_PKC_BM_SYMMETRIC_H_
#include "els_pkc_benchmark_utils.h"
#include <mcuxClAeadModes.h>
#include <mcuxClAes.h>         /* Interface to AES-related definitions and types */
#include <mcuxClCipher.h>      /* Interface to the entire mcuxClCipher component */
#include <mcuxClCipherModes.h> /* Interface to the entire mcuxClCipherModes component */
#include <mcuxClAead.h>        /* Interface to the entire mcuxClAead component */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Run all symmetric key tests.
 */
void run_tests_symmetric(void);

/*!
 * @brief Wrapper function for executing a block cipher encryption algorithm.
 *
 * @param mode Block cipher mode.
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @param a_result Struct for the algorithm result.
 * @param key_size Size of the key in bytes.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If error in algorithm happens.
 */
bool exec_cl_cipher_mode(mcuxClCipher_Mode_t mode,
                         uint32_t block_amount,
                         char *data_from,
                         algorithm_result *a_result,
                         const uint16_t key_size,
                         const bool cache_enable);

/*!
 * @brief Wrapper function for executing an AEAD algorithm.
 *
 * @param mode AEAD mode.
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @param a_result Struct for the algorithm result.
 * @param key_size Size of the key in bytes.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_cl_aead_mode(mcuxClAead_Mode_t mode,
                       uint32_t block_amount,
                       char *data_from,
                       algorithm_result *a_result,
                       const uint16_t key_size,
                       const bool cache_enable);

/*!
 * @brief Performance test for CBC encryption.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param key_size Size of the key in bytes.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 */
void test_aes_cbc(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable);

/*!
 * @brief Performance test for ECB encryption.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param key_size Size of the key in bytes.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 */
void test_aes_ecb(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable);

/*!
 * @brief Performance test for CTR encryption.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param key_size Size of the key in bytes.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 */
void test_aes_ctr(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable);

/*!
 * @brief Performance test for CCM AEAD.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param key_size Size of the key in bytes.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 */
void test_aes_ccm(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable);

/*!
 * @brief Performance test for GCM AEAD.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param block_amount Constant defining either SINGLE_BLOCK or MULTIPLE_BLOCKS.
 * @param key_size Size of the key in bytes.
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 */
void test_aes_gcm(
    char *code_from, char *data_from, uint32_t block_amount, const uint16_t key_size, const bool cache_enable);

#endif /* _ELS_PKC_BM_SYMMETRIC_H_ */
