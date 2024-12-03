Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Mini/micro USB cable
- K32W148-EVK Board
- Personal Computer

Board settings
==============
No special board setting.
To flash the board in case the sensor is put in deep sleep, press SW3 or RESET.

Hardware references
=====================
|  Platform   | WAKESW | OPTSW  | ADVLED   | CONNLED  |
|-------------|--------|--------|----------|----------|
| K32W148-EVK | SW3    | SW2    | LED2     | LED1     |

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2.  Download the program to the target board.
3.  Press the reset button on your board to begin running the demo.
4.  Open a serial terminal application and use the following settings with the detected serial device:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

User interface
================
After flashing the board, both nodes enter Deep-sleep mode. To flash the board again, press **WAKESW**.
The application default configuration enables low power that disables LED support.
The user disables low power and enables LED support setting to 0.
To wake up the node, press the **WAKESW** button. Both applications provide guidance over the UART.

Running the demo
================
The setup requires two supported platforms, one for the adv_ext_peripheral, and one for the adv_ext_central.

1. Open a serial port terminal and connect it to each platform with the settings provided in the previous paragraph.
The start screen is displayed after the boards are reset.
2. On the adv_ext_peripheral application, press the **WAKESW** button. The board exits Deep-sleep mode and displays the menu.
Use the **OPTSW** to choose an option. The option printed on the bottom changes every time the switch is pressed.
When the option matches your intention (for example, 3 Starts Extended Connectable Advertising), press the **WAKESW** again
to make a decision. The advertising type chosen is started and the board starts entering low-power between advertising events.
Next time the **WAKESW** is pressed, an updated menu is printed.
There is no timeout for advertising. The board continues advertising until it is stopped, or a connection is established (for legacy
and extended connectable advertising only) with an adv_ext_central device.
3. If the adv_ext_central connects to an adv_ext_peripheral device, it bonds (if no bond was previously made),
does service discovery (only the first time it connects with the peripheral), configures notification and waits for notifications
from the peripheral. If no data is sent within 5 seconds, the node disconnects and reenters Deep-sleep mode.
The peripheral sends a notification with the value of the temperature read through an ADC from a thermistor, if present, or randomly generated.
When the central receives the notification, it displays it on the terminal window and disconnects in 5 seconds.