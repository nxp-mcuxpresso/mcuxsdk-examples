/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/
#include "pin_mux.h"
#include "board_comp.h"
#include "app.h"
#if (defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)) || (defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0))
#include "fsl_adapter_gpio.h"
#endif
#if defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c >= 1)
#include "fsl_component_serial_manager.h"
#endif
#if defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)
#include "fsl_component_led.h"
#endif
#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)
#include "fsl_component_button.h"
#endif

#if (defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 1)) && defined(gDebugConsoleEnable_d) && \
    (gDebugConsoleEnable_d == 1)
#error second serial manager Instance can not be used if DebugConsole enabled, (sharing the same UART instance)
#endif

#if (defined(gAppOtaExternalStorage_c) && (gAppOtaExternalStorage_c > 0))
#if (defined(gAppLedCnt_c) && (gAppLedCnt_c > 0))
#if (!defined(gAppRequireRgbLed_c) || (gAppRequireRgbLed_c == 0))
/* Even if the BSP supports 2 LEDs the Application may want one LED only but
 * that LED may RGB or monochrome.
 * Here, if the external flash is required, we can only use the RGDB LED.
 */
#error "Monochrome LED use is incompatible with external Flash PB0 conflict"
#endif /* gAppRequireRgbLed_c */
#endif
#endif /* gAppOtaExternalStorage_c */
/************************************************************************************
*************************************************************************************
* Private type definitions and macros
*************************************************************************************
************************************************************************************/
#ifndef SERIAL_MANAGER_RING_BUFFER_SIZE
#define SERIAL_MANAGER_RING_BUFFER_SIZE (128U)
#endif

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

#if defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c >= 1)

static uint8_t s_ringBuffer1[SERIAL_MANAGER_RING_BUFFER_SIZE];

static serial_port_uart_config_t uartConfig1 = {
    .instance     = BOARD_DEBUG_UART_INSTANCE,
    .baudRate     = BOARD_DEBUG_UART_BAUDRATE,
    .parityMode   = kSerialManager_UartParityDisabled,
    .stopBitCount = kSerialManager_UartOneStopBit,
    .enableRx     = 1,
    .enableTx     = 1,
#if (defined(gBoardUseUart0HwFlowControl) && (gBoardUseUart0HwFlowControl > 0) && (BOARD_APP_UART_INSTANCE == 0U))
    .enableRxRTS  = 1,
    .enableTxCTS  = 1,
#endif
};

static const serial_manager_config_t s_serialManagerConfig1 = {
    .type           = kSerialPort_Uart,
    .ringBuffer     = &s_ringBuffer1[0],
    .ringBufferSize = SERIAL_MANAGER_RING_BUFFER_SIZE,
    .portConfig     = (serial_port_uart_config_t *)&uartConfig1,
};
#endif /* defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c >= 1) */

#if defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)

#if (defined gBoardLedMonochromeHdl)
static const led_config_t g_monoLedConfig = {
    .type = kLED_TypeMonochrome,
    .ledMonochrome =
        {
            .monochromePin =
                {
                    .dimmingEnable = 0,
                    .gpio =
                        {
#if (defined(LED_USE_CONFIGURE_STRUCTURE) && (LED_USE_CONFIGURE_STRUCTURE > 0U))
                            kHAL_GpioDirectionOut,
#endif
                            BOARD_LED_GPIO_DEFAULT_STATE,
                            BOARD_LED_GPIO_PORT,
                            BOARD_LED_GPIO_PIN,
                        },
                },
        },
};
#endif

