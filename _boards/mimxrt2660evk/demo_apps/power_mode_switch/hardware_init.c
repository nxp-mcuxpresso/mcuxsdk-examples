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
#include "app.h" /* extern s_powerInitCfg / s_topologyCfg + BOARD_InitHardware proto. */
#if MCUX_POWER_PF9453_SUPPLY
#include "fsl_lpi2c.h" /* LPI2C1 transport for the PF9453 VDD_CORE supply (board glue below). */
#endif
/*${header:end}*/

#if MCUX_POWER_PF9453_SUPPLY
/* PF9453 PMIC is on HSP LPI2C1 (SDA = PIO2_24, SCL = PIO2_25) on the MIMXRT2660-EVK, 7-bit slave
 * address 0x32 (default OTP). The board owns only the I2C transport; fsl_power owns the PF9453
 * handle and all register logic. */
#define BOARD_PF9453_LPI2C          HSP__LPI2C_1
#define BOARD_PF9453_LPI2C_CLK_FREQ (CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpi2c1_fclk))
#define BOARD_PF9453_LPI2C_BAUDRATE (100000U)
#define BOARD_PF9453_I2C_ADDR       (0x32U)
#endif

/*${function:start}*/
/*
 * Bring the VBAT retention SRAM (VBAT__SRAM) up to ACTIVE so the application can
 * use it and it is retained across Deep Power Down 1.  Enable read/write access,
 * then follow the RM power-up sequence SHUTDOWN (SD) -> SHUTDOWN_POWER_UP (SD PU)
 * -> ACTIVE (ACT): PWR_MODE (CTRL[5:2]) must not jump straight to ACTIVE.  Lock
 * bits (CTRL[26:16]) are left clear so the mode stays changeable.
 * [hw-defer] pending silicon confirmation of the VBAT SRAM power sequence.
 */
static void BOARD_InitVbatSram(void)
{
    uint32_t ctrl;
    VBAT__SRAM->CTRL |= 0x3U; /* RAM_RD_EN (bit 0) | RAM_WR_EN (bit 1) */
    ctrl = VBAT__SRAM->CTRL & ~(uint32_t)SRAM_CTRL_PWR_MODE_MASK; /* SHUTDOWN (SD = 0) */
    VBAT__SRAM->CTRL = ctrl;
    VBAT__SRAM->CTRL = ctrl | SRAM_CTRL_PWR_MODE(4U); /* SHUTDOWN_POWER_UP (SD PU) */
    VBAT__SRAM->CTRL = ctrl | SRAM_CTRL_PWR_MODE(6U); /* ACTIVE (ACT)              */
}

#if MCUX_POWER_PF9453_SUPPLY
/* Mux PIO2_24/PIO2_25 to HSP LPI2C1 and initialise the LPI2C master (100 kHz). */
static void BOARD_PmicI2cInit(void)
{
    lpi2c_master_config_t masterConfig;
    uint32_t srcClock_Hz = BOARD_PF9453_LPI2C_CLK_FREQ;

    /* The LPI2C1 clock root must be configured by BOARD_CommonSetting() before this runs. A zero
     * source clock would give a bad baud divider and a silently dead PMIC bus, so fail loudly. */
    assert(srcClock_Hz != 0U);

    BOARD_InitI2CPins();

    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = BOARD_PF9453_LPI2C_BAUDRATE;
    LPI2C_MasterInit(BOARD_PF9453_LPI2C, &masterConfig, srcClock_Hz);
}

/* Blocking I2C register write, matching the PF9453 driver's transport callback contract. */
static status_t BOARD_Pmic_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize,
                                    uint8_t *txBuff, uint8_t txBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;
    xfer.flags          = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(BOARD_PF9453_LPI2C, &xfer);
}

/* Blocking I2C register read, matching the PF9453 driver's transport callback contract. */
static status_t BOARD_Pmic_I2C_Receive(uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize,
                                       uint8_t *rxBuff, uint8_t rxBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;
    xfer.flags          = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(BOARD_PF9453_LPI2C, &xfer);
}
#endif /* MCUX_POWER_PF9453_SUPPLY */

