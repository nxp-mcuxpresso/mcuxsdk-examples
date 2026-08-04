# DSC RGPIO LED Output

## Overview

The DSC RGPIO LED Output example demonstrates the basic usage of the dsc_rgpio driver
to control GPIO output. The example initializes a GPIO pin as digital output and
periodically toggles it to blink an LED.

## Supported Boards

- mc56f85000evk

## Hardware Requirements

- Target board
- Personal computer

## Board Settings

No special hardware setup required. The example uses the on-board yellow LED (GPIO0 pin 0).

## Running the Demo

After building and flashing the example, the yellow LED on the board blinks periodically.
The following message is printed to the debug console:

```
 GPIO Driver example

 The LED is blinking.
```
