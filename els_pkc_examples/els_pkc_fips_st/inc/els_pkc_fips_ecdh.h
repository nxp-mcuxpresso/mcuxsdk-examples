/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_FIPS_ECDH_H_
#define _ELS_PKC_FIPS_ECDH_H_

#include "els_pkc_fips_util.h"
#include <mcuxClEcc.h>
#include <mcuxClPkc_Types.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Execute KAT for ECDH.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_ecdh_kat(uint64_t options, char name[]);

#endif /* _ELS_PKC_FIPS_ECDH_H_ */
