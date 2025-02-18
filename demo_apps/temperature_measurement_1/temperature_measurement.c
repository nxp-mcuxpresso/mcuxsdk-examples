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
    uint64_t sensorValue = 0;


    BOARD_InitHardware();

    PRINTF("\r\n Temperature measurement example.");

    status = SCMI_SensorDescriptionGet(SM_PLATFORM_A2P, 0U, &numSensorFlags, desc);
    if (SCMI_ERR_SUCCESS != status)
    {
        PRINTF("\r\n Sensor descriptor get failed!");
        assert(false);
    }
    numSensorDescs = numSensorFlags & NUM_SENSOR_DESCS_MASK;
    for (int i = 0; i < numSensorDescs; i++)
    {
	    PRINTF("\r\n desc[%d].sensorId = 0x%x", i, desc[i].sensorId);
	    PRINTF("\r\n desc[%d].name = %s", i, desc[i].name);
    }
    while (1)
    {
        /* choose a sensor to get temperature */
        PRINTF("\r\n Pls input a number to choose sensor to get data:(valid input is 0-%d)", numSensorDescs - 1);
        ch = GETCHAR();
	PUTCHAR(ch);
        if (ch >= '0' && ch <= '9')
        {
            digit = ch - '0';
            if (digit >= numSensorDescs)
            {
                PRINTF("\r\n Valid number is 0-%d\r\n", numSensorDescs - 1);
            }
	    else
	    {
		PRINTF("\r\n Sensor(desc index is %d) %s is selected", digit, desc[digit].name);
		descIdx = digit;
	    }
        }
        else
        {
            PRINTF("\r\n Pls input a number:");
            continue;
        }

        status = SCMI_SensorConfigSet(SM_PLATFORM_A2P, desc[descIdx].sensorId, SCMI_SENSOR_CONFIG_SET_ENABLE(1U));
        if (SCMI_ERR_SUCCESS != status)
        {
            PRINTF("\r\n Sensor configuration failed!");
            continue;
        }

        status = SCMI_SensorConfigGet(SM_PLATFORM_A2P, desc[descIdx].sensorId, &sensorConfig);
        if (SCMI_ERR_SUCCESS != status)
        {
            enabled = SCMI_SENSOR_CONFIG_GET_ENABLED(sensorConfig);
            if (!enabled)
            {
                PRINTF("\r\n Sensor not enabled!");
                continue;
            }
        }
        PRINTF("\r\n Sensor is ready to read, press enter key to get the temperature.");

        GETCHAR();
        
        status = SCMI_SensorReadingGet(SM_PLATFORM_A2P, desc[descIdx].sensorId, SCMI_SENSOR_READ_FLAGS_ASYNC(0U), readings);
        if (status == SCMI_ERR_SUCCESS)
        {
            sensorValue = readings[0].sensorValueLow | ((uint64_t)readings[0].sensorValueHigh << 32UL);
            PRINTF("\r\n Current temperature is %llu degrees Celsius", sensorValue);
        }
	else
        {
            PRINTF("\r\n Failed to get data from sensor");
            continue;
        }
    }
}
