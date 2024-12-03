# cns_adc_temperature_measurement

## Overview
The adc_temperature_measurement examples shows how to measure internal temperature sensor.

By selecting internal voltage reference 1.2V, 16-bit audio ADC accuracy and by measuring the internal
temperature sensor, the temperature is calculated according to the following formula:
            Tmeans(in C) = Conversion Result/TS_GAIN - TS_OFFSET.

## Supported Boards
