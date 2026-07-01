/*
 * Copyright 2021, 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __TZM_API_H__
#define __TZM_API_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef TZM_IS_NONSECURE_CALLED
#define TZM_IS_NONSECURE_CALLED __attribute__((cmse_nonsecure_call))
#endif

#ifndef TZM_IS_NOSECURE_ENTRY
#if defined(BOARD_TZM_SG_IN_SRAM)
#define TZM_IS_NOSECURE_ENTRY \
    __attribute__((cmse_nonsecure_entry, section(".sg_ramfunc")))
#else
#define TZM_IS_NOSECURE_ENTRY \
    __attribute__((cmse_nonsecure_entry))
#endif
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

void TZM_JumpToNormalWorld(uint32_t nonsecVtorAddress);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* __TZM_API_H__ */
