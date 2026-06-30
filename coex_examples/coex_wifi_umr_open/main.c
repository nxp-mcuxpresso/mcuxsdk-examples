/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Coex Wi-Fi + BLE LE Audio Application (Edgefast Open)
 *
 * Architecture:
 *   This app combines the edgefast_open unicast_media_receiver BT app with Wi-Fi CLI.
 *   - Wi-Fi and BT use combo firmware (CONFIG_BT_BYPASS_DOWNLOAD=y) loaded
 *     together over SDIO. This is required for edgefast_open coex — separate
 *     BT UART download causes SDIO bus conflicts.
 *   - The BT task (unicast_media_receiver_task) creates its own shell via
 *     le_audio_shell_init() with prompt "UMR>>".
 *   - Since le_audio_shell_init() is called inside the BT task and we cannot
 *     modify it, we register the "wifi" command on the same shell externally.
 *     The shell handle (s_shellHandle) is a global variable in le_audio_shell.c,
 *     so we access it via extern and poll until it becomes non-NULL.
 *   - This gives a single shell prompt (UMR>>) with both BT commands and
 *     Wi-Fi commands (wifi wlan-scan, wifi wlan-version).
 *
 * CAAM DMA Workaround:
 *   UMR calls bt_csip_set_member_generate_rsi() which uses bt_encrypt_le().
 *   In coex mode, CAAM hardware DMA corrupts BT UART DMA. We use linker
 *   --wrap=bt_encrypt_le to redirect crypto through HCI_LE_Encrypt command.
 *   See coex_bt_encrypt.c for the implementation.
 *
 * Flow:
 *   main() -> task_main -> coex_cli_init() + coex_controller_init() [Wi-Fi init]
 *          -> xTaskCreate(unicast_media_receiver_task) [BT task]
 *          -> poll s_shellHandle -> SHELL_RegisterCommand(wifi) -> vTaskDelete
 *
 * Reference apps:
 *   - middleware/edgefast_open/examples/wifi_cli_over_ble_wu (edgefast_open + Wi-Fi coex pattern)
 *   - middleware/edgefast_open/examples/unicast_media_receiver (BT LE Audio UMR)
 *   - examples/coex_examples/coex_wifi_umr_4cis_open (4CIS coex reference)
 */

#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "task.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_adapter_uart.h"
#include "FreeRTOS.h"

#include "wifi.h"
#include "fsl_sdmmc_host.h"
#include "app.h"
#include "fsl_device_registers.h"
#include "fsl_iomuxc.h"
#include "fsl_lpuart_edma.h"
#include "fsl_dmamux.h"

#include "fsl_shell.h"
#include "cli.h"
#include "coex_cli.h"
#include "unicast_media_receiver.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_InitHardware(void);
extern void coex_controller_init(void);

/*
 * s_shellHandle is defined as a global (non-static) variable in le_audio_shell.c.
 * The BT task calls le_audio_shell_init() which creates the shell and sets this handle.
 * We extern it here to register our "wifi" command on the same shell, giving users
 * access to both BT and Wi-Fi commands from a single prompt.
 */
extern shell_handle_t s_shellHandle;

static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const int TASK_MAIN_PRIO       = (configMAX_PRIORITIES - 5);
const int TASK_MAIN_STACK_SIZE = (2 * 1024);
TaskHandle_t task_main_handle;

SHELL_COMMAND_DEFINE(wifi,
                     "\r\n\"wifi\": wifi related function\r\n"
                     "  USAGE: wifi <wlan-command> [args]\r\n",
                     cmd_wifi,
                     SHELL_IGNORE_PARAMETER_COUNT);

/*******************************************************************************
 * Code
 ******************************************************************************/
static void printSeparator(void)
{
    PRINTF("========================================\r\n");
}

/**
 * @brief Wi-Fi shell command handler.
 *
 * Looks up the WLAN CLI command by name and executes it.
 * Usage from shell: wifi wlan-scan, wifi wlan-version, wifi wlan-connect <profile>, etc.
 */
