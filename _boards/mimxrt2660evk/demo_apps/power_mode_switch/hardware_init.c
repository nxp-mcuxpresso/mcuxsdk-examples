/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_vbatcon.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
/*
 * Bring the VBAT retention SRAM (VBAT__SRAM) up to ACTIVE so the application can
 * use it and it is retained across Deep Power Down 1.  Enable read/write access,
 * then follow the RM power-up sequence SHUTDOWN (SD) -> SHUTDOWN_POWER_UP (SD PU)
 * -> ACTIVE (ACT): PWR_MODE (CTRL[5:2]) must not jump straight to ACTIVE.  Lock
 * bits (CTRL[26:16]) are left clear so the mode stays changeable.
 * [hw-defer] pending silicon confirmation of the VBAT SRAM power sequence.
 */
void BOARD_InitVbatSram(void)
{
    uint32_t ctrl;
    VBAT__SRAM->CTRL |= 0x3U; /* RAM_RD_EN (bit 0) | RAM_WR_EN (bit 1) */
    ctrl = VBAT__SRAM->CTRL & ~(uint32_t)SRAM_CTRL_PWR_MODE_MASK; /* SHUTDOWN (SD = 0) */
    VBAT__SRAM->CTRL = ctrl;
    VBAT__SRAM->CTRL = ctrl | SRAM_CTRL_PWR_MODE(4U); /* SHUTDOWN_POWER_UP (SD PU) */
    VBAT__SRAM->CTRL = ctrl | SRAM_CTRL_PWR_MODE(6U); /* ACTIVE (ACT)              */
}

/*
 * VBATCON GPR[0] / GPR[1] boot-context markers (see app.h). VBATCON_GPR_COUNT
 * on this device is 2, so both markers fit in the array; VBATCON_Init/Deinit
 * never touch GPRs, and the VBAT domain is always powered, so both survive
 * every Power Down / Deep Power Down PoR.
 */
#define BOARD_VBAT_GPR_DPD_VARIANT   0U
#define BOARD_VBAT_GPR_ENABLED_WAKEUP 1U

void BOARD_SetDpdVariantMarker(uint32_t variant)
{
    VBATCON_WriteGPR(VBAT__VBATCON, BOARD_VBAT_GPR_DPD_VARIANT, variant);
}

uint32_t BOARD_GetAndClearDpdVariantMarker(void)
{
    uint32_t variant = VBATCON_ReadGPR(VBAT__VBATCON, BOARD_VBAT_GPR_DPD_VARIANT);
    VBATCON_WriteGPR(VBAT__VBATCON, BOARD_VBAT_GPR_DPD_VARIANT, BOARD_DPD_VARIANT_DPD2);
    return variant;
}

void BOARD_SetEnabledWakeupMarker(uint8_t wakeupSrc)
{
    VBATCON_WriteGPR(VBAT__VBATCON, BOARD_VBAT_GPR_ENABLED_WAKEUP, (uint32_t)wakeupSrc);
}

uint8_t BOARD_GetAndClearEnabledWakeupMarker(void)
{
    uint32_t wakeupSrc = VBATCON_ReadGPR(VBAT__VBATCON, BOARD_VBAT_GPR_ENABLED_WAKEUP);
    VBATCON_WriteGPR(VBAT__VBATCON, BOARD_VBAT_GPR_ENABLED_WAKEUP, 0xFFU);
    return (uint8_t)wakeupSrc;
}

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree (incl. the boot ramp to HP run mode), TRDC, and
     * Debug Console init. When BOARD_USE_PF9453 is set, BOARD_CommonSetting() also calls
     * BOARD_InitPmicSupply() as its first action, before the clock ramp — PF9453 must be up
     * before that ramp. No PMIC-specific code is needed here. */
    BOARD_CommonSetting();
    BOARD_ResetMPU();

    IOMUXC_SetPin_Mux_Config(
     IOMUXC_PIO0_4_VBAT_GPIO0_GPIO4,
      0xD0U);

    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO1_0_WAKE_GPIO0_GPIO0, 0xA0U);

    /* Configure both wakeup buttons as GPIO inputs (SW5 = VBAT pin 4,
     * SW6 = WAKE pin 0).  GPIO_PinInit also enables the WAKE GPIO port clock. */
    gpio_pin_config_t btnConfig = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0U,
    };
    GPIO_PinInit(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &btnConfig);
    GPIO_PinInit(BOARD_USER_BUTTON_6_GPIO, BOARD_USER_BUTTON_6_GPIO_PIN, &btnConfig);

    /* BOARD_CommonSetting() already called POWER_SetPolicy() with POR-default handshake
     * routing (NULL). Override with this demo's custom routing -- a standalone
     * POWER_SetHandshakeRouting() call is enough; no need to re-run POWER_SetPolicy(). */
    POWER_SetHandshakeRouting(&s_handshakeRoutingCfg);
}
/*${function:end}*/
