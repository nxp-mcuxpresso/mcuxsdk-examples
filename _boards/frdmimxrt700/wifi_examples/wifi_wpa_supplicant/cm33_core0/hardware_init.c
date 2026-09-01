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
#include "fsl_cache.h"
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

/* Minimal PMIC init for Deep Sleep, mirroring the verified single-core
 * usb_lpm_device_hid_mouse example on FRDM-IMXRT700. */
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

    /* Enable TRNG clock required by ELS PKC PSA crypto driver */
    GlikeyWriteEnable(GLIKEY3, 1U);                                     /* Enable SYSCON0_SEC_CLK_CTRL write */
    SYSCON0->SEC_CLK_CTRL |= SYSCON0_SEC_CLK_CTRL_TRNG_REFCLK_EN_MASK; /* Enable TRNG REF CLOCK */
    CLOCK_AttachClk(kFRO1_DIV2_to_TRNG);                                /* Max 96MHz with 1.0V normal supply */
    CLOCK_SetClkDiv(kCLOCK_DivTrngClk, 1U);

    /* Disable XCACHEs for ELS PKC DMA operations */
    XCACHE_DisableCache(XCACHE0);
    XCACHE_DisableCache(XCACHE1);

    __DSB();
    __ISB();

#if CONFIG_HOST_SLEEP
    /* Mux the SW5 button pad and initialize the PMIC for Wi-Fi host-sleep Deep Sleep. */
    BOARD_InitBUTTONsPins();
    BOARD_PowerInitPMIC();
#endif
}
/*${function:end}*/
