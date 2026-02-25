/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _ISI_CONFIG_H_
#define _ISI_CONFIG_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define CAMERA_DEVICE_OUTPUT_FORMAT     kCSI2RX_DataTypeYUV422_8Bit

/* AP1302 firmware embedding is controlled from board reconfig.cmake.
 * These are fallbacks only.
 */
#ifndef CAMERA_NEED_LOAD_FM
#define CAMERA_NEED_LOAD_FM             1
#endif
#ifndef APP_CAMERA_FW_EMBEDDED
#define APP_CAMERA_FW_EMBEDDED          1
#endif

/* Linker-provided symbol, see board reconfig.cmake which generates an .S file
 * using .incbin and places the firmware into the normal text/rodata region (m_text).
 */
extern const uint8_t __ap1302_fw_start[];
#define APP_CAMERA_FW_ADDRESS           ((uint32_t)__ap1302_fw_start)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
/*${prototype:end}*/
#endif /* _ISI_CONFIG_H_ */
