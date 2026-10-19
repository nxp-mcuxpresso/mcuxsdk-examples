/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ELS_HASH_H_
#define _ELS_HASH_H_

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
extern uint32_t mcuxClHashModes_sha224_oneshot_example(void);
extern uint32_t mcuxClHashModes_sha256_oneshot_example(void);
extern uint32_t mcuxClHashModes_sha256_longMsgOneshot_example(void);
extern uint32_t mcuxClHashModes_sha256_streaming_example(void);
extern uint32_t mcuxClHashModes_sha384_oneshot_example(void);
extern uint32_t mcuxClHashModes_sha512_oneshot_example(void);

#endif /* _ELS_HASH_H_ */
