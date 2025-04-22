/*
 * Copyright 2024 NXP
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
#define APP_ERM_IRQ                          ERM0_SINGLE_BIT_IRQn
#define APP_ERM_SINGLE_BIT_ERROR_IRQ_HANDLER ERM0_SINGLE_BIT_IRQHandler

#define APP_EIM_MEMORY_CHANNEL    kEIM_MemoryChannelRAMA0
#define APP_EIM_MEMORY_CHANNEL_EN kEIM_MemoryChannelRAMAEnable
#define APP_ERM_MEMORY_CHANNEL    kERM_MemoryChannelRAMA0
#define APP_ERM_RAM_ECC_ENABLE    0x1U   /* Memory channel RAMA0 */
#define APP_ERM_RAM_START_ADDR    0x20000000UL
#define APP_ERM_RAM_SIZE          0x800U /* 8KB */
#define APP_ERM_MAGIC_NUMBER      0xAAAAAAAAU

#define APP_ERM_RAM_CHECK_ADDR 0x20000100UL /* The address of RAM to check, must be in selected RAM block. */

#define APP_ERM_MEMORY_RECODE_OFFSET                \
    (true) /* In MCXA, the offset of error location \
            is provided not the absolute address. */

/*${macro:end}*/

/*******************************************************************************
 * Functions
 ******************************************************************************/
/*${function:start}*/

/*${function:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_MemoryInit(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
