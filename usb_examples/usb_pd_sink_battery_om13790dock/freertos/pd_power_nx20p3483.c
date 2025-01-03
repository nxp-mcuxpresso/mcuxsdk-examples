/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_pd_config.h"
#include "usb_pd.h"
#include "usb_pd_i2c.h"
#include "pd_board_config.h"
#include "pd_power_nx20p3483.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define NX20P3483_DEVICE_ID_REG_VALUE (0x09u)

#define nx20p_I2cWrite(nx20pInstance, data, dataLen) \
    PD_I2cWriteBlocking(nx20pInstance->i2cHandle, nx20pInstance->nx20pSlaveAddress, 0, 0, (data), dataLen)
#define nx20p_I2cRead(nx20pInstance, reg, regLen, data, dataLen) \
    PD_I2cReadBlocking(nx20pInstance->i2cHandle, nx20pInstance->nx20pSlaveAddress, reg, regLen, data, dataLen)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

typedef struct _pd_nx20p_instance
{
    pd_handle pdHandle;
    usb_pd_i2c_handle i2cHandle;
    uint32_t source5VOcpCurrent;
    uint32_t sinkOvpVoltage;
    uint8_t nx20pSlaveAddress;
    uint8_t initialized;
} pd_nx20p_instance_t;

uint32_t HW_I2CGetFreq(uint8_t i2cInstance);