#if defined(gBoardLedRgbHdl)
static const led_config_t g_RgbLedConfig = {
    .type = kLED_TypeRgb,
    .ledRgb =
        {
            .redPin =
                {
                    .dimmingEnable = 0,
                    .gpio =
                        {
#if (defined(LED_USE_CONFIGURE_STRUCTURE) && (LED_USE_CONFIGURE_STRUCTURE > 0U))
                            .direction = kHAL_GpioDirectionOut,
#endif
                            .level = BOARD_RGB_LED_RED_DEFAULT_STATE,
                            .port  = BOARD_RGB_LED_RED_GPIO_PORT,
                            .pin   = BOARD_RGB_LED_RED_GPIO_PIN,
                        },
                },
            .greenPin =
                {
                    .dimmingEnable = 0,
                    .gpio =
                        {
#if (defined(LED_USE_CONFIGURE_STRUCTURE) && (LED_USE_CONFIGURE_STRUCTURE > 0U))
                            .direction = kHAL_GpioDirectionOut,
#endif
                            .level = BOARD_RGB_LED_GREEN_DEFAULT_STATE,
                            .port  = BOARD_RGB_LED_GREEN_GPIO_PORT,
                            .pin   = BOARD_RGB_LED_GREEN_GPIO_PIN,
                        },
                },
            .bluePin =
                {
                    .dimmingEnable = 0,
                    .gpio =
                        {
#if (defined(LED_USE_CONFIGURE_STRUCTURE) && (LED_USE_CONFIGURE_STRUCTURE > 0U))
                            .direction = kHAL_GpioDirectionOut,
#endif
                            .level = BOARD_RGB_LED_BLUE_DEFAULT_STATE,
                            .port  = BOARD_RGB_LED_BLUE_GPIO_PORT,
                            .pin   = BOARD_RGB_LED_BLUE_GPIO_PIN,
                        },
                },
        },
};
#endif


#if (defined(gBoardLedBlue_d) && (gBoardLedBlue_d == 1))
static const led_config_t g_BlueLedConfig = {
    .type = kLED_TypeMonochrome,
    .ledMonochrome =
        {
            .monochromePin =
                {
                    .dimmingEnable = 0,
                    .gpio =
                        {
#if (defined(LED_USE_CONFIGURE_STRUCTURE) && (LED_USE_CONFIGURE_STRUCTURE > 0U))
                            kHAL_GpioDirectionOut,
#endif
                            BOARD_LED_BLUE_DEFAULT_STATE,
                            BOARD_LED_BLUE_GPIO_PORT,
                            BOARD_LED_BLUE_GPIO_PIN,
                        },
                },
        },
};
#endif

#endif /* defined(gAppLedCnt_c) && (gAppLedCnt_c > 0) */

#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)

/*Button pins configuration*/
static const button_config_t g_button0Config = {
    .gpio =
        {
            .pinStateDefault = BOARD_BUTTON1_DEFAULT_STATE,
            .port            = BOARD_BUTTON1_GPIO_PORT,
            .pin             = BOARD_BUTTON1_GPIO_PIN,
        },
};

static const button_config_t g_button1Config = {
    .gpio =
        {
            .pinStateDefault = BOARD_WAKEUP_GPIO_DEFAULT_STATE,
            .port            = BOARD_WAKEUP_GPIO_PORT,
            .pin             = BOARD_WAKEUP_GPIO_PIN,
        },
};

#endif /* defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0) */

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

#if defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c >= 1)

/* Initialize the serial manager */
void BOARD_InitSerialManager(serial_handle_t serialManagerHandle)
{
    serial_manager_status_t ret;
    
    /* First instance of serial manager uses the UART for FSCI */
    uartConfig1.clockRate = CLOCK_GetFreq(BOARD_DEBUG_UART_CLKSRC);

    /* Init Serial Manager */
    ret = SerialManager_Init((serial_handle_t)serialManagerHandle, &s_serialManagerConfig1);
    assert(kStatus_SerialManager_Success == ret);
    (void)ret;
}

/* Initialize the second serial manager */
void BOARD_InitSerialManager2(serial_handle_t serialManagerHandle)
{
    /* Second instance of serial manager uses the UART for Debug Console */
    BOARD_InitDebugConsole2();
}

/* DeInit Serial Manager before going lowpower when wake domain is in DS  (device DS3 or lower)*/
void BOARD_UninitSerialManager(void)
{
    /* disable clock and pins */
    BOARD_UninitDebugConsole();
}

/* Initialize the serial manager */
void BOARD_ReinitSerialManager(serial_handle_t serialManagerHandle)
{
    serial_manager_status_t ret;

    /* Init LPUART clock and pins */
    BOARD_InitDebugConsole();

    ret = SerialManager_ExitLowpower((serial_handle_t)serialManagerHandle);
    assert(kStatus_SerialManager_Success == ret);
    (void)ret;
}

