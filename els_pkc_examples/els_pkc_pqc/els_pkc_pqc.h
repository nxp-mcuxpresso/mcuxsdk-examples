/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_PQC_H_
#define _ELS_PKC_PQC_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* ELS_PKC CLib example functions return uint32_t; 0 means success. */
#define ELS_PKC_EXAMPLE_STATUS_OK 0U
/*******************************************************************************
 * API
 ******************************************************************************/
extern uint32_t mcuxClSignature_MlDsa44_KeyGenSignVerify_example(void);
extern uint32_t mcuxClSignature_MlDsa44_Verify_example(void);
extern uint32_t mcuxClSignature_MlDsa65_KeyGenSignVerify_example(void);
extern uint32_t mcuxClSignature_MlDsa65_Verify_example(void);
extern uint32_t mcuxClSignature_MlDsa87_KeyGenSignVerify_example(void);
extern uint32_t mcuxClSignature_MlDsa87_Verify_example(void);

#endif /* _ELS_PKC_PQC_H_ */
