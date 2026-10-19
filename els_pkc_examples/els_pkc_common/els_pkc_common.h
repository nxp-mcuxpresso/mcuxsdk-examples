/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_COMMON_H_
#define _ELS_PKC_COMMON_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXIT_CODE_ERROR 0x00U
#define EXIT_CODE_OK    0x01U
/* ELS_PKC CLib example functions return uint32_t; 0 means success. */
#define ELS_PKC_EXAMPLE_STATUS_OK 0U
/*******************************************************************************
 * API
 ******************************************************************************/
extern uint32_t mcuxClEls_Rng_Prng_Get_Random_example(void);
extern uint32_t mcuxClEls_Common_Get_Info_example(void);
extern uint32_t mcuxCsslFlowProtection_example(void);
extern uint32_t mcuxCsslMemory_Compare_example(void);
extern uint32_t mcuxCsslMemory_Copy_example(void);
extern uint32_t mcuxClKey_example(void);

#endif /* _ELS_PKC_COMMON_H_ */
