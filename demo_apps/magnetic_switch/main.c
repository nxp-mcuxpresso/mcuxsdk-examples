/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* SDK Included Files */
#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "fsl_nmh1000.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/
nmh_handle_t g_nmhHandle;


/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    nmh_config_t config = {0};
    status_t result;
    uint8_t magData;

    BOARD_InitHardware();

    PRINTF("\r\nI2C magnetic switch NMH1000\r\n");

    BOARD_MagSwitch_I2C_Init();

    /* Configure the I2C function */
    config.I2C_SendFunc    = BOARD_MagSwitch_I2C_Send;
    config.I2C_ReceiveFunc = BOARD_MagSwitch_I2C_Receive;

    /* Initialize magnetic switch sensor */
    result = NMH_Init(&g_nmhHandle, &config);
    if (result != kStatus_Success)
    {
        PRINTF("NMH_Init failed, error: %d\r\n", result);
        while(true);
    }

    while(true)
    {
        /* Read value of relative magnetic field strength */
        result = NMH_ReadMagData(&g_nmhHandle, &magData);
        if (result != kStatus_Success)
        {
            PRINTF("NMH_ReadMagData failed, error: %d\r\n", result);
            while(true);
        }

        PRINTF("Relative magnetic field strength: %d G\r\n", magData);

        SDK_DelayAtLeastUs(1000000, SystemCoreClock);
    }
}

