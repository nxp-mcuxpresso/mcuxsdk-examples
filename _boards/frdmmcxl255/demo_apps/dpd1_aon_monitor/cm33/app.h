/*
 * Copyright 2026 NXP
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
#define APP_MU         MUA
#define APP_MU_CHANNEL (0U)

/* AON LPUART — CM33 enables its RX interrupt before DPD1 entry;
 * CM0+ handles the actual NVIC interrupt and performs the wakeup. */
#define APP_AON_LPUART AON__LPUART0

#define CORE1_BOOT_ADDRESS (0xA1000000UL)

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
extern unsigned char core1_image_start[];
#define CORE1_IMAGE_START core1_image_start
#elif defined(__GNUC__)
extern const char core1_image_start[];
extern const char *core1_image_end;
extern int core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_BootCore1(void);
uint32_t get_core1_image_size(void);
void BOARD_InitDEBUG_UARTPins(void);
void BOARD_Init96MClocksBoot(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
