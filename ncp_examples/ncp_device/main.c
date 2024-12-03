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
#if CONFIG_HOST_SLEEP
#include "host_sleep.h"
#endif

#include "ncp_adapter.h"
#include "osa.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define NCP_INBUF_SIZE     4096

#if (CONFIG_NCP_WIFI) && !(CONFIG_NCP_BLE)
#define TASK_MAIN_PRIO         configMAX_PRIORITIES - 4
#else
#define TASK_MAIN_PRIO         OSA_TASK_PRIORITY_MIN - 2
#endif
#define TASK_MAIN_STACK_SIZE   3072

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

    printSeparator();
    result = ncp_adapter_init();
    assert(NCP_SUCCESS == result);

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
    /* register ot idle function to os idle hook list */
    OSA_SetupIdleFunction(otSysRunIdleTask);
    PRINTF("OT initialized\r\n");
#endif

    result = ncp_cmd_list_init();
    assert(NCP_SUCCESS == result);

    printSeparator();
#if CONFIG_HOST_SLEEP
    hostsleep_init();
#endif

    while (1)
    {
        /* wait for interface up */
        OSA_TimeDelay(portMAX_DELAY);
    }
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

    printSeparator();
    PRINTF("NCP device demo\r\n");
    printSeparator();

#if (CONFIG_NCP_USB) && (CONFIG_WIFI_USB_FILE_ACCESS)
    usb_init();
#endif
    (void)OSA_TaskCreate((osa_task_handle_t)main_task_handle, OSA_TASK(task_main), NULL);

    OSA_Start();
    for (;;)
        ;
}
