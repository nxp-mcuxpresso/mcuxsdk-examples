Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
- Personal Computer

Board settings
==============
No special board setting.
To flash the board in case the sensor is put in deep sleep, press SW3 or RESET.

Hardware references
=====================
|  Platform   | WAKESW | SCANLED   | CONNLED  |
|-------------|--------|--------   |----------|
| KW47-EVK    | SW3    |  LED2     | LED1     |

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
2. On the board that implements the adv_ext_central application, there are two options: Press **WAKESW** to start
active scanning or long press **WAKESW** to start passive scanning. If catching extended scannable advertising is
not an option, choose passive scanning. Otherwise, select active scanning.
The device wakes up, starts scanning, and enters Deep-sleep mode. The scanning ends when the 60 seconds timeout is
reached or when a connection with an adv_ext_peripheral device is established.
During scanning, all advertisements caught from adv_ext_peripheral devices are displayed on the terminal
window. When an extended non-connectable, non-scannable advertising with a periodic advertising attached is detected,
the adv_ext_central device attempts to sync with the periodic advertising train and prints the periodic advertising
data on the terminal window.
When the 60 seconds timer expires or the connection ends, the device reenters Deep-sleep mode until the **WAKESW** is pressed
again and all syncs with periodic advertising trains are terminated.
If gAppLowpowerEnabled_d is set 0, LEDs are enabled. The SCANLED flashes, whenever the device is scanning and is ON otherwise.
The CONNLED flashes, whenever there is a connection under way and is ON otherwise.
3. If the adv_ext_central connects to an adv_ext_peripheral device, it bonds (if no bond was previously made),
does service discovery (only the first time it connects with the peripheral), configures notification and waits for notifications
from the peripheral. If no data is sent within 5 seconds, the node disconnects and reenters Deep-sleep mode.
The peripheral sends a notification with the value of the temperature read through an ADC from a thermistor, if present, or randomly generated.
When the central receives the notification, it displays it on the terminal window and disconnects in 5 seconds.