/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_ASYMMETRIC_H_
#define _ELS_PKC_ASYMMETRIC_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXIT_CODE_ERROR 0x00U
#define EXIT_CODE_OK    0x01U
/*******************************************************************************
 * API
 ******************************************************************************/
extern bool mcuxClEcc_ECDH_KeyAgreement_NIST_P256_example(void);
extern bool mcuxClEcc_ECDSA_FixedKeys_NIST_P256_example(void);
extern bool mcuxClRsa_KeyGeneration_example(void);
extern bool mcuxClEcc_EdDSA_VerifySignature_Ed25519_example(void);
extern bool mcuxClEcc_MontDH_Curve25519_example(void);
extern bool mcuxClEcc_MontDH_Curve448_example(void);
extern bool mcuxClEcc_EdDSA_Ed25519_example(void);
extern bool mcuxClEcc_ECDSA_FixedKeys_NIST_P256_example(void);

#endif /* _ELS_PKC_ASYMMETRIC_H_ */
