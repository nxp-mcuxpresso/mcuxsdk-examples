/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_slcd.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define LCD (&g_slcd)
#define APP_SLCD_DUTY_CYCLE          kSLCD_1Div4DutyCycle
#define APP_SLCD_LOW_PIN_ENABLED     0xC04180C2U /* LCD_P1/6/7/15/16/22/30/31 = 1. */
#define APP_SLCD_HIGH_PIN_ENABLED    0x00AF42C6U /* LCD_P30/31/33/34/38/39/41/46/48/49/50/51/53/55 = 1. */
#define APP_SLCD_BACK_PANEL_LOW_PIN  0x00418002U /* LCD_P1/15/16/22 = 1. */
#define APP_SLCD_BACK_PANEL_HIGH_PIN 0U

/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS 0xA1000000

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
 * Variables
 ******************************************************************************/
/*${variable:start}*/
extern SLCD_Type g_slcd;
extern const uint8_t slcd_lcd_gpio_seg_pin[];
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_SetSlcdBackPlanePhase(void);
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif
/*${prototype:end}*/

#endif /* _APP_H_ */