/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_FIPS_MAC_H_
#define _ELS_PKC_FIPS_MAC_H_

#include "els_pkc_fips_util.h"
#include <mcuxClHash.h>
#include <mcuxClHashModes.h>
#include <mcuxClHmac.h>
#include <mcuxClMac.h>
#include <mcuxClMacModes.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Execute KAT for HMAC.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_hmac_kat(uint64_t options, char name[]);

/*!
 * @brief Execute KAT for CMAC.
 *
 * @param options Containing which algorithm to execute.
 * @param name Containing the name of the algorithm.
 */
void execute_cmac_kat(uint64_t options, char name[]);

#endif /* _ELS_PKC_FIPS_MAC_H_ */
