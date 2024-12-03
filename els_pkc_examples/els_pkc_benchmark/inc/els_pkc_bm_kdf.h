/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_BM_KDF_H_
#define _ELS_PKC_BM_KDF_H_
#include "els_pkc_benchmark_utils.h"
#include <mcuxClEls_Kdf.h>
#include <mcuxClAes.h>
#include <internal/mcuxClKey_Internal.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Run all kdf-algorithm tests.
 */
void run_tests_kdf();

/*!
 * @brief Performance test for HKDF-RFC5869.
 *
 * @param code_from Flag for determining if code run from flash or ram.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
void test_hkdf_rfc5869(char *code_from);

/*!
 * @brief Performance test for HKDF-SP80056C.
 *
 * @param code_from Flag for determining if code run from flash or ram.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
void test_hkdf_sp80056c(char *code_from);

/*!
 * @brief Performance test for CKDF-SP800108.
 *
 * @param code_from Flag for determining if code run from flash or ram.
 * @param key_size Size of the derived key in bits.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
void test_ckdf_Sp800108(char *code_from, uint16_t key_size);

/*!
 * @brief Execution of CKDF-SP800108.
 *
 * @param a_result Struct for the algorithm result.
 * @param key_size Size of the derived key in bits.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_ckdf_Sp800108(algorithm_result *a_result, uint16_t key_size);

/*!
 * @brief Execution of HKDF-SP80056C.
 *
 * @param a_result Struct for the algorithm result.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_hkdf_sp80056c(algorithm_result *a_result);

/*!
 * @brief Execution of HKDF-RFC5869.
 *
 * @param a_result Struct for the algorithm result.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_hkdf_rfc5869(algorithm_result *a_result);

#endif /* _ELS_PKC_BM_KDF_H_ */
