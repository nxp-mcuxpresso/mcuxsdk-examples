## Hardware requirements

- Type-C USB cable
- IMXRTL345MB board
- Logic analyzer or oscilloscope (to probe the SCT outputs)
- Personal computer

## Board settings

The example drives two SCTimer (SCT0) center-aligned complementary output pairs (IOCON FUNC4):

| Pair | Side      | SCT output | Pin     | Board location |
|------|-----------|------------|---------|----------------|
| 1    | high side | SCT0_OUT0  | TBD     | TBD            |
| 1    | low side  | SCT0_OUT1  | TBD     | TBD            |
| 2    | high side | SCT0_OUT3  | TBD     | TBD            |
| 2    | low side  | SCT0_OUT4  | TBD     | TBD            |

# Prepare the Demo

1.  Connect a Type-C USB cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

## Running the demo

The log below shows the example output in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SCTimer complementary PWM + dead-time example
Two center-aligned pairs, 20000 Hz, dead time 120 ticks; duty ramps 10%-90%.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Probe pair 1 on TBD / TBD and pair 2 on TBD / TBD (plus GND) with a logic analyzer /
oscilloscope. Each pair's outputs are complementary with a dead-time gap at every switching edge. The
duty cycle is updated continuously (glitchless, applied at the next period); pair 2 ramps in the opposite direction to pair 1, and the PWM frequency stays constant throughout.
