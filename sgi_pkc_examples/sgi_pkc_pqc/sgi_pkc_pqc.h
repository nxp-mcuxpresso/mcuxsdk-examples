/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SGI_PKC_PQC_H_
#define _SGI_PKC_PQC_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXIT_CODE_ERROR 0x00U
#define EXIT_CODE_OK    0x01U
/*******************************************************************************
 * API
 ******************************************************************************/
extern bool mcuxClMlKem_KeyGenEncAndDec_example(void);
extern bool mcuxClSignature_MlDsa44_KeyGenSignVerify_example(void);
extern bool mcuxClSignature_MlDsa44_Verify_example(void);
extern bool mcuxClSignature_MlDsa65_KeyGenSignVerify_example(void);
extern bool mcuxClSignature_MlDsa65_Verify_example(void);
extern bool mcuxClSignature_MlDsa87_KeyGenSignVerify_example(void);
extern bool mcuxClSignature_MlDsa87_Verify_example(void);

#endif /* _SGI_PKC_PQC_H_ */
