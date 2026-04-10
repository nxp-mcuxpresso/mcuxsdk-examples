# FRDM-KW43 Power Mode Switch

## Hardware requirements

- Type-C USB cable
- FRDM-KW43 Board
- Personal Computer

## Board settings

This demo does not require special jumper changes for the default LPTMR and SW3 wakeup flows.

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

When running the demo, the debug console shows the menu to command the MCU to the target power mode.

Use LPTMR or SW4 as the wakeup source when prompted by the console.

NOTE: Only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow
and receive the wrong input value.

```text
###########################    Power Mode Switch Demo    ###########################
    Core Clock = 96000000Hz
    Power mode: Active

Please Select the desired power mode:

        Press A to enter Active mode!
        Press B to enter Sleep1 mode!
        Press C to enter DeepSleep1 mode!
        Press D to enter DeepSleep2 mode!
        Press E to enter DeepSleep3 mode!
        Press F to enter DeepSleep4 mode!
        Press G to enter PowerDown1 mode!
        Press H to enter PowerDown2 mode!
        Press I to enter PowerDown3 mode!
        Press J to enter PowerDown4 mode!
        Press K to enter DeepPowerDown1 mode!
        Press L to enter DeepPowerDown2 mode!

        Waiting for power mode select...
```

