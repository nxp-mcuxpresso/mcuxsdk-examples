/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_FIPS_DRBG_H_
#define _ELS_PKC_FIPS_DRBG_H_

#include "els_pkc_fips_util.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Execute KAT for DRBG.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_drbg_kat(uint64_t options, char name[]);

#endif /* _ELS_PKC_FIPS_DRBG_H_ */
