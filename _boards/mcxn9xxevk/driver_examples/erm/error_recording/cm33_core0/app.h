/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_ERM                              ERM0
#define APP_EIM                              EIM0
#define APP_ERM_IRQ                          ERM_SINGLE_BIT_ERROR_IRQn
#define APP_ERM_SINGLE_BIT_ERROR_IRQ_HANDLER ERM_SINGLE_BIT_ERROR_IRQHandler

#define APP_EIM_MEMORY_CHANNEL    kEIM_MemoryChannelRAMB
#define APP_EIM_MEMORY_CHANNEL_EN kEIM_MemoryChannelRAMBEnable
#define APP_ERM_MEMORY_CHANNEL    kERM_MemoryChannelRAMB
#define APP_ERM_RAM_ECC_ENABLE    0x2U    /* Memory channel RAMX & RAMB */
#define APP_ERM_RAM_START_ADDR    0x20008000u
#define APP_ERM_RAM_SIZE          0x2000U /* 32KB */
#define APP_ERM_MAGIC_NUMBER      0xAAAAAAAAu

#define APP_ERM_RAM_CHECK_ADDR                  \
    0x20008000u /* The address of RAM to check, \
              must be in selected RAM block. */

#define APP_ERM_MEMORY_RECODE_OFFSET (false)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_MemoryInit(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
