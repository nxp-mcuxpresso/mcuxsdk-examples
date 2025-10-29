/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_MU              MUA
#define APP_MU_IRQHandler   MU_A_RX_IRQHandler(void){MU_A_Joined_IRQHandler();}; \
                       void MU_A_TX_IRQHandler(void){MU_A_Joined_IRQHandler();}; \
                       void MU_A_INT_IRQHandler(void){MU_A_Joined_IRQHandler();}; \
                       void MU_A_Joined_IRQHandler

#define BOOT_CORE1_BY_MU    0

#define CORE1_BOOT_ADDRESS  0xA1000000

#define LED_INIT()          //not possible to use on-board LED from the secondary core
#define LED_TOGGLE()        //not possible to use on-board LED from the secondary core

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
extern unsigned char core1_image_start[];
#define CORE1_IMAGE_START core1_image_start
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern uint32_t core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void MU_A_Joined_IRQHandler(void);
void BOARD_InitHardware(void);
void APP_BootCore1(void);
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif
/*${prototype:end}*/

#endif /* _APP_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/