# FRDM-KW43 Power Manager Test

## Hardware requirements

- Type-C USB cable
- FRDM-KW43 Board
- Personal Computer

## Board settings

- No special jumper change is required for the default LPTMR wakeup flow.
- On current KW43 ROM, Power Down and Deep Power Down wake through normal boot instead of fast boot resume.

## Prepare the Demo

1. Connect a USB cable between the host PC and the FRDM board J28.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

## Running the demo

The demo uses LPTMR0 as the wakeup source for all framework power states.

For Sleep and Deep Sleep, the application resumes execution after wakeup.

For Power Down and Deep Power Down, the current KW43 A0 ROM returns through the normal boot path, so the banner is printed again after wakeup.