static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    const struct cli_command *command = NULL;

    if (argc < 2)
    {
        PRINTF("Missing wifi subcommand\r\n");
        return kStatus_SHELL_Success;
    }

    /* Shift argv to skip "wifi" prefix */
    argc--;
    argv++;

    /* lookup_command() searches the registered WLAN CLI commands by name */
    command = lookup_command(argv[0], strlen(argv[0]));
    if (command != NULL)
    {
        command->function(argc, argv);
        PRINTF("Command %s executed\r\n", command->name);
    }
    else
    {
        PRINTF("Unknown wifi subcommand: %s\r\n", argv[0]);
    }

    return kStatus_SHELL_Success;
}

void task_main(void *param)
{
#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
    PRINTF("     Initialize Firecrest-2EL (IW612) M2 Module\r\n");
#elif defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
    PRINTF("     Initialize RB3P 1XK Direct-M2 Module\n");
#elif defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2)
    PRINTF("     Initialize CA2 1ZM Direct-M2 Module\r\n");
#endif
    printSeparator();

    /* Initialize Wi-Fi CLI commands and download combo firmware (BT+Wi-Fi) over SDIO */
    coex_cli_init();
    coex_controller_init();
    PRINTF("host init done\r\n");

    /* Wait for Wi-Fi interface to come up before starting BT task */
    vTaskDelay(3000);

    /* Create the BT LE Audio task — this will call le_audio_shell_init() internally */
    if (xTaskCreate(unicast_media_receiver_task, "unicast_media_receiver_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 3, NULL) != pdPASS)
    {
        PRINTF("unicast_media_receiver_task creation failed!\r\n");
        while (1)
            ;
    }

    /*
     * Register "wifi" command on the BT shell.
     *
     * le_audio_shell_init() is called early in the BT task (before BT init),
     * so s_shellHandle becomes non-NULL quickly. We poll until it's ready
     * rather than using a fixed delay, making this robust regardless of
     * BT initialization timing.
     */
    while (s_shellHandle == NULL)
    {
        vTaskDelay(100);
    }
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(wifi));
    PRINTF("\r\nWiFi command registered. Use: wifi <wlan-command>\r\n");

    /* task_main's job is done — BT task and shell handle the rest */
    vTaskDelete(NULL);
}

/*
 * Weak stub: vTaskListTasks is a newer FreeRTOS API (v10.6+) that may not be
 * available in this FreeRTOS version. The wifi_nxp osa_cli.c calls it, so we
 * provide a no-op stub to satisfy the linker.
 */
__attribute__((weak)) void vTaskListTasks(char *pcWriteBuffer, size_t uxBufferLength)
{
    (void)pcWriteBuffer;
    (void)uxBufferLength;
}

int main(void)
{
    BOARD_InitHardware();

    printSeparator();
    PRINTF("        Coex Wi-Fi UMR Open APP\r\n");
    printSeparator();

    BaseType_t result =
        xTaskCreate(task_main, "main", TASK_MAIN_STACK_SIZE, NULL, TASK_MAIN_PRIO, &task_main_handle);
    assert(pdPASS == result);
    (void)result;

    vTaskStartScheduler();
    for (;;)
        ;
}

#if CONFIG_COEX_APP
void vApplicationIdleHook(void)
{
}
#endif

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    PRINTF("ERROR: stack overflow on task %s.\r\n", pcTaskName);
    portDISABLE_INTERRUPTS();
    (void)xTask;
    (void)pcTaskName;
    for (;;)
        ;
}

#ifndef __GNUC__
void __assert_func(const char *file, int line, const char *func, const char *failedExpr)
{
    PRINTF("ASSERT ERROR \"%s\": file \"%s\" Line \"%d\" function name \"%s\"\n", failedExpr, file, line, func);
    for (;;)
    {
        __BKPT(0);
    }
}
#endif
