/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_DATA_LENGTH            32U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void I2C_MASTER_IRQHandler(void);
void I2C_SLAVE_IRQHandler(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint8_t g_slave_buff[I2C_DATA_LENGTH];
volatile uint8_t g_master_buff[I2C_DATA_LENGTH];
volatile uint8_t g_masterTxIndex = 0;
volatile uint8_t g_masterRxIndex = 0xFFU;
volatile uint8_t g_slaveTxIndex  = 0xFFU;
volatile uint8_t g_slaveRxIndex  = 0;
volatile bool g_masterReadBegin  = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

#pragma interrupt alignsp saveall
void I2C_MASTER_IRQHandler(void)
{
    uint8_t tmpdata;
    /* Clear pending flag. */
    I2C_MasterClearStatusFlags(EXAMPLE_I2C_MASTER_BASEADDR, (uint16_t)kI2C_InterruptPendingFlag);

    if (g_masterReadBegin)
    {
        /* Change direction to read direction and automatically send ACK. */
        I2C_SetTransferDirection(EXAMPLE_I2C_MASTER_BASEADDR, kI2C_Read);

        /* Read dummy to free the bus. */
        (void)I2C_ReadByte(EXAMPLE_I2C_MASTER_BASEADDR);

        g_masterReadBegin = false;

        return;
    }

    /* If tx Index < I2C_DATA_LENGTH, master send->slave receive transfer is ongoing. */
    if (g_masterTxIndex < I2C_DATA_LENGTH)
    {
        I2C_WriteByte(EXAMPLE_I2C_MASTER_BASEADDR, g_master_buff[g_masterTxIndex]);
        g_masterTxIndex++;
    }

    /* If rx Index < I2C_DATA_LENGTH, master receive->slave send transfer is ongoing. */
    if (g_masterRxIndex < I2C_DATA_LENGTH)
    {
        /* Send STOP after receiving the last byte. */
        if (g_masterRxIndex == (I2C_DATA_LENGTH - 1U))
        {
            I2C_MasterStop(EXAMPLE_I2C_MASTER_BASEADDR);
        }

        tmpdata                        = I2C_ReadByte(EXAMPLE_I2C_MASTER_BASEADDR);
        g_master_buff[g_masterRxIndex] = tmpdata;
        g_masterRxIndex++;

        /* Send NAK at the last byte. */
        if (g_masterRxIndex == (I2C_DATA_LENGTH - 1U))
        {
            I2C_SendAck(EXAMPLE_I2C_MASTER_BASEADDR, false);
        }
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
    SDK_ISR_EXIT_BARRIER;
}
#pragma interrupt off

#pragma interrupt alignsp saveall
void I2C_SLAVE_IRQHandler(void)
{
    uint8_t tmpdata;
    uint16_t status = I2C_SlaveGetStatusFlags(EXAMPLE_I2C_SLAVE_BASEADDR);

    /* Clear pending flag. */
    I2C_MasterClearStatusFlags(EXAMPLE_I2C_SLAVE_BASEADDR, (uint16_t)kI2C_InterruptPendingFlag);

    if (status & kI2C_AddressAsSlaveInterruptFlag)
    {
        /* Slave transmit, master reading from slave. */
        if (status & kI2C_SlaveTransmitFlag)
        {
            /* Change direction to send data. */
            I2C_SetTransferDirection(EXAMPLE_I2C_SLAVE_BASEADDR, kI2C_Write);

            /* Start to send data in tx buffer. */
            g_slaveTxIndex = 0;
        }
        else
        {
            /* Slave receive, master writing to slave. */
            I2C_SetTransferDirection(EXAMPLE_I2C_SLAVE_BASEADDR, kI2C_Read);

            /* Read dummy to free the bus. */
            (void)I2C_ReadByte(EXAMPLE_I2C_SLAVE_BASEADDR);

            return;
        }
    }

    if (g_slaveTxIndex == I2C_DATA_LENGTH)
    {
        /* Change to RX mode when send out all data in tx buffer. */
        I2C_SetTransferDirection(EXAMPLE_I2C_SLAVE_BASEADDR, kI2C_Read);

        /* Read dummy to release bus. */
        (void)I2C_ReadByte(EXAMPLE_I2C_SLAVE_BASEADDR);
    }

    /* If tx Index < I2C_DATA_LENGTH, master receive->slave send transfer is ongoing. */
    if (g_slaveTxIndex < I2C_DATA_LENGTH)
    {
        I2C_WriteByte(EXAMPLE_I2C_SLAVE_BASEADDR, g_slave_buff[g_slaveTxIndex]);
        g_slaveTxIndex++;
    }

    /* If rx Index < I2C_DATA_LENGTH, slave receive->master send transfer is ongoing. */
    if (g_slaveRxIndex < I2C_DATA_LENGTH)
    {
        /* Send NAK at the last byte. */
        if (g_slaveRxIndex == (I2C_DATA_LENGTH - 1U))
        {
            I2C_SendAck(EXAMPLE_I2C_SLAVE_BASEADDR, false);
        }

        tmpdata                      = I2C_ReadByte(EXAMPLE_I2C_SLAVE_BASEADDR);
        g_slave_buff[g_slaveRxIndex] = tmpdata;
        g_slaveRxIndex++;
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
    SDK_ISR_EXIT_BARRIER;
}
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t tmpmasterdata;
    uint8_t tmpslavedata;
    i2c_slave_config_t slaveConfig;

    i2c_master_config_t masterConfig;

    BOARD_InitHardware();

    PRINTF("\r\nI2C example -- MasterFunctionalInterrupt_SlaveFunctionalInterrupt.\r\n");

/* Only support CORTEX-M architecture */
#if defined __CORTEX_M
    /*  Enable master and slave NVIC interrupt. */
    EnableIRQ(I2C_MASTER_IRQ);
    EnableIRQ(I2C_SLAVE_IRQ);

    /* Set i2c slave interrupt priority higher. */
    NVIC_SetPriority(I2C_SLAVE_IRQ, 0);
    NVIC_SetPriority(I2C_MASTER_IRQ, 1);
#else
    /* Enable master and slave INTC interrupt.
     * Set i2c slave interrupt priority higher. */
    EnableIRQWithPriority(I2C_SLAVE_IRQ, 2U);
    EnableIRQWithPriority(I2C_MASTER_IRQ, 1U);
#endif
    /*1.Set up i2c slave first*/
    /*
     * slaveConfig.eAddressingMode = kI2C_Address7bit;
     * slaveConfig.bEnableGeneralCall = false;
     * slaveConfig.bEnableWakeUp = false;
     * slaveConfig.bEnableModule = true;
     */
    I2C_SlaveGetDefaultConfig(&slaveConfig, I2C_MASTER_SLAVE_ADDR_7BIT, I2C_SLAVE_CLK_FREQ);
    slaveConfig.bEnableModule = true;
    I2C_SlaveInit(EXAMPLE_I2C_SLAVE_BASEADDR, &slaveConfig);

    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
    {
        g_slave_buff[i] = 0;
    }

    /*2.Set up i2c master to send data to slave*/
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
    {
        g_master_buff[i] = i;
    }

    PRINTF("Master will send data :");
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /*
     * masterConfig.u32BaudRateBps = 100000U;
     * masterConfig.bEnableStopHold = false;
     * masterConfig.u16GlitchFilterWidth = 20ns;
     * masterConfig.bEnableModule = true;
     */
    I2C_MasterGetDefaultConfig(&masterConfig, I2C_MASTER_CLK_FREQ);
    masterConfig.bEnableModule = true;
    I2C_MasterInit(EXAMPLE_I2C_MASTER_BASEADDR, &masterConfig);

    /* Master send address to slave. */
    I2C_MasterStart(EXAMPLE_I2C_MASTER_BASEADDR, I2C_MASTER_SLAVE_ADDR_7BIT, kI2C_MasterTransmit);

    /* Enable module interrupt. */
    I2C_EnableInterrupts(EXAMPLE_I2C_MASTER_BASEADDR, kI2C_GlobalInterruptEnable);
    I2C_EnableInterrupts(EXAMPLE_I2C_SLAVE_BASEADDR, kI2C_GlobalInterruptEnable);

    /* Wait slave receive finished. */
    while (g_slaveRxIndex < I2C_DATA_LENGTH)
    {
    }

    /* Disable module interrupt. */
    I2C_DisableInterrupts(EXAMPLE_I2C_MASTER_BASEADDR, kI2C_GlobalInterruptEnable);
    I2C_DisableInterrupts(EXAMPLE_I2C_SLAVE_BASEADDR, kI2C_GlobalInterruptEnable);

    /* Master send stop command. */
    I2C_MasterStop(EXAMPLE_I2C_MASTER_BASEADDR);

    /*3.Transfer completed. Check the data.*/
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
    {
        tmpmasterdata = g_master_buff[i];
        tmpslavedata  = g_slave_buff[i];
        if (tmpslavedata != tmpmasterdata)
        {
            PRINTF("\r\nError occurred in this transfer ! \r\n");
            break;
        }
    }

    PRINTF("Slave received data :");
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /*4.Set up slave ready to send data to master.*/
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
    {
        g_slave_buff[i] = ~g_slave_buff[i];
    }

    PRINTF("This time , slave will send data: :");
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /*  Already setup the slave transfer ready in item 1. */

    /* 5.Set up master to receive data from slave. */

    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
    {
        g_master_buff[i] = 0;
    }

    /* Master send address to slave. */
    I2C_MasterStart(EXAMPLE_I2C_MASTER_BASEADDR, I2C_MASTER_SLAVE_ADDR_7BIT, kI2C_MasterReceive);

    /* Enable module interrupt. */
    I2C_EnableInterrupts(EXAMPLE_I2C_MASTER_BASEADDR, kI2C_GlobalInterruptEnable);
    I2C_EnableInterrupts(EXAMPLE_I2C_SLAVE_BASEADDR, kI2C_GlobalInterruptEnable);

    g_masterReadBegin = true;

    /* Master put receive data in receive buffer. */
    g_masterRxIndex = 0;

    /* Wait master receive finished. */
    while (g_masterRxIndex < I2C_DATA_LENGTH)
    {
    }

    /* Disable module interrupt. */
    I2C_DisableInterrupts(EXAMPLE_I2C_MASTER_BASEADDR, kI2C_GlobalInterruptEnable);
    I2C_DisableInterrupts(EXAMPLE_I2C_SLAVE_BASEADDR, kI2C_GlobalInterruptEnable);

    /*6.Transfer completed. Check the data.*/
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
    {
        tmpmasterdata = g_master_buff[i];
        tmpslavedata  = g_slave_buff[i];
        if (tmpslavedata != tmpmasterdata)
        {
            PRINTF("\r\nError occurred in the transfer ! \r\n");
            break;
        }
    }

    PRINTF("Master received data :");
    for (uint32_t i = 0U; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }
    PRINTF("\r\n\r\n");

    PRINTF("\r\nEnd of I2C example .\r\n");
    while (1)
    {
    }
}