void HW_I2CReleaseBus(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static pd_nx20p_instance_t s_nx20pInstances[PD_CONFIG_MAX_PORT];

/*******************************************************************************
 * Code
 ******************************************************************************/

static void PD_NX20PInitRegisters(uint8_t port)
{
    uint8_t txBuf[2];
    pd_nx20p_instance_t *nx20pInstance = &s_nx20pInstances[port - 1];
    /* Device control, exit dead-battery mode. Resets all registers to default values */
    nx20p_I2cWrite(nx20pInstance, (uint8_t *)"\x0B\x04", 2);

    /* Write 5V source OCP config */
    PD_NX20PSet5VSourceOCP(port, nx20pInstance->source5VOcpCurrent);

    /* Write sink OVP config */
    PD_NX20PSetSinkOVP(port, nx20pInstance->sinkOvpVoltage);

    /* Mask 5V RCP interrupt and OVP interrupt */
    nx20p_I2cWrite(nx20pInstance, (uint8_t *)"\x06\x18", 2);

    /* Disable SINK RCP interrupt. */
    nx20p_I2cRead(nx20pInstance, 0x07, 1, &txBuf[1], 1);
    txBuf[0] = 0x07u;
    txBuf[1] |= 0x40u;
    nx20p_I2cWrite(nx20pInstance, txBuf, 2);
    /* Clear interrupt */
    PD_NX20PClearInt(port);
}

static void PD_NX20PCheckInit(uint8_t port)
{
    pd_nx20p_instance_t *nx20pInstance = &s_nx20pInstances[port - 1];
    if (nx20pInstance->initialized == 0)
    {
        nx20pInstance->initialized = 1;
        PD_NX20PInitRegisters(port);
    }
}

void PD_NX20PSetSinkOVP(uint8_t port, uint16_t voltage)
{
    uint8_t txBuf[2];
    pd_nx20p_instance_t *nx20pInstance = &s_nx20pInstances[port - 1];

    if ((port == 0) || (port > PD_CONFIG_MAX_PORT))
    {
        return;
    }
    PD_NX20PCheckInit(port);

    /* Set OVLO */
    txBuf[0] = 0x08;
    if (voltage <= 6000)
        txBuf[1] = 0x00;
    else if (voltage <= 6800)
        txBuf[1] = 0x01;
    else if (voltage <= 10000)
        txBuf[1] = 0x02;
    else if (voltage <= 11500)
        txBuf[1] = 0x03;
    else if (voltage <= 14000)
        txBuf[1] = 0x04;
    else if (voltage <= 17000)
        txBuf[1] = 0x05;
    else
        txBuf[1] = 0x06;
    nx20p_I2cWrite(nx20pInstance, txBuf, 2);
}

#if 0
static uint16_t PD_NX20PGetSinkOVP(pd_nx20p_instance_t *nx20pInstance)
{
    uint8_t rxData;
    PD_NX20PCheckInit(port);

    nx20p_I2cRead(nx20pInstance, 0x08, 1, &rxData, 1);
    switch (rxData)
    {
        case 0x00:
            return 6000;
        case 0x01:
            return 6800;
        case 0x02:
            return 10000;
        case 0x03:
            return 11500;
        case 0x04:
            return 14000;
        case 0x05:
            return 17000;
        case 0x06:
            return 23000;
        default:
            return 6000;
    }
}
#endif

void PD_NX20PSet5VSourceOCP(uint8_t port, uint16_t current)
{
    uint8_t txBuf[2];
    pd_nx20p_instance_t *nx20pInstance = &s_nx20pInstances[port - 1];

    if ((port == 0) || (port > PD_CONFIG_MAX_PORT))
    {
        return;
    }
    PD_NX20PCheckInit(port);

    /* Set OCP */
    txBuf[0] = 0x0A;
    if (current <= 400)
    {
        txBuf[1] = 0x00;
    }
    else
    {
        current -= 400;
        txBuf[1] = (current + 200 - 1) / 200;
    }
    nx20p_I2cWrite(nx20pInstance, txBuf, 2);
}

#if 0
static uint16_t PD_NX20PGet5VSourceOCP(pd_nx20p_instance_t *nx20pInstance)
{
    uint8_t rxData;
    PD_NX20PCheckInit(port);

    nx20p_I2cRead(nx20pInstance, 0x0A, 1, &rxData, 1);
    return (rxData * 200) + 400;
}
#endif

void PD_NX20PClearInt(uint8_t port)
{
    volatile uint8_t rxData;
    pd_nx20p_instance_t *nx20pInstance = &s_nx20pInstances[port - 1];

    if ((port == 0) || (port > PD_CONFIG_MAX_PORT))
    {
        return;
    }
    PD_NX20PCheckInit(port);

    /* Clear interrupt */
    nx20p_I2cRead(nx20pInstance, 0x04u, 1, (uint8_t *)&rxData, 1);
    nx20p_I2cRead(nx20pInstance, 0x05u, 1, (uint8_t *)&rxData, 1);
}

void PD_NX20PInit(uint8_t port, uint32_t source5VOcpCurrent, uint32_t sinkOvpVoltage)
{
    pd_nx20p_instance_t *nx20pInstance        = &s_nx20pInstances[port - 1];
    pd_nx20p_config_t portsNX20PConfigArray[] = {
#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE)
        {
            .nx20pI2CMaster     = kInterface_i2c0 + BOARD_PD_I2C_INDEX,
            .nx20pSlaveAddress  = PD_NX20P3483_SLAVE_ADDRESS,
            .nx20pI2cSrcClock   = 0u,
            .nx20pI2cReleaseBus = (void *)HW_I2CReleaseBus,
        },
#endif
    };

    if ((port == 0) || (port > PD_CONFIG_MAX_PORT) || (port > (sizeof(s_nx20pInstances) / sizeof(pd_nx20p_instance_t))))
    {
        return;
    }

    portsNX20PConfigArray[port - 1].nx20pI2cSrcClock = HW_I2CGetFreq(portsNX20PConfigArray[port - 1].nx20pI2CMaster);
    nx20pInstance->nx20pSlaveAddress                 = portsNX20PConfigArray[port - 1].nx20pSlaveAddress;

    /* initialize I2C */
    if (PD_I2cInit(&nx20pInstance->i2cHandle, portsNX20PConfigArray[port - 1].nx20pI2CMaster,
                   portsNX20PConfigArray[port - 1].nx20pI2cSrcClock,
                   (PD_I2cReleaseBus)portsNX20PConfigArray[port - 1].nx20pI2cReleaseBus) != kStatus_PD_Success)
    {
        return;
    }
    if (nx20pInstance->i2cHandle == NULL)
    {
        return;
    }
    nx20pInstance->initialized        = 0;
    nx20pInstance->source5VOcpCurrent = source5VOcpCurrent;
    nx20pInstance->sinkOvpVoltage     = sinkOvpVoltage;
}

void PD_NX20PDeinit(uint8_t port)
{
    pd_nx20p_instance_t *nx20pInstance = &s_nx20pInstances[port - 1];

    if ((port == 0) || (port > PD_CONFIG_MAX_PORT) || (port > (sizeof(s_nx20pInstances) / sizeof(pd_nx20p_instance_t))))
    {
        return;
    }

    PD_I2cDeinit(nx20pInstance->i2cHandle);
    nx20pInstance->i2cHandle = NULL;
    return;
}

void PD_NX20PExitDeadBatteryMode(uint8_t port)
{
    pd_nx20p_instance_t *nx20pInstance = &s_nx20pInstances[port - 1];
    nx20pInstance->initialized         = 1;
    PD_NX20PInitRegisters(port);
}
