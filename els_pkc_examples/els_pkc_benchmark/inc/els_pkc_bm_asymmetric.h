/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_BM_ASYMMETRIC_H_
#define _ELS_PKC_BM_ASYMMETRIC_H_
#include "els_pkc_benchmark_utils.h"
#include <mcuxClEcc.h>
#include <mcuxClPkc_Types.h>
#include <mcuxClRandomModes.h>
#include <mcuxClRsa.h>
#include <mcuxClEcc_ECDSA_Internal.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Run all asymmetric key tests.
 */
void run_tests_asymmetric(void);

/*!
 * @brief Performance test for RSA signature.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param m_length Constant defining if SHA-256 digest or SHA-512 digest.
 */
void test_rsa_signature(char *code_from, char *data_from, uint32_t m_length);

/*!
 * @brief Function executing RSA sign using SHA-256 or SHA-512 digest.
 *
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @param m_length Constant defining if SHA-256 digest SHA-512 digest.
 * @param a_result Struct for the algorithm result. Setting sign/s.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_rsa_sign_pss_sha(char *data_from, uint32_t m_length, signature_algorithm_result *a_result);

/*!
 * @brief Function executing RSA verification using SHA-256 or SHA-512 digest.
 *
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @param m_length Constant defining if SHA-256 digest SHA-512 digest.
 * @param a_result Struct for the algorithm result. Setting verify/s.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_rsa_verify_pss_sha(char *data_from, uint32_t m_length, signature_algorithm_result *a_result);

/*!
 * @brief Performance test for ECC signature.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param m_length Constant defining if large or small input message.
 */
void test_ecc_ed25519_signature(char *code_from, char *data_from, uint32_t m_length);

/*!
 * @brief Function executing EdDSA sign on Ed25519.
 *
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @param m_length Constant defining if large or small input message.
 * @param a_result Struct for the algorithm result. Setting sign/s.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_EdDSA_generate_signature_Ed25519(char *data_from, uint32_t m_length, signature_algorithm_result *a_result);

/*!
 * @brief Function executing EdDSA verification on Ed25519.
 *
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @param m_length Constant defining if large or small input message.
 * @param a_result Struct for the algorithm result. Setting verify/s.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_EdDSA_verify_signature_Ed25519(char *data_from, uint32_t m_length, signature_algorithm_result *a_result);

/*!
 * @brief Function executing ECC sign on Weier p-256/384/521.
 *
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @param m_length Constant defining if large or small input message.
 * @param bit_length Bit length of algorithm (P and N).
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_weier_ecc_generate_signature(char *data_from, uint32_t m_length, uint32_t bit_length);

/*!
 * @brief Function executing ECC sign on Weier p-256/384/521.
 *
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @param m_length Constant defining if large or small input message.
 * @param bit_length Bit length of algorithm (P and N).
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_weier_ecc_verify_signature(char *data_from, uint32_t m_length, uint32_t bit_length);

/*!
 * @brief Performance test for Weier signature.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param m_length Constant defining if large or small input message.
 * @param bit_length Bit length of algorithm (P and N).
 */
void test_weier_signature(char *code_from, char *data_from, uint32_t m_length, uint32_t bit_length);

#endif /* _ELS_PKC_BM_ASYMMETRIC_H_ */