/* DeInit Serial Manager before going lowpower when wake domain is in DS  (device DS3 or lower)*/
void BOARD_UninitSerialManager2(void)
{
    /* disable clock and pins */
    BOARD_UninitDebugConsole2();
}
/* Change the serial manager BaudRate*/
void BOARD_ChangeSerialManagerBaudRate(serial_handle_t serialManagerHandle, uint32_t baudrate)
{
    serial_manager_status_t ret;

    ret = SerialManager_Deinit((serial_handle_t)serialManagerHandle);
    assert(kStatus_SerialManager_Success == ret);
    (void)ret;
    BOARD_InitSerialManager((serial_handle_t)serialManagerHandle);
}

/* Initialize the serial manager */
void BOARD_ReinitSerialManager2(serial_handle_t serialManagerHandle)
{
    serial_manager_status_t ret;

    /* Init LPUART clock and pins */
    BOARD_InitDebugConsole2();

    ret = SerialManager_ExitLowpower((serial_handle_t)serialManagerHandle);
    assert(kStatus_SerialManager_Success == ret);
    (void)ret;
}

#endif /* defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c >= 1) */

#if defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)

/*Initialize LED*/
#if (defined gBoardLedMonochromeHdl)
void BOARD_InitMonochromeLed(led_handle_t ledHandle)
{
    led_status_t ret;


    ret = LED_Init(ledHandle, &g_monoLedConfig);
    assert(kStatus_LED_Success == ret);
    (void)ret;
}

void BOARD_UnInitMonochromeLed(led_handle_t ledHandle)
{
    led_status_t ret;
    ret = LED_TurnOnOff(ledHandle, 0U);
    assert(kStatus_LED_Success == ret);
    (void)ret;
}
#endif

/*Initialize LED*/
#if defined(gBoardLedRgbHdl)
void BOARD_InitRgbLed(led_handle_t ledHandle)
{
    led_status_t ret;


    ret = LED_Init(ledHandle, &g_RgbLedConfig);
    assert(kStatus_LED_Success == ret);
    (void)ret;
}

void BOARD_UnInitRgbLed(led_handle_t ledHandle)
{
    led_status_t ret = LED_TurnOnOff(ledHandle, 0U);
    assert(kStatus_LED_Success == ret);
    (void)ret;
}
#endif

#endif /* defined(gAppLedCnt_c) && (gAppLedCnt_c > 0) */

#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)

/*Initialize Button*/
void BOARD_InitButton0(button_handle_t buttonHandle)
{
    button_status_t ret;
    button_config_t button_config = g_button0Config;

    /* Init button module and Gpio module */
    ret = BUTTON_Init(buttonHandle, &button_config);
    assert(ret == kStatus_BUTTON_Success);
    (void)ret;
}

void BOARD_Button0ExitPowerDown(button_handle_t buttonHandle)
{
    button_status_t ret;

    ret = BUTTON_ExitLowpower(buttonHandle);
    assert(ret == kStatus_BUTTON_Success);
    (void)ret;
}

void BOARD_InitButton1(button_handle_t buttonHandle)
{
    button_status_t ret;
    button_config_t button_config = g_button1Config;

    /* Init button module and Gpio module */
    ret = BUTTON_Init(buttonHandle, &button_config);
    assert(ret == kStatus_BUTTON_Success);
    (void)ret;
}

void BOARD_Button1ExitPowerDown(button_handle_t buttonHandle)
{
    button_status_t ret;

    ret = BUTTON_ExitLowpower(buttonHandle);
    assert(ret == kStatus_BUTTON_Success);
    (void)ret;
}

#endif /* defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0) */

/************************************************************************************
*************************************************************************************
* Localization board specific functions
*************************************************************************************
************************************************************************************/

#if defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)
#if (defined(gBoardLedBlue_d) && (gBoardLedBlue_d == 1))
/*Initialize LED*/
void BOARD_InitBlueLed(led_handle_t ledHandle)
{
    led_status_t ret;

    ret = LED_Init(ledHandle, &g_BlueLedConfig);
    assert(kStatus_LED_Success == ret);
    (void)ret;
}

void BOARD_UnInitBlueLed(led_handle_t ledHandle)
{
    led_status_t ret;
    ret = LED_TurnOnOff(ledHandle, 0U);
    assert(kStatus_LED_Success == ret);
    (void)ret;
}
#endif
#endif /* defined(gAppLedCnt_c) && (gAppLedCnt_c > 0) */
