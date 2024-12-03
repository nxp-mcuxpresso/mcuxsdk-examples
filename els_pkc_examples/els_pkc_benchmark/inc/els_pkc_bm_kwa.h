/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_BM_KWA_H_
#define _ELS_PKC_BM_KWA_H_
#include "els_pkc_benchmark_utils.h"
#include <mcuxClExample_RFC3394_Helper.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Run all key-wrapping algorithm tests.
 */
void run_tests_kwa();

/*!
 * @brief Function for executing RFC-3394 wrapping function.
 *
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If error in algorithm happens.
 */
bool exec_rfc3394_wrap(char *data_from);

/*!
 * @brief Function for executing RFC-3394 unwrapping function.
 *
 * @param data_from String "RAM" or "Flash" to determine, if data should be
 * taken from RAM or Flash.
 * @retval MCUXCLEXAMPLE_STATUS_ERROR If error in algorithm happens.
 * @retval MCUXCLEXAMPLE_STATUS_OK If error in algorithm happens.
 */
bool exec_rfc3394_unwrap(char *data_from);

/*!
 * @brief Performance test for RFC-3394 wrapping function.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 */
void test_rfc3394_wrap(char *code_from, char *data_from, bool cache_enable);

/*!
 * @brief Performance test for RFC-3394 unwrapping function.
 *
 * @param code_from String "RAM" or "FLASH".
 * @param data_from String "RAM" or "FLASH".
 * @param cache_enable Specifies if run with same algorihtm was executed before or not (warm up run).
 */
void test_rfc3394_unwrap(char *code_from, char *data_from, bool cache_enable);

#endif /* _ELS_PKC_BM_KWA_H_ */
