/*
 * Copyright 2025 NXP
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
#define DEMO_POWER_MODE_NAME                    \
    {                                           \
        "Active",           "Sleep",            \
        "Deep Sleep",       "Power Down1",      \
        "Power Down2",      "Deep Power Down1", \
        "Deep Power Down2", "Deep Power Down3", \
        "Shut Down"}
#define DEMO_POWER_MODE_TRANS                                                               \
    {                                                                                       \
        "Active --> Sleep --> Active",                                                      \
        "Active --> Deep Sleep --> Active",                                                 \
        "Active --> Power Down1 --> Active",                                                \
        "Active --> Power Down2 --> Active",                                                \
        "Active --> Deep Power Down1 --> Active",                                           \
        "Active --> Deep Power Down1 --> Deep Power Down2 --> Deep Power Down1 --> Active", \
        "Active --> Deep Power Down1 --> Deep Power Down2 --> Active",                      \
        "Active --> Deep Power Down2 --> Active",                                           \
        "Active --> Deep Power Down2 --> Deep Power Down1 --> Active",                      \
        "Active --> Deep Power Down3 --> Active",                                           \
        "Active --> Shut Down --> Active",                                                  \
    }

#define DEMO_POWER_MODE_DESC             \
    {"Selected to enter Sleep(1.28mA@VDD_BAT), then wakeup to Active.", \
      "Selected to enter Deep Sleep(473uA@VDD_BAT), then wakeup to Active. ",\
      "Selected to enter Power Down1(470uA@VDD_BAT), then wakeup to Active.",   \
     "Selected to enter Power Down2(459uA@VDD_BAT), then wakeup to Active.", \
      "Selected to enter DPD1(No CM33 SRAM retained, CM0P execute WFI, 15.5uA@VDD_BAT), then wakeup to Active.", \
      "Selected to enter DPD1(No CM33 SRAM retained, CM0P active, 22uA@VDD_BAT), input any key with AON UART to enter DPD2(All PLS SRAM retained, 7.8uA@VDD_BAT), wakeup to DPD1(CM0P execute WFI, 24uA) after 10s later with LPTMR, press SW5 back to Active.",  \
     "Selected to enter DPD1(No CM33 SRAM retained, CM0P active, 22uA@VDD_BAT), input any key with AON UART to enter DPD2(No PLS SRAM retained, 3.6uA@VDD_BAT), wakeup to Active after 10s later with LPTMR.",  \
     "Selected to enter DPD2(No PLS SRAM retained, 3.4uA@VDD_BAT), then wakeup to Active.",   \
     "Selected to enter DPD2(All PLS SRAM retained, 7.8uA@VDD_BAT), wakeup to DPD1(No CM33 SRAM retained, CM0P execute WFI, 24uA@VDD_BAT) with selected wakeup source, press SW5 back to Active.",   \
     "Selected to enter DPD3(695nA@VDD_BAT).", \
     "Selected to enter SD(413nA@VDD_BAT)."     \
    }

#define APP_MU         MUA
#define APP_MU_CHANNEL (0U)

#define APP_EXT_INT_BUTTON "SW5"
#define APP_EXT_INT_IRQ    GPIO00_AON_IRQn
#define APP_EXT_INT_ISR    GPIO00_AON_IRQHandler
#define APP_EXT_INT_GPIO   AON__GPIO0
#define APP_EXT_INT_PIN    9U

#define APP_WAKEUP_BUTTON      "SW2" /*P1_14*/
#define APP_WAKEUP_BUTTON_IRQ  GPIO10_IRQn
#define APP_WAKEUP_BUTTON_ISR  GPIO10_IRQHandler
#define APP_WAKEUP_BUTTON_GPIO GPIO1
#define APP_WAKEUP_BUTTON_PIN  14U

#define APP_LPTMR_BASE     AON__LPTMR0
#define LPTMR_USEC_COUNT   10000000U

#define LPTMR_SOURCE_CLOCK (CLOCK_GetFreq(kCLOCK_Fro16k))

#define RTC_ALARM0_TIME_SEC 10U
#define RTC_ALARM1_TIME_SEC 15U

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

#define APP_ENABLE_ADVC (0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void APP_InitROSC(void);
void BOARD_InitHardware(void);
void BOARD_InitWakeupButtonAsGPIO(void);
void BOARD_InitWakeupButtonAsWUUPin(void);
void BOARD_InitExtIntButtonAsGPIO(void);
void APP_BootCore1(void);
uint32_t get_core1_image_size(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
