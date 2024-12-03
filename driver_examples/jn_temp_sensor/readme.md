# temp_sensor

## Overview

The temp_sensor example shows how to use the temperature sensor.

In this example, the calibration data (adc and temperature sensor) are first loaded from flash and then the temperature is computed.
For reading the temperature, the software:
a.	Calls load_calibration_param_from_flash(). This step shall be done once prior doing any read of temperature.
If calibration data of either ADC or temperature sensor is not present, the function returns false and measurement of temperature cannot be done.
b.	Calls get_temperature(8, &temperature_in_128th_degree) to read the temperature. 8 is the number of sample to acquire.

Note that reported temperature is only valid if function returns true menaning that calibration data for ADC and temperature sensor are present in flash.

