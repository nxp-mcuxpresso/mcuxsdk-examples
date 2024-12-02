/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_FIPS_RSA_H_
#define _ELS_PKC_FIPS_RSA_H_

#include "els_pkc_fips_util.h"
#include <mcuxClEls_Cipher.h>
#include <mcuxClRsa.h>
#include <mcuxClPkc_Types.h>
#include <mcuxClRandomModes.h>
#include <mcuxClRandom.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Execute KAT for RSA sign and verify.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_rsa_kat(uint64_t options, char name[]);

#endif /* _ELS_PKC_FIPS_RSA_H_ */
