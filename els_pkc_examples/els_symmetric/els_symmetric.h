/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_PKC_SYMMETRIC_H_
#define _ELS_PKC_SYMMETRIC_H_

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
extern uint32_t mcuxClEls_Cipher_Aes128_Cbc_Encrypt_example(void);
extern uint32_t mcuxClEls_Cipher_Aes128_Ecb_Encrypt_example(void);
extern uint32_t mcuxClMacModes_Els_Cmac_Aes128_Oneshot_example(void);

#endif /* _ELS_PKC_SYMMETRIC_H_ */
