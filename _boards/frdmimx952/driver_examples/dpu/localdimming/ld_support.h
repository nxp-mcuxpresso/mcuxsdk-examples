/*
 * Copyright 2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _LD_SUPPORT_H_
#define _LD_SUPPORT_H_

/*******************************************************************************
 * DEFINITION
 ******************************************************************************/
#define LD_SRAM_ADDRESS  (uint32_t *)0x4B300000

#if defined(__GNUC__)
extern const unsigned char __ld_fw_start[];
extern const unsigned char __ld_fw_end[];
extern const unsigned long __ld_fw_size[];

#define LD_IMAGE_DATA_START ((uint32_t *)__ld_fw_start)
#define LD_IMAGE_DATA_END   ((uint32_t *)__ld_fw_end)
#define LD_IMAGE_DATA_SIZE  ((uintptr_t)__ld_fw_size)
#endif


/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

void BOARD_LD_Init(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _LD_SUPPORT_H_ */
