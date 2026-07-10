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

#include "fsl_gpio.h"
#if !defined(RW612_SERIES)
#include "fsl_iomuxc.h"
#include "fsl_gpc.h"
#include "fsl_lpuart_edma.h"
#include "fsl_dmamux.h"
#endif /* RW612_SERIES */

//#include "appl_utils.h"

#include "fsl_adapter_uart.h"

//#include "netif/ethernet.h"
//#include "ethernetif.h"
//#include "fsl_enet.h"
#include "app_config.h"

#ifdef RW612_SERIES
#ifndef CONFIG_WPA_SUPP_CRYPTO_MBEDTLS_PSA
#if defined(MBEDTLS_NXP_SSSAPI)
#include "sssapi_mbedtls.h"
#elif defined(MBEDTLS_MCUX_CSS_API)
#include "platform_hw_ip.h"
#include "css_mbedtls.h"
#elif defined(MBEDTLS_MCUX_CSS_PKC_API)
#include "platform_hw_ip.h"
#include "css_pkc_mbedtls.h"
#elif defined(MBEDTLS_MCUX_ELS_PKC_API)
#include "platform_hw_ip.h"
#include "els_pkc_mbedtls.h"
#elif defined(MBEDTLS_MCUX_ELS_API)
#include "platform_hw_ip.h"
#include "els_mbedtls.h"
#elif defined(MBEDTLS_MCUX_ELE_S400_API)
#include "ele_mbedtls.h"
#else
#ifdef CONFIG_KSDK_MBEDTLS
#include "ksdk_mbedtls.h"
#endif
#endif
#endif
#endif /* RW612_SERIES */

#if defined(MBEDTLS_USER_CONFIG_FILE)
#include MBEDTLS_USER_CONFIG_FILE
#endif
#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
#include "threading_alt.h"
#endif

#if defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0)
#include "PWR_Interface.h"
#include "fwk_platform_lowpower.h"
#endif /* APP_LOWPOWER_ENABLED */

#if APP_LOWPOWER_ENABLED
#include "coex_lpm.h"
#include "host_sleep.h"
#endif

#include <peripheral_ht.h>
#include "coex_cli.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

const int TASK_MAIN_PRIO       = (configMAX_PRIORITIES-7);
const int TASK_MAIN_STACK_SIZE = (2 * 1024);
TaskHandle_t task_main_handle;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_InitHardware(void);
extern void coex_controller_init();
extern void APP_InitServices(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

#ifdef RW612_SERIES
#if defined(configUSE_TICKLESS_IDLE) && (configUSE_TICKLESS_IDLE == 1)
/* Tickless idle is allowed by default but can be disabled runtime with APP_SetTicklessIdle */
static int ticklessIdleAllowed = 0;
#endif
#endif

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
#if defined(RW612_SERIES)
    PRINTF("     Initialize RW612 Module\r\n");
#elif defined(WIFI_IW610_BOARD_MURATA_2LL_M2)
    PRINTF("     Initialize Nighthwak-2LL (IW610) M2 Module\r\n");
#elif defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
    PRINTF("     Initialize Firecrest-2EL (IW612) Direct-M2 Module\r\n");
#elif defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2)
    PRINTF("     Initialize 88W8987 1ZM Direct-M2 Module\r\n");
#elif defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
    PRINTF("     Initialize IW416 1XK Direct-M2 Module\r\n");
#endif
    printSeparator();

#if CONFIG_POWER_MANAGER
    coex_LowPowerInit();
#endif

#if CONFIG_COEX_ENABLE_MENU
    coex_cli_init();
#endif
    coex_controller_init();

#if CONFIG_COEX_ENABLE_PM_MENU
    APP_LPTimerCreate();
#endif

    if (xTaskCreate(peripheral_ht_task, "peripheral_ht_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("peripheral ht task creation failed!\r\n");
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
#if (defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0))|| \
    (defined(APP_USE_SENSORS) && (APP_USE_SENSORS > 0))
    APP_InitServices();
#endif
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

#if defined(configUSE_TICKLESS_IDLE) && (configUSE_TICKLESS_IDLE == 1)
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
#if defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0)
    bool abortIdle = false;
    uint64_t expectedIdleTimeUs, actualIdleTimeUs;

    if(ticklessIdleAllowed > 0)
    {
        uint32_t irqMask = DisableGlobalIRQ();

        /* Disable and prepare systicks for low power */
        abortIdle = PWR_SysticksPreProcess((uint32_t)xExpectedIdleTime, &expectedIdleTimeUs);

        if (abortIdle == false)
        {
            /* Enter low power with a maximal timeout */
            actualIdleTimeUs = PWR_EnterLowPower(expectedIdleTimeUs);

            /* Re enable systicks and compensate systick timebase */
            PWR_SysticksPostProcess(expectedIdleTimeUs, actualIdleTimeUs);
        }

        /* Exit from critical section */
        EnableGlobalIRQ(irqMask);
    }
    else
    {
        /* Tickless idle is not allowed, wait for next tick interrupt */
        __WFI();
    }
#endif /* APP_LOWPOWER_ENABLED */
}

void APP_SetTicklessIdle(bool enable)
{
    if(enable == true)
    {
        ticklessIdleAllowed++;
    }
    else
    {
        ticklessIdleAllowed--;
    }
}

#endif /* configUSE_TICKLESS_IDLE */
#endif /* RW612_SERIES*/

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
