/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_FIPS_CIPHER_H_
#define _ELS_PKC_FIPS_CIPHER_H_

#include "els_pkc_fips_util.h"
#include <mcuxClCipherModes.h>
#include <mcuxClAeadModes.h>
#include <mcuxClCipher.h>
#include <mcuxClAead.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Execute KAT for CBC encrypt and decrypt.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_cbc_kat(uint64_t options, char name[]);

/*!
 * @brief Execute KAT for ECB encrypt and decrypt.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_ecb_kat(uint64_t options, char name[]);

/*!
 * @brief Execute KAT for CCM encrypt and decrypt.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_ccm_kat(uint64_t options, char name[]);

/*!
 * @brief Execute KAT for GCM encrypt and decrypt.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_gcm_kat(uint64_t options, char name[]);

/*!
 * @brief Execute KAT for CTR encrypt and decrypt.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_ctr_kat(uint64_t options, char name[]);

#endif /* _ELS_PKC_FIPS_CIPHER_H_ */
