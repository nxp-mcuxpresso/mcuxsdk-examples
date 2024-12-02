/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_FIPS_KEY_GEN_H_
#define _ELS_PKC_FIPS_KEY_GEN_H_

#include "els_pkc_fips_util.h"
#include <mcuxClEcc.h>
#include <mcuxClPkc_Types.h>
#include <mcuxClEcc_ECDSA_Internal.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Execute Pairwise Consistency Test for ECC key generation.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_ecc_keygen_pct(uint64_t options, char name[]);

#endif /* _ELS_PKC_FIPS_KEY_GEN_H_ */
