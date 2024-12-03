/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPI2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define LPI2C_BAUDRATE               100000U
#define LPI2C_DATA_LENGTH            32U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

lpi2c_master_config_t sMasterConfig      = {0};
lpi2c_slave_config_t sSlaveConfig        = {0};
uint8_t g_slave_buff[LPI2C_DATA_LENGTH]  = {0};
uint8_t g_master_buff[LPI2C_DATA_LENGTH] = {0};
volatile uint8_t g_masterTxIndex         = 0U;
volatile uint8_t g_masterRxIndex         = 0U;
volatile uint8_t g_slaveTxIndex          = 0U;
volatile uint8_t g_slaveRxIndex          = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
#pragma interrupt alignsp saveall
void LPI2C_MASTER_IRQHandler(void);
void LPI2C_MASTER_IRQHandler(void)
{
    uint16_t u16Flags = 0U;
    status_t reVal    = kStatus_Fail;
    /* Get interrupt status flags */
    u16Flags = LPI2C_MasterGetStatusFlags(EXAMPLE_LPI2C_MASTER_BASEADDR);

    if (u16Flags & kLPI2C_MasterTxReadyInterruptFlag)
    {
        /* If tx Index < LPI2C_DATA_LENGTH, master send->slave receive transfer is ongoing. */
        if (g_masterTxIndex < LPI2C_DATA_LENGTH)
        {
            reVal = LPI2C_MasterSend(EXAMPLE_LPI2C_MASTER_BASEADDR, &g_master_buff[g_masterTxIndex++], 1, false);
            if (reVal != kStatus_Success)
            {
                return;
            }
            /* The last byte */
            if (g_masterTxIndex == LPI2C_DATA_LENGTH)
            {
                /* Master send stop command. */
                reVal = LPI2C_MasterStop(EXAMPLE_LPI2C_MASTER_BASEADDR);
                if (reVal != kStatus_Success)
                {
                    return;
                }
                /* Disable master Tx interrupt otherwise the Tx interrupt will always works */
                LPI2C_MasterDisableInterrupts(EXAMPLE_LPI2C_MASTER_BASEADDR, kLPI2C_MasterTxReadyInterruptFlag);
            }
        }
    }

    if (u16Flags & kLPI2C_MasterRxReadyInterruptFlag)
    {
        /* If rx Index < LPI2C_DATA_LENGTH, master receive->slave send transfer is ongoing. */
        if (g_masterRxIndex < LPI2C_DATA_LENGTH)
        {
            g_master_buff[g_masterRxIndex++] = EXAMPLE_LPI2C_MASTER_BASEADDR->MRDR;
            if (g_masterRxIndex == LPI2C_DATA_LENGTH - 1U)
            {
                /* Master send stop command. */
                reVal = LPI2C_MasterStop(EXAMPLE_LPI2C_MASTER_BASEADDR);
                if (reVal != kStatus_Success)
                {
                    return;
                }
                /* Disable master Rx interrupt otherwise the Rx interrupt will always works */
                LPI2C_MasterDisableInterrupts(EXAMPLE_LPI2C_MASTER_BASEADDR, kLPI2C_MasterTxReadyInterruptFlag);
            }
        }
    }
}
#pragma interrupt off

