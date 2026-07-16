/** @file main.c
 *
 *  @brief main file
 *
 *  Copyright 2020-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *  The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////

// SDK Included Files
#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_os_abstraction.h"
#include "app.h"

#include "fsl_rtc.h"
#include "fsl_power.h"

#include "ncp_adapter.h"
#include "app_notify.h"
#include "osa.h"
#if defined(MBEDTLS_USER_CONFIG_FILE)
#include MBEDTLS_USER_CONFIG_FILE
#endif
#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
#include "threading_alt.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define NCP_INBUF_SIZE     4096

#if (CONFIG_NCP_WIFI) && !(CONFIG_NCP_BLE)
#define TASK_MAIN_PRIO         (PRIORITY_RTOS_TO_OSA(((configMAX_PRIORITIES - CONFIG_NCP_PRIORITY_OFFSET)-5)))
#else
#define TASK_MAIN_PRIO         (PRIORITY_RTOS_TO_OSA(((configMAX_PRIORITIES - CONFIG_NCP_PRIORITY_OFFSET)-9)))
#endif
#define TASK_MAIN_STACK_SIZE   4096

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern int system_ncp_init(void);
extern int ncp_cmd_list_init(void);
#if CONFIG_NCP_WIFI
extern int wifi_ncp_init(void);
#endif
#if CONFIG_NCP_BLE
extern int ble_ncp_init(void);
#endif
#if CONFIG_NCP_OT
extern void appOtStart(int argc, char *argv[]);
extern void otSysRunIdleTask(void);
#endif
extern void coex_controller_init(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

void task_main(osa_task_param_t arg);
OSA_TASK_DEFINE(task_main, TASK_MAIN_PRIO, 1, TASK_MAIN_STACK_SIZE, 0);
static OSA_TASK_HANDLE_DEFINE(main_task_handle);

uint32_t current_cmd = 0;
uint16_t g_cmd_seqno = 0;
uint8_t cmd_buf[NCP_INBUF_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/

static void printSeparator(void)
{
    PRINTF("========================================\r\n");
}

void task_main(void *param)
{
    int32_t result = 0;

    result = ncp_adapter_init(0);
    assert(NCP_SUCCESS == result);

    printSeparator();
    PRINTF("NCP device demo\r\n");
    printSeparator();

    PRINTF("Initialize NCP config littlefs CLIs\r\n");
    result = system_ncp_init();
    assert(NCP_SUCCESS == result);

#if CONFIG_NCP_WIFI
    result = wifi_ncp_init();
    assert(NCP_SUCCESS == result);
#endif

    coex_controller_init();

#if CONFIG_NCP_BLE
    result = ble_ncp_init();
    assert(NCP_SUCCESS == result);
#endif

#if CONFIG_NCP_OT
    appOtStart(0, NULL);
    PRINTF("OT initialized\r\n");
#endif

    result = ncp_cmd_list_init();
    assert(NCP_SUCCESS == result);

    app_notify_event(APP_EVT_INIT_DONE, APP_EVT_REASON_SUCCESS, NULL, 0);

    printSeparator();

    /* Initialization completed, destroy task */
    OSA_TaskDestroy((osa_task_handle_t)main_task_handle);
}

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
int main(void)
{
    BaseType_t result = 0;
    (void)result;
    BOARD_InitHardware();

    RTC_Init(RTC);

#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
    config_mbedtls_threading_alt();
#endif
#if (CONFIG_NCP_USB) && (CONFIG_WIFI_USB_FILE_ACCESS)
    usb_init();
#endif
    (void)OSA_TaskCreate((osa_task_handle_t)main_task_handle, OSA_TASK(task_main), NULL);

    OSA_Start();
    for (;;)
        ;
}

#if CONFIG_COEX_APP
void vApplicationIdleHook(void)
{
#if(CONFIG_NCP_OT)
    otSysRunIdleTask();
#endif
}
#endif
