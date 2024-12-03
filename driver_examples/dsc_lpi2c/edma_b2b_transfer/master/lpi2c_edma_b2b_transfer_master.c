/*
 * Copyright 2020,2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_edma.h"
#include "fsl_edma.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_BAUDRATE               100000U
#define I2C_DATA_LENGTH            33U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

AT_NONCACHEABLE_SECTION(uint8_t g_master_txBuff[I2C_DATA_LENGTH]);
AT_NONCACHEABLE_SECTION(uint8_t g_master_rxBuff[I2C_DATA_LENGTH]);
AT_NONCACHEABLE_SECTION_ALIGN(lpi2c_master_edma_transfer_handle_t g_m_edma_handle, sizeof(edma_channel_tcd_t));
volatile bool g_MasterCompletionFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void lpi2c_master_callback(lpi2c_master_edma_transfer_handle_t *psHandle)
{
    /* Signal transfer success when received success status. */
    if (psHandle->completionStatus == kStatus_Success)
    {
        g_MasterCompletionFlag = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    lpi2c_master_config_t sMasterConfig;
    lpi2c_master_transfer_t sMasterXfer = {0};
    edma_config_t sEdmaConfig;
    status_t reVal          = kStatus_Fail;
    uint8_t u8DeviceAddress = 0x01U;

    BOARD_InitHardware();

    PRINTF("\r\nLPI2C board2board EDMA example -- Master transfer.\r\n");

    /* Set channel */
#if (FSL_FEATURE_LPI2C_HAS_SEPARATE_DMA_RX_TX_REQn(EXAMPLE_LPI2C_MASTER) != 0)
    DMAMUX_ConnectChannelToTriggerSource(EXAMPLE_LPI2C_DMAMUX, EXAMPLE_I2C_DMAMUX_RX_CHANNEL,
                                         EXAMPLE_DMA_RX_REQUEST_SRC);
    DMAMUX_ConnectChannelToTriggerSource(EXAMPLE_LPI2C_DMAMUX, EXAMPLE_I2C_DMAMUX_TX_CHANNEL,
                                         EXAMPLE_DMA_TX_REQUEST_SRC);
#else
    DMAMUX_ConnectChannelToTriggerSource(EXAMPLE_LPI2C_DMAMUX, EXAMPLE_I2C_DMAMUX_CHANNEL, EXAMPLE_DMA_REQUEST_SRC);
#endif

    /* EDMA init */
    EDMA_GetDefaultConfig(&sEdmaConfig);
    EDMA_Init(EXAMPLE_LPI2C_MASTER_DMA, &sEdmaConfig);

    /* Set up i2c master to send data to slave*/
    /* First data in txBuff is data length of the transmitting data. */
    g_master_txBuff[0] = I2C_DATA_LENGTH - 1U;
    for (uint16_t i = 1U; i < I2C_DATA_LENGTH; i++)
    {
        g_master_txBuff[i] = i - 1;
    }

    PRINTF("Master will send data :");
    for (uint16_t i = 0U; i < I2C_DATA_LENGTH - 1U; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_txBuff[i + 1]);
    }
    PRINTF("\r\n\r\n");

    /* LPI2C Master configuration */
    LPI2C_MasterGetDefaultConfig(&sMasterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

    /* Change the default baudrate configuration */
    sMasterConfig.u32BaudRateBps = I2C_BAUDRATE;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(EXAMPLE_LPI2C_MASTER, &sMasterConfig);

    /* Create the LPI2C master DMA driver handle */
    LPI2C_MasterCreateEDMAHandle(EXAMPLE_LPI2C_MASTER, &g_m_edma_handle, lpi2c_master_callback, NULL,
                                 EXAMPLE_LPI2C_MASTER_DMA, EXAMPLE_LPI2C_TX_DMA_CHANNEL, EXAMPLE_LPI2C_RX_DMA_CHANNEL);

    /* deviceAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveAddress(w) + deviceAddress + length of data buffer + data buffer + stop*/
    sMasterXfer.u8SlaveAddress    = I2C_MASTER_SLAVE_ADDR_7BIT;
    sMasterXfer.eDirection        = kLPI2C_Write;
    sMasterXfer.pu8Command        = &u8DeviceAddress;
    sMasterXfer.u8CommandSize     = 1;
    sMasterXfer.pData             = g_master_txBuff;
    sMasterXfer.u16DataSize       = I2C_DATA_LENGTH;
    sMasterXfer.u8ControlFlagMask = kLPI2C_TransferStartStopFlag;

    /* Send master non-blocking data to slave */
    reVal = LPI2C_MasterTransferEDMA(&g_m_edma_handle, &sMasterXfer);
    if (reVal != kStatus_Success)
    {
        return -1;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag)
    {
    }
    g_MasterCompletionFlag = false;

    PRINTF("Receive sent data from slave :");

    /* deviceAddress = 0x01, data = g_master_rxBuff - read from slave.
      start + slaveAddress(w) + deviceAddress + repeated start + slaveAddress(r) + rx data buffer + stop */
    sMasterXfer.u8SlaveAddress    = I2C_MASTER_SLAVE_ADDR_7BIT;
    sMasterXfer.eDirection        = kLPI2C_Read;
    sMasterXfer.pu8Command        = &u8DeviceAddress;
    sMasterXfer.u8CommandSize     = 1;
    sMasterXfer.pData             = g_master_rxBuff;
    sMasterXfer.u16DataSize       = I2C_DATA_LENGTH - 1;
    sMasterXfer.u8ControlFlagMask = kLPI2C_TransferStartStopFlag;

    /* Receive non-blocking data from slave */
    reVal = LPI2C_MasterTransferEDMA(&g_m_edma_handle, &sMasterXfer);

    if (reVal != kStatus_Success)
    {
        return -1;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag)
    {
    }
    g_MasterCompletionFlag = false;

    for (uint16_t i = 0U; i < I2C_DATA_LENGTH - 1; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_rxBuff[i]);
    }
    PRINTF("\r\n\r\n");

    /* Transfer completed. Check the data.*/
    for (uint16_t i = 0U; i < I2C_DATA_LENGTH - 1; i++)
    {
        if (g_master_rxBuff[i] != g_master_txBuff[i + 1])
        {
            PRINTF("\r\nError occurred in the transfer ! \r\n");
            break;
        }
    }

    PRINTF("\r\nEnd of LPI2C example .\r\n");
    while (1)
    {
    }
}