#pragma interrupt alignsp saveall
void LPI2C_SLAVE_IRQHandler(void);
void LPI2C_SLAVE_IRQHandler(void)
{
    uint16_t u16Flags = 0U;

    /* Get interrupt status flags. */
    u16Flags = LPI2C_SlaveGetStatusFlags(EXAMPLE_LPI2C_SLAVE_BASEADDR);

    if (u16Flags & kLPI2C_SlaveAddressValidInterruptFlag)
    {
        /* Release the i2c bus */
        EXAMPLE_LPI2C_SLAVE_BASEADDR->SASR;
        return;
    }

    if (u16Flags & kLPI2C_SlaveRxReadyInterruptFlag)
    {
        /* If rx Index < LPI2C_DATA_LENGTH, slave receive->master send transfer is ongoing. */
        if (g_slaveRxIndex < LPI2C_DATA_LENGTH)
        {
            /* Send NACK at the last byte. */
            if (g_slaveRxIndex == (LPI2C_DATA_LENGTH - 1U))
            {
                EXAMPLE_LPI2C_SLAVE_BASEADDR->STAR = LPI2C_STAR_TXNACK_MASK;
            }

            g_slave_buff[g_slaveRxIndex++] = EXAMPLE_LPI2C_SLAVE_BASEADDR->SRDR;

            if (g_slaveRxIndex == LPI2C_DATA_LENGTH)
            {
                LPI2C_SlaveDisableInterrupts(EXAMPLE_LPI2C_SLAVE_BASEADDR,
                                             kLPI2C_SlaveRxReadyInterruptFlag | kLPI2C_SlaveAddressValidInterruptFlag);
            }
        }
    }

    if (u16Flags & kLPI2C_SlaveTxReadyInterruptFlag)
    {
        /* If rx Index < LPI2C_DATA_LENGTH, slave send->master receive transfer is ongoing. */
        if (g_slaveTxIndex < LPI2C_DATA_LENGTH)
        {
            EXAMPLE_LPI2C_SLAVE_BASEADDR->STDR = g_slave_buff[g_slaveTxIndex++];

            if (g_slaveTxIndex == LPI2C_DATA_LENGTH)
            {
                LPI2C_SlaveDisableInterrupts(EXAMPLE_LPI2C_SLAVE_BASEADDR,
                                             kLPI2C_SlaveTxReadyInterruptFlag | kLPI2C_SlaveAddressValidInterruptFlag);
            }
        }
    }
}
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    status_t reVal = kStatus_Fail;
    uint16_t i;

    BOARD_InitHardware();

    PRINTF("\r\nLPI2C example -- MasterFunctionalInterrupt_SlaveFunctionalInterrupt.\r\n");

    /* 1.Set up lpi2c slave first */
    LPI2C_SlaveGetDefaultConfig(&sSlaveConfig, LPI2C_MASTER_SLAVE_ADDR_7BIT, LPI2C_SLAVE_CLOCK_FREQUENCY);

    LPI2C_SlaveInit(EXAMPLE_LPI2C_SLAVE_BASEADDR, &sSlaveConfig);

    for (uint16_t i = 0U; i < LPI2C_DATA_LENGTH; i++)
    {
        g_slave_buff[i] = 0;
    }

    /* 2.Set up lpi2c master to send data to slave */
    for (uint16_t i = 0U; i < LPI2C_DATA_LENGTH; i++)
    {
        g_master_buff[i] = i;
    }

    PRINTF("Master will send data :");
    for (uint16_t i = 0U; i < LPI2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /* Setup lpi2c master */
    LPI2C_MasterGetDefaultConfig(&sMasterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);
    sMasterConfig.u32BaudRateBps = LPI2C_BAUDRATE;

    LPI2C_MasterInit(EXAMPLE_LPI2C_MASTER_BASEADDR, &sMasterConfig);

    /* Master start and send address to slave. */
    reVal = LPI2C_MasterStart(EXAMPLE_LPI2C_MASTER_BASEADDR, LPI2C_MASTER_SLAVE_ADDR_7BIT, kLPI2C_Write);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /* Enable module interrupt. */
    LPI2C_SlaveEnableInterrupts(EXAMPLE_LPI2C_SLAVE_BASEADDR,
                                kLPI2C_SlaveRxReadyInterruptFlag | kLPI2C_SlaveAddressValidInterruptFlag);
    LPI2C_MasterEnableInterrupts(EXAMPLE_LPI2C_MASTER_BASEADDR, kLPI2C_MasterTxReadyInterruptFlag);

    /* Wait slave receive finished. */
    while (g_slaveRxIndex < LPI2C_DATA_LENGTH)
    {
    }

    /* 3.Transfer completed. Check the data. */
    for (i = 0U; i < LPI2C_DATA_LENGTH; i++)
    {
        if (g_slave_buff[i] != g_master_buff[i])
        {
            PRINTF("\r\nError occurred in this transfer ! \r\n");
            break;
        }
    }

    PRINTF("Slave received data :");
    for (i = 0U; i < LPI2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /* 4.Set up slave ready to send data to master. */
    for (i = 0U; i < LPI2C_DATA_LENGTH; i++)
    {
        g_slave_buff[i] = ~g_slave_buff[i];
    }

    PRINTF("This time, slave will send data: ");
    for (i = 0U; i < LPI2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /* 5.Set up master to receive data from slave. */
    for (i = 0U; i < LPI2C_DATA_LENGTH; i++)
    {
        g_master_buff[i] = 0;
    }

    /* Master start and send address to slave. */
    reVal = LPI2C_MasterStart(EXAMPLE_LPI2C_MASTER_BASEADDR, LPI2C_MASTER_SLAVE_ADDR_7BIT, kLPI2C_Read);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /* Set command to receive data so that the master will send a NACK when received enough data */
    EXAMPLE_LPI2C_MASTER_BASEADDR->MTDR = LPI2C_MTDR_CMD(0X1U) | LPI2C_MTDR_DATA(LPI2C_DATA_LENGTH - 1);

    LPI2C_SlaveEnableInterrupts(EXAMPLE_LPI2C_SLAVE_BASEADDR,
                                kLPI2C_SlaveTxReadyInterruptFlag | kLPI2C_SlaveAddressValidInterruptFlag);
    LPI2C_MasterEnableInterrupts(EXAMPLE_LPI2C_MASTER_BASEADDR, kLPI2C_MasterRxReadyInterruptFlag);

    /* Wait master receive finished. */
    while (g_masterRxIndex < LPI2C_DATA_LENGTH)
    {
    }
    /* 6.Transfer completed. Check the data. */
    for (i = 0U; i < LPI2C_DATA_LENGTH; i++)
    {
        if (g_slave_buff[i] != g_master_buff[i])
        {
            PRINTF("\r\nError occurred in the transfer ! \r\n");
            break;
        }
    }

    PRINTF("Master received data :");
    for (i = 0U; i < LPI2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }
    PRINTF("\r\n\r\n");

    PRINTF("\r\nEnd of LPI2C example .\r\n");
    while (1)
    {
    }
}
