/* ---------------------------------------------------------------------------------------
 * @file:    cfpa_data.s
 * @purpose: CMSIS Cortex-M33 Core Device Startup File for the LPC55S16
 * @version: 1.1
 * @date:    2020-11-7
 * ---------------------------------------------------------------------------------------*/
/*
 * Copyright 1997-2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*****************************************************************************/
/* Version: GCC for ARM Embedded Processors                                  */
/*****************************************************************************/

.section cfpa_data_section
.global cfpa_data
.type cfpa_data, %object
.align 4

cfpa_data:
.incbin "../../script/pfr/cfpa.bin"
	
cfpa_data_end:
.global cfpa_data_size
.type cfpa_data_size, %object
.align 4

cfpa_data_size:
.int cfpa_data_size - cfpa_data