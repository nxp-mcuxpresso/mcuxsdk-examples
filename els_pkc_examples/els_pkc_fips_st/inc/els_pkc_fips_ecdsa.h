/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_FIPS_ECDSA_H_
#define _ELS_PKC_FIPS_ECDSA_H_

#include "els_pkc_fips_util.h"
#include <mcuxClPkc_Types.h>
#include <mcuxClEcc.h>
#include <mcuxClEcc_ECDSA_Internal.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Execute KAT for ECDSA Weier sign and verify.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_ecdsa_kat(uint64_t options, char name[]);

/*!
 * @brief Execute KAT for EdD25519 sign and verify.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_eddsa_kat(uint64_t options, char name[]);

#endif /* _ELS_PKC_FIPS_ECDSA_H_ */
