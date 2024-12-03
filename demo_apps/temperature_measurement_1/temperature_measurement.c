/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "sm_platform.h"
#include "app.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    bool enabled = false;
    int32_t status;
    uint32_t sensorConfig;
    scmi_sensor_desc_t desc;
    scmi_sensor_reading_t readings[SCMI_SENSOR_MAX_READINGS];

    BOARD_InitHardware();

    PRINTF("\r\n Temperature measurement example.");

    status = SCMI_SensorDescriptionGet(SM_PLATFORM_A2P, 0U, NULL, &desc);
    if (SCMI_ERR_SUCCESS != status)
    {
        PRINTF("\r\n Sensor descriptor get failed!");
    }

    status = SCMI_SensorConfigSet(SM_PLATFORM_A2P, desc.sensorId, SCMI_SENSOR_CONFIG_SET_ENABLE(1U));
    if (SCMI_ERR_SUCCESS != status)
    {
        PRINTF("\r\n Sensor configuration failed!");
    }

    status = SCMI_SensorConfigGet(SCMI_A2P, desc.sensorId, &sensorConfig);
    if (SCMI_ERR_SUCCESS != status)
    {
        enabled = SCMI_SENSOR_CONFIG_GET_ENABLED(sensorConfig);
        if (!enabled)
        {
            PRINTF("\r\n Sensor not enabled!");
        }
    }

    PRINTF("\r\n Sensor is ready to read, press entry key to get the temperature.");

    while (1)
    {
        GETCHAR();
        
        int32_t sensorValue = 0;

        status = SCMI_SensorReadingGet(SCMI_A2P, desc.sensorId, SCMI_SENSOR_READ_FLAGS_ASYNC(0U), readings);
        if (status == SCMI_ERR_SUCCESS)
        {
            sensorValue = readings[0].sensorValueLow;
        }
        PRINTF("\r\n current temperature is %.2f degrees Celsius", (double)((float)sensorValue / 100.0F));
    }
}
