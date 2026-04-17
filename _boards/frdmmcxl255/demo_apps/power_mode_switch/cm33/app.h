/*
 * Copyright 2025-2026 NXP
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

#ifndef APP_ENABLE_ADVC
#define APP_ENABLE_ADVC           (CONFIG_APP_ENABLE_ADVC)
#endif
#ifndef APP_ENABLE_CONTEXT_SAVING
#define APP_ENABLE_CONTEXT_SAVING (CONFIG_APP_ENABLE_CONTEXT_SAVING)
#endif

/* Common power mode descriptions (modes 0-3: same for all configurations) */
#define DEMO_POWER_MODE_DESC_COMMON \
    "Selected to enter Sleep(1.32mA@VDD_BAT), then wakeup to Active.",    \
    "Selected to enter Deep Sleep(545uA@VDD_BAT), then wakeup to Active. ", \
    "Selected to enter Power Down1(492uA@VDD_BAT), then wakeup to Active.", \
    "Selected to enter Power Down2(428uA@VDD_BAT), then wakeup to Active."

/* Power mode descriptions vary based on context saving and ADVC settings */
#if APP_ENABLE_CONTEXT_SAVING
    #if APP_ENABLE_ADVC
        /* Context Saving Enabled + ADVC Enabled */
        #define DEMO_POWER_MODE_DESC_DPD1_SINGLE \
            "Selected to enter DPD1(All CM33 SRAM retained, CM0P execute WFI, 24.2uA@VDD_BAT), then wakeup to Active."
        #define DEMO_POWER_MODE_DESC_DPD1_TO_DPD2_TO_DPD1 \
            "Selected to enter DPD1(All CM33 SRAM retained, CM0P active, 32.6uA@VDD_BAT), input any key with AON UART to enter " \
            "DPD2(All AON SRAM retained, 6.61uA@VDD_BAT), wakeup to DPD1(CM0P execute WFI, 24.17uA) after 10s later with LPTMR, " \
            "press SW5 back to Active."
        #define DEMO_POWER_MODE_DESC_DPD1_TO_DPD2_TO_ACTIVE \
            "Selected to enter DPD1(All CM33 SRAM retained, CM0P active, 32.6uA@VDD_BAT), input any key with AON UART to enter " \
            "DPD2(All AON SRAM retained, 6.61uA@VDD_BAT), wakeup to Active after 10s later with LPTMR."
        #define DEMO_POWER_MODE_DESC_DPD2_SINGLE \
            "Selected to enter DPD2(All AON SRAM retained, 6.61uA@VDD_BAT), then wakeup to Active."
        #define DEMO_POWER_MODE_DESC_DPD2_TO_DPD1 \
            "Selected to enter DPD2(All AON SRAM retained, 11.81uA@VDD_BAT), wakeup to DPD1(All CM33 SRAM retained, CM0P " \
            "execute WFI, 41.01uA@VDD_BAT) with selected wakeup source, press SW5 back to Active."
    #else
        /* Context Saving Enabled + ADVC Disabled */
        #define DEMO_POWER_MODE_DESC_DPD1_SINGLE \
            "Selected to enter DPD1(All CM33 SRAM retained, CM0P execute WFI, 27.48uA@VDD_BAT), then wakeup to Active."
        #define DEMO_POWER_MODE_DESC_DPD1_TO_DPD2_TO_DPD1 \
            "Selected to enter DPD1(All CM33 SRAM retained, CM0P active, 38.40uA@VDD_BAT), input any key with AON UART to enter " \
            "DPD2(All AON SRAM retained, 6.1uA@VDD_BAT), wakeup to DPD1(CM0P execute WFI, 38.40uA) after 10s later with LPTMR, " \
            "press SW5 back to Active."
        #define DEMO_POWER_MODE_DESC_DPD1_TO_DPD2_TO_ACTIVE \
            "Selected to enter DPD1(All CM33 SRAM retained, CM0P active, 38.40uA@VDD_BAT), input any key with AON UART to enter " \
            "DPD2(All AON SRAM retained, 6.1uA@VDD_BAT), wakeup to Active after 10s later with LPTMR."
        #define DEMO_POWER_MODE_DESC_DPD2_SINGLE \
            "Selected to enter DPD2(All AON SRAM retained, 6.1uA@VDD_BAT), then wakeup to Active."
        #define DEMO_POWER_MODE_DESC_DPD2_TO_DPD1 \
            "Selected to enter DPD2(All AON SRAM retained, 6.1uA@VDD_BAT), wakeup to DPD1(All CM33 SRAM retained, CM0P " \
            "execute WFI, 35.12uA@VDD_BAT) with selected wakeup source, press SW5 back to Active."
    #endif
