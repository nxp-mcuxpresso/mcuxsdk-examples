/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_BM_KEY_GEN_H_
#define _ELS_PKC_BM_KEY_GEN_H_
#include "els_pkc_benchmark_utils.h"
#include <mcuxClEcc.h>
#include <mcuxClPkc_Types.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Run all key generation tests.
 */
void run_tests_key_gen();

/*!
 * @brief Performance test for Weier key generation.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param bit_length Bit length of algorithm (P and N).
 */
void test_ecc_weier_key_gen(char *code_from, char *data_from, uint32_t bit_length);

/*!
 * @brief Function executing ECC key generation on Weier p-256/384/521.
 *
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @param bit_length Bit length of algorithm (P and N).
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If algorithm succeeds.
 */
bool exec_ecc_weier_key_gen(char *data_from, uint32_t bit_length);

#endif /* _ELS_PKC_BM_KEY_GEN_H_ */
