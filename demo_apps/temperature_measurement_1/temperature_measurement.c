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
#define NUM_SENSOR_DESCS_MASK (0xFFFU)
#define NUM_SENSOR_DESCS_SHIFT (0U)
#define NUM_SENSOR_DESCS(x) (((uint32_t)(((uint32_t)(x)) << NUM_SENSOR_DESCS_SHIFT)) & NUM_SENSOR_DESCS_MASK)

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
    scmi_sensor_desc_t desc[SCMI_SENSOR_MAX_DESC];
    scmi_sensor_reading_t readings[SCMI_SENSOR_MAX_READINGS];
    uint32_t numSensorFlags = 0U;
    uint32_t numSensorDescs = 0U;
    int digit = 0U;
    char ch;
    int descIdx = 0;

    BOARD_InitHardware();

    PRINTF("\r\n Temperature measurement example.");

    status = SCMI_SensorDescriptionGet(SM_PLATFORM_A2P, 0U, &numSensorFlags, desc);
    numSensorDescs = numSensorFlags & NUM_SENSOR_DESCS_MASK;
    for (int i = 0; i < numSensorDescs; i++)
    {
	    PRINTF("desc[%d].sensorId = 0x%x\r\n", i, desc[i].sensorId);
	    PRINTF("desc[%d].name = %s\r\n", i, desc[i].name);
    }
    /* choose a sensor to get temperature */
    PRINTF("Pls input a number to choose sensor to get data:(valid input is 0-%d)\r\n", numSensorDescs - 1);
    while (1)
    {
        ch = GETCHAR();
	PUTCHAR(ch);
        if (ch >= '0' && ch <= '9')
        {
            digit = ch - '0';
            if (digit >= numSensorDescs)
            {
                PRINTF("valid number is 0-%d\r\n", numSensorDescs - 1);
            }
	    else
	    {
		PRINTF("sensor(desc index is %d) %s is selected\r\n", digit, desc[digit].name);
		descIdx = digit;
		break;
	    }
        }
        else
        {
            PRINTF("Pls input a number:\r\n");
        }

    }
    if (SCMI_ERR_SUCCESS != status)
    {
        PRINTF("\r\n Sensor descriptor get failed!");
    }

    status = SCMI_SensorConfigSet(SM_PLATFORM_A2P, desc[descIdx].sensorId, SCMI_SENSOR_CONFIG_SET_ENABLE(1U));
    if (SCMI_ERR_SUCCESS != status)
    {
        PRINTF("\r\n Sensor configuration failed!");
    }

    status = SCMI_SensorConfigGet(SM_PLATFORM_A2P, desc[descIdx].sensorId, &sensorConfig);
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

        status = SCMI_SensorReadingGet(SM_PLATFORM_A2P, desc[descIdx].sensorId, SCMI_SENSOR_READ_FLAGS_ASYNC(0U), readings);
        if (status == SCMI_ERR_SUCCESS)
        {
            sensorValue = readings[0].sensorValueLow;
            PRINTF("\r\n current temperature is %.2f degrees Celsius", (double)((float)sensorValue / 100.0F));
        }
	else
        {
            PRINTF("Failed to get data from sensor\r\n");
        }
    }
}