#else
    #if APP_ENABLE_ADVC
        /* Context Saving Disabled + ADVC Enabled */
        #define DEMO_POWER_MODE_DESC_DPD1_SINGLE \
            "Selected to enter DPD1(No CM33 SRAM retained, CM0P execute WFI, 19.57uA@VDD_BAT), then wakeup to Active."
        #define DEMO_POWER_MODE_DESC_DPD1_TO_DPD2_TO_DPD1 \
            "Selected to enter DPD1(No CM33 SRAM retained, CM0P active, 32uA@VDD_BAT), input any key with AON UART to enter " \
            "DPD2(All AON SRAM retained, 5.8uA@VDD_BAT), wakeup to DPD1(CM0P execute WFI, 32uA) after 10s later with LPTMR, " \
            "press SW5 back to Active."
        #define DEMO_POWER_MODE_DESC_DPD1_TO_DPD2_TO_ACTIVE \
            "Selected to enter DPD1(No CM33 SRAM retained, CM0P active, 32uA@VDD_BAT), input any key with AON UART to enter " \
            "DPD2(No AON SRAM retained, 2.6uA@VDD_BAT), wakeup to Active after 10s later with LPTMR."
        #define DEMO_POWER_MODE_DESC_DPD2_SINGLE \
            "Selected to enter DPD2(No AON SRAM retained, 2.5uA@VDD_BAT), then wakeup to Active."
        #define DEMO_POWER_MODE_DESC_DPD2_TO_DPD1 \
            "Selected to enter DPD2(All AON SRAM retained, 5.85uA@VDD_BAT), wakeup to DPD1(No CM33 SRAM retained, CM0P " \
            "execute WFI, 35uA@VDD_BAT) with selected wakeup source, press SW5 back to Active."
    #else
        /* Context Saving Disabled + ADVC Disabled */
        #define DEMO_POWER_MODE_DESC_DPD1_SINGLE \
            "Selected to enter DPD1(No CM33 SRAM retained, CM0P execute WFI, 24.12uA@VDD_BAT), then wakeup to Active."
        #define DEMO_POWER_MODE_DESC_DPD1_TO_DPD2_TO_DPD1 \
            "Selected to enter DPD1(No CM33 SRAM retained, CM0P active, 41uA@VDD_BAT), input any key with AON UART to enter " \
            "DPD2(All AON SRAM retained, 5.89uA@VDD_BAT), wakeup to DPD1(CM0P execute WFI, 41uA) after 10s later with LPTMR, " \
            "press SW5 back to Active."
        #define DEMO_POWER_MODE_DESC_DPD1_TO_DPD2_TO_ACTIVE \
            "Selected to enter DPD1(No CM33 SRAM retained, CM0P active, 40uAuA@VDD_BAT), input any key with AON UART to enter " \
            "DPD2(No AON SRAM retained, 2.52uA@VDD_BAT), wakeup to Active after 10s later with LPTMR."
        #define DEMO_POWER_MODE_DESC_DPD2_SINGLE \
            "Selected to enter DPD2(No AON SRAM retained, 2.45uA@VDD_BAT), then wakeup to Active."
        #define DEMO_POWER_MODE_DESC_DPD2_TO_DPD1 \
            "Selected to enter DPD2(All AON SRAM retained, 5.90uA@VDD_BAT), wakeup to DPD1(No CM33 SRAM retained, CM0P " \
            "execute WFI, 52uA@VDD_BAT) with selected wakeup source, press SW5 back to Active."
    #endif
#endif

/* Common tail descriptions (same for all configurations) */
#define DEMO_POWER_MODE_DESC_TAIL \
    "Selected to enter DPD3(498nA@VDD_BAT).", \
    "Selected to enter SD(175nA@VDD_BAT)."

/* Final power mode description array - built from components */
#define DEMO_POWER_MODE_DESC \
    { \
        DEMO_POWER_MODE_DESC_COMMON, \
        DEMO_POWER_MODE_DESC_DPD1_SINGLE, \
        DEMO_POWER_MODE_DESC_DPD1_TO_DPD2_TO_DPD1, \
        DEMO_POWER_MODE_DESC_DPD1_TO_DPD2_TO_ACTIVE, \
        DEMO_POWER_MODE_DESC_DPD2_SINGLE, \
        DEMO_POWER_MODE_DESC_DPD2_TO_DPD1, \
        DEMO_POWER_MODE_DESC_TAIL \
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

#define APP_LPTMR_BASE   AON__LPTMR0
#define LPTMR_USEC_COUNT 10000000U

#define LPTMR_SOURCE_CLOCK (CLOCK_GetFreq(kCLOCK_Fro16k))

#define APP_RTC_BASE        AON__RTC_AON
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
void BOARD_InitDEBUG_UARTPins(void);
void BOARD_Init96MClocksBoot(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
