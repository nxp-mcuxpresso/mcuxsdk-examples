/** @file main.c
 *
 *  @brief main file
 *
 *  Copyright 2008-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////

// SDK Included Files
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"

#if !defined(RW612_SERIES)
#include "wifi.h"
#include "fsl_sdmmc_host.h"
#endif /* RW612_SERIES */

#include "fsl_common.h"
#include "fsl_device_registers.h"

#include "BT_common.h"
#include "BT_version.h"
#include "BT_hci_api.h"

#include "fsl_gpio.h"
#if !defined(RW612_SERIES)
#include "fsl_iomuxc.h"
#include "fsl_gpc.h"
#include "fsl_lpuart_edma.h"
#include "fsl_dmamux.h"
#endif /* RW612_SERIES */

#include "fsl_adapter_uart.h"
#include "usb_host_config.h"
#include "usb_host.h"

#include "netif/ethernet.h"
#include "app_config.h"

#if defined(MBEDTLS_USER_CONFIG_FILE)
#include MBEDTLS_USER_CONFIG_FILE
#endif
#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
#include "threading_alt.h"
#endif

#ifndef CONFIG_WIFI_BLE_COEX_APP
#define CONFIG_WIFI_BLE_COEX_APP 1 // needs to define CONFIG_WIFI_BLE_COEX_APP with value, 0 for disable Wi-Fi, 1 for enable Wi-Fi
#endif

#ifndef CONFIG_OT_CLI
#define CONFIG_OT_CLI 0 // needs to define CONFIG_OT_CLI with value, 0 for disable OT, 1 for enable OT
#endif

struct bt_conn; // Forward declaration

#include "app_a2dp_sink.h"
#include "coex_cli.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

const int TASK_MAIN_PRIO       = (configMAX_PRIORITIES-5);
const int TASK_MAIN_STACK_SIZE = (2 * 1024);
TaskHandle_t task_main_handle;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void BOARD_InitHardware(void);
extern void coex_controller_init();
extern void otSysRunIdleTask(void);
//extern void APP_InitServices(void);
/*******************************************************************************
 * Code
 ******************************************************************************/

static void printSeparator(void)
{
    PRINTF("========================================\r\n");
}

void task_main(void *param)
{
    printSeparator();
#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
    PRINTF("     Initialize Firecrest-2EL (IW612) M2 Module\r\n");
#elif defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
    PRINTF("     Initialize RB3P 1XK Direct-M2 Module\n");
#elif defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2)
    PRINTF("     Initialize CA2 1ZM Direct-M2 Module\r\n");
#endif
    printSeparator();

#if CONFIG_COEX_ENABLE_MENU
    coex_cli_init();
#endif
    coex_controller_init();

    if (xTaskCreate(app_a2dp_sink_task, "app_a2dp_sink_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("a2dp sink task creation failed!\r\n");
        while (1)
            ;
    }

#if CONFIG_COEX_ENABLE_MENU
    coex_menuPrint();
    while (1)
    {
        int ch = pollChar();
        if (ch != -1)
        {
            coex_menuAction(ch);
        }
    }
#endif
}

#if defined (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK) \
        && (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK == 1U)
void stackOverflowHookHandler(void* task_name)
{
    printf("stack-overflow exception from task: %s\r\n",(char*)task_name);
}
#endif /* #if defined (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK) && (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK == 1U) */


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

int main(void)
{
    BaseType_t result = 0;
    (void)result;

    extern void BOARD_InitHardware(void);    /*fix build warning: function declared implicitly.*/
    BOARD_InitHardware();
  
#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
    config_mbedtls_threading_alt();
#endif

#ifdef RW612_SERIES
#ifdef OOB_WAKEUP
    Configure_H2C_gpio();
    C2H_sleep_gpio_cfg();
#endif
#endif /* RW612_SERIES */

    printSeparator();
    PRINTF("        Coex APP\r\n");
    printSeparator();

    result =
        xTaskCreate(task_main, "main", TASK_MAIN_STACK_SIZE, NULL, TASK_MAIN_PRIO, &task_main_handle);
    assert(pdPASS == result);    
#if defined (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK) \
        && (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK == 1U)
    EM_register_sof_handler(stackOverflowHookHandler);
#endif /* #if defined (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK) && (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK == 1U) */

    vTaskStartScheduler();
    for (;;)
    {
        ;
    }
}

#if CONFIG_COEX_APP
void vApplicationIdleHook(void)
{
#if(CONFIG_OT_CLI)
    otSysRunIdleTask();
#endif
}
#endif /* CONFIG_COEX_APP*/

#ifndef APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK
/**
 * @brief Loop forever if stack overflow is detected.
 *
 * If configCHECK_FOR_STACK_OVERFLOW is set to 1,
 * this hook provides a location for applications to
 * define a response to a stack overflow.
 *
 * Use this hook to help identify that a stack overflow
 * has occurred.
 *
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName)
{
    PRINTF("ERROR: stack overflow on task %s.\r\n", pcTaskName);

    portDISABLE_INTERRUPTS();

    /* Unused Parameters */
    (void)xTask;
    (void)pcTaskName;

    /* Loop forever */
    for (;;)
    {
    }
}
#endif

#ifndef __GNUC__
void __assert_func(const char *file, int line, const char *func, const char *failedExpr)
{
    PRINTF("ASSERT ERROR \" %s \": file \"%s\" Line \"%d\" function name \"%s\" \n", failedExpr, file, line, func);
    for (;;)
    {
        __BKPT(0);
    }
}
#endif
