/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_lpi2c.h"
#include "fsl_pf9453.h"
#include "app.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static status_t I2C_SendFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *txBuff, uint8_t txBuffSize);
static status_t I2C_ReceiveFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

static uint8_t DEMO_MenuSelection(void);
static void DEMO_ReadDeviceId(void);
static void DEMO_SetBuck2RunVoltage(void);
static void DEMO_ConfigBuck2(void);
static void DEMO_DumpRegisters(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static pf9453_handle_t g_pf9453Handle;
static uint8_t s_regBuffer[16];

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    lpi2c_master_config_t masterConfig;
    pf9453_config_t pmicConfig;

    BOARD_InitHardware();

    PRINTF("\r\nPF9453 PMIC driver example\r\n");

    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = DEMO_PF9453_LPI2C_BAUDRATE;
    LPI2C_MasterInit(DEMO_PF9453_LPI2C, &masterConfig, DEMO_PF9453_LPI2C_CLKSRC_FREQ);

    /* The driver has no built-in address or GetDefaultConfig: the caller supplies the I2C
     * transport and the (OTP-dependent) slave address. */
    pmicConfig.I2C_SendFunc    = I2C_SendFunc;
    pmicConfig.I2C_ReceiveFunc = I2C_ReceiveFunc;
    pmicConfig.slaveAddress    = DEMO_PF9453_I2C_ADDR;
    PF9453_CreateHandle(&g_pf9453Handle, &pmicConfig);

    while (1)
    {
        switch (DEMO_MenuSelection())
        {
            case '1':
                DEMO_ReadDeviceId();
                break;
            case '2':
                DEMO_SetBuck2RunVoltage();
                break;
            case '3':
                DEMO_ConfigBuck2();
                break;
            case '4':
                DEMO_DumpRegisters();
                break;
            default:
                break;
        }
    }
}

/* Blocking LPI2C register write with an 8-bit sub-address, matching the driver's transport type. */
static status_t I2C_SendFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *txBuff, uint8_t txBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = txBuff;
    xfer.dataSize       = txBuffSize;
    xfer.flags          = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(DEMO_PF9453_LPI2C, &xfer);
}

static status_t I2C_ReceiveFunc(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    lpi2c_master_transfer_t xfer;

    xfer.slaveAddress   = deviceAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data           = rxBuff;
    xfer.dataSize       = rxBuffSize;
    xfer.flags          = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(DEMO_PF9453_LPI2C, &xfer);
}

static uint8_t DEMO_MenuSelection(void)
{
    uint8_t item;

    PRINTF("\r\n------------------------ PF9453 Menu ------------------------\r\n");
    PRINTF("[1]. Read device ID.\r\n");
    PRINTF("[2]. Set BUCK2 (VDD_CORE) run voltage.\r\n");
    PRINTF("[3]. Configure BUCK2 (run + standby voltage, enable mode).\r\n");
    PRINTF("[4]. Dump BUCK2 registers.\r\n");

    for (;;)
    {
        item = GETCHAR();
        if ((item >= '1') && (item <= '4'))
        {
            break;
        }
        PRINTF("\r\nWrong menu item, please retry...\r\n");
    }
    return item;
}

static void DEMO_ReadDeviceId(void)
{
    uint8_t devId = 0U;
    status_t status;

    status = PF9453_ReadReg(&g_pf9453Handle, PF9453_DEV_ID, &devId);
    if (status != kStatus_Success)
    {
        PRINTF("\r\nFailed to read DEV_ID (I2C error 0x%x). Check wiring / slave address.\r\n", (uint32_t)status);
        return;
    }

    PRINTF("\r\nDEV_ID = 0x%02x", (uint32_t)devId);
    if ((devId & PF9453_CHIP_ID_MASK) == PF9453_CHIP_ID_PF9453)
    {
        PRINTF(" (PF9453 detected, CHIP_REV = %d)\r\n", (uint32_t)(devId & 0x0FU));
    }
    else
    {
        PRINTF(" (unexpected CHIP_ID nibble)\r\n");
    }
}

static void DEMO_SetBuck2RunVoltage(void)
{
    uint8_t ch;
    uint16_t mv;
    status_t status;

    PRINTF("\r\nSelect BUCK2 run voltage:\r\n");
    PRINTF("\t[a]. 0.8 V (low/normal drive)\r\n");
    PRINTF("\t[b]. 0.9 V (over drive)\r\n");
    for (;;)
    {
        ch = GETCHAR();
        if ((ch == 'a') || (ch == 'b'))
        {
            break;
        }
        PRINTF("Wrong input, please retry...\r\n");
    }
    mv = (ch == 'b') ? 900U : 800U;

    status = PF9453_SetBuck2RunVoltage(&g_pf9453Handle, mv);
    PRINTF("\r\nPF9453_SetBuck2RunVoltage(%d mV) -> 0x%x\r\n", (uint32_t)mv, (uint32_t)status);
}

static void DEMO_ConfigBuck2(void)
{
    pf9453_buck2_config_t cfg;
    status_t status;

    /* RT2660 supply: BUCK2 on at ACTIVE/STANDBY, run 0.9 V, standby 0.65 V, default DVS ramp. */
    cfg.runMillivolt     = 900U;
    cfg.standbyMillivolt = 650U;
    cfg.enableMode       = kPF9453_EnableOnActiveStandby;
    cfg.forcePwm         = false;
    cfg.activeDischarge  = false;
    cfg.dvsRamp          = kPF9453_Buck2Ramp_25mVPer2us;

    status = PF9453_ConfigBuck2(&g_pf9453Handle, &cfg);
    PRINTF("\r\nPF9453_ConfigBuck2(run=900mV, standby=650mV, on@ACTIVE/STANDBY) -> 0x%x\r\n", (uint32_t)status);
}

static void DEMO_DumpRegisters(void)
{
    status_t status;
    uint8_t i;

    /* BUCK2 control/output block: BUCK2CTRL(0x14) .. BUCK2OUT_STBY(0x1D). */
    status = PF9453_DumpReg(&g_pf9453Handle, PF9453_BUCK2CTRL, s_regBuffer, 10U);
    if (status != kStatus_Success)
    {
        PRINTF("\r\nDump failed (I2C error 0x%x).\r\n", (uint32_t)status);
        return;
    }

    PRINTF("\r\nBUCK2 register block:\r\n");
    for (i = 0U; i < 10U; i++)
    {
        PRINTF("  Reg 0x%02x = 0x%02x\r\n", (uint32_t)(PF9453_BUCK2CTRL + i), (uint32_t)s_regBuffer[i]);
    }
}
