/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "temp_sensor.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
// uncomment if you want to verify the computation of the temperature against expected values (unit test)
// #define TEST
#ifdef TEST
#include "temp_sensor_drv_test.h"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware. */
    BOARD_InitHardware();
    PRINTF("Temperature measurement example.\r\n");

#ifdef TEST
    test_temperature_computation();
#else
    bool status = false;
    // Read temperature and adc calibration parameter once. Store ADC calibration values into ADC register.
    status = load_calibration_param_from_flash(DEMO_ADC_BASE);

    while (1)
    {
        // get the temperature (average on 8 ADC samples)
        int32_t temperature_in_128th_degree = 0;
        status = get_temperature(DEMO_ADC_BASE, DEMO_ADC_TEMPERATURE_SENSOR_CHANNEL, THREE_HUNDRED_MICROSECONDS, 8,
                                 &temperature_in_128th_degree);
        if (status)
        {
            // integer part
            int32_t temp_int_part = temperature_in_128th_degree / 128;
            // decimal part
            int32_t temp_dec_part = (((temperature_in_128th_degree - (temp_int_part * 128))) * 10) / 128;
            PRINTF("\r\n Temperature in Celsius (average on 8 samples) = %d.%d\r\n", temp_int_part, temp_dec_part);
        }
        else
        {
            PRINTF(
                "FAIL: temperature measurement failed due to wrong ADC configuration or missing calibration data!\r\n");
        }
        PRINTF("\r\n");
        /* Get the input from terminal to start another conversion  */
        GETCHAR();
    }
#endif
}