void BOARD_InitHardware(void)
{
#if MCUX_POWER_PF9453_SUPPLY
    /*
     * Route VDD_CORE to the on-board PF9453 PMIC (BUCK2). This MUST happen before the boot clocks
     * (BOARD_CommonSetting -> BOARD_InitBootClocks -> POWER_EnterHpRun) ramp the core to HP, so BUCK2
     * is already at 0.9 V for the ramp. Open the LPI2C1 transport (the LPI2C1 root is at its reset
     * default here; I2C tolerates the baud drift once the boot clocks reprogram the root) and hand the
     * wrappers to POWER_InitExtSupply, which creates the PF9453 handle and does the BUCK2 bring-up.
     * Matches the validated reference (PMIC up before the clock ramp).
     */
    {
        power_ext_supply_config_t extCfg;
        BOARD_PmicI2cInit();
        extCfg.I2C_SendFunc    = BOARD_Pmic_I2C_Send;
        extCfg.I2C_ReceiveFunc = BOARD_Pmic_I2C_Receive;
        extCfg.slaveAddress    = BOARD_PF9453_I2C_ADDR;
        POWER_InitExtSupply(&extCfg);
    }
#endif

    /* Board common setting: MPU, Power and Clock Tree (incl. the boot ramp to HP run mode), TRDC, and
     * Debug Console init. */
    BOARD_CommonSetting();
    BOARD_ResetMPU();
    /*
     * Mux the two wakeup-button pads to their GPIO functions with input buffers
     * enabled: PIO0_4 -> VBAT_GPIO (SW5, 250k pulldown) and PIO1_0 -> WAKE_GPIO
     * (SW6).  Without this the GPIO peripheral never sees the button edge, so no
     * wakeup interrupt fires in any mode.
     */
    BOARD_InitBUTTONsPins();
    /*
     * SW6 (PIO1_0 = WAKE_SS_BTN) is active-low: per the board schematic SW6 is
     * SPST-NO and shorts the pad to GND on press, and its external pull-up R49
     * (100K) is DNP (unpopulated).  An MCU internal pull-up is therefore
     * mandatory.  The pin tool leaves this pad with no pull (0x80), so override
     * it with a pullup (0xA0): idle 1, press 0 -> falling edge, caught by the
     * either-edge arm.  (SW5 = PIO0_4 is the opposite: active-high, 0xD0 pulldown.)
     */
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO1_0_WAKE_GPIO0_GPIO0, 0xA0U);

    /* Configure both wakeup buttons as GPIO inputs (SW5 = VBAT pin 4,
     * SW6 = WAKE pin 0).  GPIO_PinInit also enables the WAKE GPIO port clock. */
    gpio_pin_config_t btnConfig = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0U,
    };
    GPIO_PinInit(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &btnConfig);
    GPIO_PinInit(BOARD_USER_BUTTON_6_GPIO, BOARD_USER_BUTTON_6_GPIO_PIN, &btnConfig);

    /* Power up the VBAT retention SRAM to ACTIVE (usable + retained across DPD1). */
    BOARD_InitVbatSram();

    /* TODO: 0x50008810 is in the MAIN MODCON region (base 0x50000000); its exact
     * function is unidentified.  Left as-is from earlier bring-up - confirm
     * against the RM and remove/replace with a named register if not required. */
    *(volatile uint32_t *)0x50008810u = 0x1;

    /*
     * Initialise the power driver from the shared-example config literals (s_powerInitCfg /
     * s_topologyCfg, defined in power_mode_switch.h). Done as the final board bring-up step so the
     * clock tree, pins, and (when enabled) the PMIC transport are all up first. (The PMIC handle
     * itself was already created by POWER_InitExtSupply above; POWER_Init does the POWERCON/PDCON/
     * topology setup.) To customise for a non-reference board, edit those literals in
     * power_mode_switch.h.
     */
    POWER_Init(&s_powerInitCfg);
}
/*${function:end}*/
