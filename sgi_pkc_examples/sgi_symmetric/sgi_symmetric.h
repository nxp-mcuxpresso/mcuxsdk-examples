/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SGI_SYMMETRIC_H_
#define _SGI_SYMMETRIC_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXIT_CODE_ERROR 0x00U
#define EXIT_CODE_OK    0x01U
/*******************************************************************************
 * API
 ******************************************************************************/
extern bool mcuxClCipherModes_Cbc_Aes128_Oneshot_example(void);
extern bool mcuxClCipherModes_Ecb_Aes128_Multipart_example(void);
extern bool mcuxClMacModes_Cmac_Aes128_Compute_example(void);

#endif /* _SGI_SYMMETRIC_H_ */
