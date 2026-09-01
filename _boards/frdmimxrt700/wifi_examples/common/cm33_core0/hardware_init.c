/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_gpio.h"
#include "app.h"
#if CONFIG_HOST_SLEEP
#include "fsl_pca9422.h"
#include "pmic_support.h"
#endif
/*${header:end}*/

/*${function:start}*/
#if CONFIG_HOST_SLEEP
/* Configure regulator output enable in Run mode (PMIC defaults). */
static void BOARD_ConfigPMICRegEnable(pca9422_handle_t *handle)
{
    pca9422_regulatoren_t cfg;

    PCA9422_GetDefaultRegEnableConfig(&cfg);
    PCA9422_WriteRegEnableConfig(handle, cfg);
}

static void BOARD_ConfigPMICEnMode(pca9422_handle_t *handle)
{
    pca9422_enmodecfg_t cfg;

    PCA9422_GetDefaultEnModeConfig(&cfg);
    PCA9422_WriteEnModeConfig(handle, cfg);
}

/* Minimal PMIC init for Deep Sleep on FRDM-IMXRT700: initialize the PCA9422 and
 * program the DEFAULT power-mode voltage configs. */
static void BOARD_PowerInitPMIC(void)
{
    pca9422_modecfg_t pca9422ModeCfg[12];
    uint32_t i;

    BOARD_InitPmic();
    for (i = 0; i < ARRAY_SIZE(pca9422ModeCfg); i++)
    {
        PCA9422_GetDefaultPowerModeConfig(&pca9422ModeCfg[i]);
    }
    for (i = 0; i < ARRAY_SIZE(pca9422ModeCfg); i++)
    {
        PCA9422_WritePowerModeConfigs(&pca9422Handle, (pca9422_power_mode_t)i, pca9422ModeCfg[i]);
    }
    BOARD_ConfigPMICRegEnable(&pca9422Handle);
    BOARD_ConfigPMICEnMode(&pca9422Handle);

    /* Select a DVS mode before the VDD1/VDD2 setpoints are used. */
    BOARD_SetPmicDVSPinStatus(0x1);
}
#endif /* CONFIG_HOST_SLEEP */

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitWIFIPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    BOARD_InitAHBSC();

#if CONFIG_HOST_SLEEP
    /* Configure the SW5 button pad (PIO0_10) as GPIO input with input buffer
     * enabled. This mirrors power_mode_switch, which relies on
     * BOARD_InitPins() having muxed PIO0_10 with INBUF_EN. */
    BOARD_InitBUTTONsPins();

    /* Initialize the PMIC so the SoC has valid rails in Deep Sleep for the
     * Wi-Fi host-sleep feature. Single-core (cm33_core0 / Compute domain). */
    BOARD_PowerInitPMIC();
#endif
}
/*${function:end}*/
