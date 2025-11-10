/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_tpm.h"
#include "fsl_xbar.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define EXAMPLE_I2C_MASTER ((LPI2C_Type *)EXAMPLE_I2C_MASTER_BASE)
#define EXAMPLE_I2C_SLAVE  ((LPI2C_Type *)EXAMPLE_I2C_SLAVE_BASE)

#define LPI2C_MASTER_SLAVE_ADDR_7BIT (0x7EU)
#define LPI2C_BAUDRATE               (400000) /* 100K */

#define LPI2C_DATA_LENGTH (32) /* MAX is 256 */

#define DEMO_PWM_FREQUENCY (24000U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[LPI2C_DATA_LENGTH];
uint8_t g_master_buff[LPI2C_DATA_LENGTH];

lpi2c_master_handle_t g_m_handle;
lpi2c_slave_handle_t g_s_handle;

volatile bool g_slaveCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void lpi2c_slave_callback(LPI2C_Type *base, lpi2c_slave_transfer_t *xfer, void *param)
{
    switch (xfer->event)
    {
        /* Setup the slave receive buffer */
        case kLPI2C_SlaveReceiveEvent:
            xfer->data     = g_slave_buff;
            xfer->dataSize = LPI2C_DATA_LENGTH;
            break;

        /* The master has sent a stop transition on the bus */
        case kLPI2C_SlaveCompletionEvent:
            g_slaveCompleted = true;
            break;

        /* Transmit event not handled in this demo */
        case kLPI2C_SlaveTransmitEvent:
        default:
            break;
    }
}

static void LPI2C_trigger_config(lpi2c_master_config_t *config)
{
    config->hostRequest.enable = true;
    config->hostRequest.polarity = kLPI2C_HostRequestPinActiveLow;
    config->hostRequest.source = kLPI2C_HostRequestInputTrigger;
}

static void tpm_init(void)
{
    tpm_config_t tpmInfo;
    tpm_chnl_pwm_signal_param_t tpmParam;
    int updatedDutycycle = 5;
    uint8_t control;

    TPM_GetDefaultConfig(&tpmInfo);
    tpmInfo.prescale = TPM_CalculateCounterClkDiv(BOARD_TPM_BASEADDR, DEMO_PWM_FREQUENCY, TPM_SOURCE_CLOCK);
    TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);
    tpmParam.chnlNumber = (tpm_chnl_t)BOARD_TPM_CHANNEL;
    tpmParam.level            = kTPM_HighTrue;
    tpmParam.dutyCyclePercent = updatedDutycycle;
    if (kStatus_Success != TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, DEMO_PWM_FREQUENCY, TPM_SOURCE_CLOCK)) {
	    PRINTF("\r\nSetup PWM fail!\r\n");
	    return;
    }
    TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
    control = TPM_GetChannelContorlBits(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL);
    TPM_EnableChannel(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, control);
    TPM_EnableInterrupts(BOARD_TPM_BASEADDR, kTPM_TimeOverflowInterruptEnable);
}

int main(void)
{
    uint32_t i = 0;
    lpi2c_slave_config_t slaveConfig;
    lpi2c_master_config_t masterConfig;
    status_t reVal = kStatus_Fail;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    XBAR_Init(kXBAR_DSC1);
    XBAR_SetSignalsConnection(kXBAR1_InputTpm6LptpmChTrigger0, kXBAR1_OutputLpi2c4Lpi2cTrgInput);
    PRINTF("\r\nIPSYNC trigger signal connected! \r\n");

    tpm_init();

    PRINTF("\r\nLPI2C example -- MasterPolling_SlaveInterrupt.\r\n");

    /* Set up i2c slave first*/
    /*
     * slaveConfig.address0 = 0U;
     * slaveConfig.address1 = 0U;
     * slaveConfig.addressMatchMode = kLPI2C_MatchAddress0;
     * slaveConfig.filterDozeEnable = true;
     * slaveConfig.filterEnable = true;
     * slaveConfig.enableGeneralCall = false;
     * slaveConfig.ignoreAck = false;
     * slaveConfig.enableReceivedAddressRead = false;
     * slaveConfig.sdaGlitchFilterWidth_ns = 0;
     * slaveConfig.sclGlitchFilterWidth_ns = 0;
     * slaveConfig.dataValidDelay_ns = 0;
     * slaveConfig.clockHoldTime_ns = 0;
     */
    LPI2C_SlaveGetDefaultConfig(&slaveConfig);

    /* Change the slave address */
    slaveConfig.address0 = LPI2C_MASTER_SLAVE_ADDR_7BIT;

    /* Initialize the LPI2C slave peripheral */
    LPI2C_SlaveInit(EXAMPLE_I2C_SLAVE, &slaveConfig, LPI2C_SLAVE_CLOCK_FREQUENCY);

    memset(g_slave_buff, 0, sizeof(g_slave_buff));

    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_SlaveTransferCreateHandle(EXAMPLE_I2C_SLAVE, &g_s_handle, lpi2c_slave_callback, NULL);

    /* Start accepting I2C transfers on the LPI2C slave peripheral */
    reVal = LPI2C_SlaveTransferNonBlocking(EXAMPLE_I2C_SLAVE, &g_s_handle,
                                           kLPI2C_SlaveReceiveEvent | kLPI2C_SlaveCompletionEvent);
    if (reVal != kStatus_Success)
    {
        return -1;
    }
    /* Set up i2c master to send data to slave*/
    for (i = 0; i < LPI2C_DATA_LENGTH; i++)
    {
        g_master_buff[i] = i;
    }

    /* Display the data the master will send */
    PRINTF("Master will send data :");
    for (i = 0; i < LPI2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }

    PRINTF("\r\n\r\n");

    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Hz = LPI2C_BAUDRATE;

    LPI2C_trigger_config(&masterConfig);

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(EXAMPLE_I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

    /* Send master blocking data to slave */
    if (kStatus_Success == LPI2C_MasterStart(EXAMPLE_I2C_MASTER, LPI2C_MASTER_SLAVE_ADDR_7BIT, kLPI2C_Write))
    {
        while (LPI2C_MasterGetStatusFlags(EXAMPLE_I2C_MASTER) & kLPI2C_MasterNackDetectFlag)
        {
        }

        reVal = LPI2C_MasterSend(EXAMPLE_I2C_MASTER, g_master_buff, LPI2C_DATA_LENGTH);
        if (reVal != kStatus_Success)
        {
            return -1;
        }

        reVal = LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
        if (reVal != kStatus_Success)
        {
            return -1;
        }
    }
    /* LPI2C_MasterTransferNonBlocking(EXAMPLE_I2C_MASTER, &g_m_handle, &masterXfer); */

    /* Wait for the transfer to complete. */
    while (!g_slaveCompleted)
    {
    }

    /* Transfer completed. Check the data. */
    for (i = 0; i < LPI2C_DATA_LENGTH; i++)
    {
        if (g_slave_buff[i] != g_master_buff[i])
        {
            PRINTF("\r\nError occurred in this transfer ! \r\n");
            break;
        }
    }

    /* Check if we matched on all of the bytes */
    if (i == LPI2C_DATA_LENGTH)
    {
        PRINTF("\r\n Transfer successful!\r\n ");
    }

    /* Display the slave data */
    PRINTF("\r\nSlave received data :");
    for (i = 0; i < LPI2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /* Hang at the end */
    while (1)
    {
    }
}
