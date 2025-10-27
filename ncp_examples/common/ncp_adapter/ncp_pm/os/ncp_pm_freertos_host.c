/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "fsl_gpio.h"
#include "fsl_adapter_gpio.h"
#include "board.h"
#include "FreeRTOS.h"
#include "fsl_component_serial_manager.h"
#include "ncp_crc.h"
#include "ncp_intf_pm.h"
#include "ncp_pm_os.h"
#include "ncp_pm.h"
#include "ncp_cmd_common.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

GPIO_HANDLE_DEFINE(host_wakeup_handle);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void wakeup_int_callback(void *param)
{
    ncp_pm_cfg_t *power_cfg = ncp_pm_get_config();

    if(power_cfg->wakeup_host)
    {
        PRINTF("Wakeup host sucessfully\r\n");
        power_cfg->wakeup_host = 0;
    }
}

static void ncp_pm_host_gpio_init(void)
{
    /* Define the init structure for the input/output switch pin */
    gpio_pin_config_t gpio_in_config = {
        .direction = kGPIO_DigitalInput,
        .outputLogic = 0,
        .interruptMode = kGPIO_IntRisingEdge
    };
    gpio_pin_config_t gpio_out_config = {
        .direction = kGPIO_DigitalOutput,
        .outputLogic = 1,
        .interruptMode = kGPIO_NoIntmode
    };
    /* Init input GPIO for wakeup MCU host */
    GPIO_PinInit(GPIO1, 26, &gpio_in_config);
    /* Init output GPIO for wakeup NCP device */
    GPIO_PinInit(GPIO1, 27, &gpio_out_config);
    hal_gpio_pin_config_t wakeup_config = {kHAL_GpioDirectionIn, 0, 1, 26};
    HAL_GpioInit(host_wakeup_handle, &wakeup_config);
    HAL_GpioSetTriggerMode(host_wakeup_handle, kHAL_GpioInterruptRisingEdge);
    HAL_GpioInstallCallback(host_wakeup_handle, wakeup_int_callback, NULL);
}

void ncp_pm_gpio_wakeup_peer(void)
{
    GPIO_PinWrite(GPIO1, 27, 0);
    ncp_pm_delay_us(1000);
    GPIO_PinWrite(GPIO1, 27, 1);
}

int ncp_pm_os_init(void)
{
    ncp_pm_host_gpio_init();

    ncp_intf_pm_init();

    return NCP_PM_STATUS_SUCCESS;
}

void ncp_pm_os_deinit(void)
{
}

void ncp_pm_init_wakeup_source(void *ws, uint32_t wsId, bool enable)
{
}

void ncp_pm_enable_wakeup_source(void *ws)
{
}

void ncp_pm_disable_wakeup_source(void *ws)
{
}

void ncp_pm_get_wakeup_source(void *ws)
{
}

void ncp_pm_os_activate_lp_timer(uint32_t duration_ms)
{
}