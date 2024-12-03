/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_BM_DH_H_
#define _ELS_PKC_BM_DH_H_
#include "els_pkc_benchmark_utils.h"
#include <mcuxClEcc.h>
#include <mcuxClPkc_Types.h>
#include <mcuxClRandomModes.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Run all ECDH tests.
 */
void run_tests_ecdh();

/*!
 * @brief Performance test for ECDH Weierstrass curves.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param bit_length Bit length of algorithm (P and N).
 */
void test_ecdh(char *code_from, char *data_from, uint32_t bit_length);

/*!
 * @brief Function executing ECDH on Weier p-256/384/521.
 *
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @param bit_length Bit length of algorithm (P and N).
 * @param a_result Struct for the algorithm result. Setting point multiplication per second
 * and secret key calculation per second.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_ecdh_point_mult(char *data_from, uint32_t bit_length, signature_algorithm_result *a_result);

#endif /* _ELS_PKC_BM_DH_H_ */
