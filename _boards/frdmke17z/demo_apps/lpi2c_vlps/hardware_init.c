/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_smc.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void APP_BootClockRUN(void)
{
    const scg_sosc_config_t g_scgSysOscConfig = {.freq        = BOARD_XTAL0_CLK_HZ,
                                                 .enableMode  = kSCG_SysOscEnable | kSCG_SysOscEnableInLowPower,
                                                 .monitorMode = kSCG_SysOscMonitorDisable,
                                                 .div2        = kSCG_AsyncClkDivBy1,
                                                 .workMode    = kSCG_SysOscModeOscLowPower};

    const scg_firc_config_t g_scgFircConfig = {
        .enableMode = kSCG_FircEnable, .div2 = kSCG_AsyncClkDivBy1, .range = kSCG_FircRange48M, .trimConfig = NULL};

    const scg_lpfll_config_t g_scgLpFllConfig = {
        .enableMode = kSCG_LpFllEnable, .div2 = kSCG_AsyncClkDivBy2, .range = kSCG_LpFllRange72M, .trimConfig = NULL};

    const scg_sys_clk_config_t g_sysClkConfigSircSource = {
        .divSlow = kSCG_SysClkDivBy4, .divCore = kSCG_SysClkDivBy1, .src = kSCG_SysClkSrcSirc};

    const scg_sys_clk_config_t g_sysClkConfigNormalRun = {
        .divSlow = kSCG_SysClkDivBy3, .divCore = kSCG_SysClkDivBy1, .src = kSCG_SysClkSrcLpFll};
    const scg_sirc_config_t scgSircConfig = {
        .enableMode = kSCG_SircEnable | kSCG_SircEnableInLowPower | kSCG_SircEnableInStop,
        .div2       = kSCG_AsyncClkDivBy2,
        .range      = kSCG_SircRangeHigh};
    scg_sys_clk_config_t curConfig;

    CLOCK_InitSysOsc(&g_scgSysOscConfig);
    CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ);

    /* Init Sirc */
    CLOCK_InitSirc(&scgSircConfig);

    /* Change to use SIRC as system clock source to prepare to change FIRCCFG register */
    CLOCK_SetRunModeSysClkConfig(&g_sysClkConfigSircSource);

    /* Wait for clock source switch finished. */
    do
    {
        CLOCK_GetCurSysClkConfig(&curConfig);
    } while (curConfig.src != g_sysClkConfigSircSource.src);

    /* Init Firc */
    CLOCK_InitFirc(&g_scgFircConfig);

    /* Init LPFLL */
    CLOCK_InitLpFll(&g_scgLpFllConfig);

    /* Use LPFLL as system clock source */
    CLOCK_SetRunModeSysClkConfig(&g_sysClkConfigNormalRun);

    /* Wait for clock source switch finished. */
    do
    {
        CLOCK_GetCurSysClkConfig(&curConfig);
    } while (curConfig.src != g_sysClkConfigNormalRun.src);

    SystemCoreClock = 72000000U;
}

/* Initialize debug console. */
void APP_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;

    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSircAsync);
    uartClkSrcFreq = CLOCK_GetIpFreq(kCLOCK_Lpuart0);

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

void BOARD_InitHardware(void)
{
    gpio_pin_config_t pin_config;
    uint32_t i;

    BOARD_InitBootPins();
    APP_BootClockRUN();
    APP_InitDebugConsole();

    /* Set LPI2C clock source, should work in stop mode */
    CLOCK_SetIpSrc(DEMO_ACCEL_I2C_CLOCK_NAME, DEMO_ACCEL_I2C_CLOCK_SOURCE);
    /* Select the SIRC 8M as LPIT clock, SIRC enabled in stop mode */
    CLOCK_SetIpSrc(kCLOCK_Lpit0, DEMO_LPIT_CLOCK_SOURCE);

    /* Reset sensor by reset pin*/
    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic  = 1;
    GPIO_PinInit(ACCEL_RESET_GPIO, ACCEL_RESET_PIN, &pin_config);
    GPIO_PinWrite(ACCEL_RESET_GPIO, ACCEL_RESET_PIN, 1);
    /* Delay to ensure reliable sensor reset */
    for (i = 0; i < SystemCoreClock / 1000U; i++)
    {
        __NOP();
    }
    GPIO_PinWrite(ACCEL_RESET_GPIO, ACCEL_RESET_PIN, 0);

    /* Delay to wait sensor stable after reset */
    for (i = 0; i < SystemCoreClock / 100U; i++)
    {
        __NOP();
    }
}
/*${function:end}*/
