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

    power_init_config_t boardPowerInitCfg;
    POWER_GetDefaultInitConfig(&boardPowerInitCfg);
    boardPowerInitCfg.handshakeRouting = &s_handshakeRoutingCfg;
    POWER_Init(&boardPowerInitCfg);
}
/*${function:end}*/
