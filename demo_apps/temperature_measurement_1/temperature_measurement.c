/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "scmi.h"
#include "scmi_internal.h"
#include "app.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef SCMI_A2P
#define SCMI_A2P (0U)
#endif
#define NUM_SENSOR_DESCS_MASK (0xFFFU)
#define NUM_SENSOR_DESCS_SHIFT (0U)
#define NUM_SENSOR_DESCS(x) (((uint32_t)(((uint32_t)(x)) << NUM_SENSOR_DESCS_SHIFT)) & NUM_SENSOR_DESCS_MASK)

/* scmi protocol(from linux: include/linux/scmi_protocol.h) */
#define TEMPERATURE_C (0x2)
#define TEMPERATURE_F (0x3)
#define TEMPERATURE_K (0x4)

/* Common attributes_high macros(from linux: drivers/firmware/arm_scmi/sensors.c) */
#define SENSOR_SCALE(x) SCMI_SENSOR_ATTR_HIGH_SENSOR_EXP(x)
#define SENSOR_SCALE_SIGN  (1 << 4)
#define SENSOR_SCALE_EXTEND  (0xFFFFFFE0)

/* Sign extend to a full s32 */
#define S32_EXT(v) \
   ({ \
       int __v = (v); \
       if (__v & SENSOR_SCALE_SIGN) \
           __v |= SENSOR_SCALE_EXTEND; \
       __v; \
   })

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
scmi_sensor_desc_t desc[SCMI_SENSOR_MAX_DESC];
scmi_sensor_reading_t readings[SCMI_SENSOR_MAX_READINGS];

/*******************************************************************************
 * Code
 ******************************************************************************/
static inline uint64_t __pow10(uint8_t x)
{
    uint64_t r = 1;

    while (x--)
    {
        r *= 10;
    }

    return r;
}

void DEMO_GetTemp(int descIdx)
{
    uint32_t sensorConfig;
    int32_t status;
    bool enabled = false;

    status = SCMI_SensorConfigSet(SCMI_A2P, desc[descIdx].sensorId, SCMI_SENSOR_CONFIG_SET_ENABLE(1U));
    if (SCMI_ERR_SUCCESS != status)
    {
        PRINTF("\r\n Sensor configuration failed!");
	return;
    }

    status = SCMI_SensorConfigGet(SCMI_A2P, desc[descIdx].sensorId, &sensorConfig);
    if (SCMI_ERR_SUCCESS != status)
    {
        enabled = SCMI_SENSOR_CONFIG_GET_ENABLED(sensorConfig);
        if (!enabled)
        {
            PRINTF("\r\n Sensor not enabled!");
	    return;
        }
    }
    PRINTF("\r\n Sensor is ready to read, press enter key to get the temperature.");

    GETCHAR();

    status = SCMI_SensorReadingGet(SCMI_A2P, desc[descIdx].sensorId, SCMI_SENSOR_READ_FLAGS_ASYNC(0U), readings);
    if (status == SCMI_ERR_SUCCESS)
    {

        unsigned int sensorType = SCMI_SENSOR_ATTR_HIGH_SENSOR_TYPE(desc[descIdx].sensorAttributesHigh);
        int scale = S32_EXT(SENSOR_SCALE(desc[descIdx].sensorAttributesHigh));
        uint64_t temp = 0ULL;
        uint64_t factor = 0ULL;
        uint64_t sensorValue = 0;

        sensorValue = readings[0].sensorValueLow | ((uint64_t)readings[0].sensorValueHigh << 32UL);
        switch (sensorType) {
        case TEMPERATURE_C:
            scale += 3;
            break;
        default:
            break;
        }

        if (sensorType >= TEMPERATURE_C && sensorType <= TEMPERATURE_K)
        {
            factor = __pow10(abs(scale));
            if (scale > 0)
            {
                temp = sensorValue * factor / 1000ULL;
            }
            else
            {
                    temp = sensorValue / factor / 1000ULL;
            }
            PRINTF("\r\n Current temperature is %llu degrees Celsius", temp);
        }
        else
        {
            PRINTF("\r\n The sensor %s is not a temperature sensor, so please select a temperature sensor", desc[descIdx].name);
        }
    }
    else
    {
        PRINTF("\r\n Failed to get data from sensor");
    }

    return;
}

/*!
 * @brief Main function
 */
int main(void)
{
    int32_t status;
    uint32_t numSensorFlags = 0U;
    uint32_t numSensorDescs = 0U;
    int digit = 0U;
    char ch;
    int descIdx = 0;

    BOARD_InitHardware();

    PRINTF("\r\n Temperature measurement example.");

    status = SCMI_SensorDescriptionGet(SCMI_A2P, 0U, &numSensorFlags, desc);
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
                PRINTF("\r\n Valid number is 0-%d", numSensorDescs - 1);
            }
            else
            {
                PRINTF("\r\n Sensor(desc index is %d) %s is selected", digit, desc[digit].name);
                descIdx = digit;
                DEMO_GetTemp(descIdx);
            }
        }
        else
        {
            PRINTF("\r\n Pls input a number:");
        }
    }
}
