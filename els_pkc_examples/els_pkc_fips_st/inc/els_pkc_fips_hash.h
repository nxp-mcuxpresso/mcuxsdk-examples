/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_FIPS_HASH_H_
#define _ELS_PKC_FIPS_HASH_H_

#include "els_pkc_fips_util.h"
#include <mcuxClHash.h>
#include <mcuxClHashModes.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Execute KAT for SHA2.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_sha_kat(uint64_t options, char name[]);

#endif /* _ELS_PKC_FIPS_HASH_H_ */
