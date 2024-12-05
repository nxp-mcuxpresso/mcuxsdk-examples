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

Hardware references
=====================
|  Platform | ADVSW | ADVLED | EXTADVLED |
|:----------|:-----:|:------:|:---------:|
| KW47-EVK  | SW2   | LED2   | LED1      |

Board settings
==============
No special board setting.

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
After flashing the beacon, the sensor is put in deep sleep (all LEDs are off). To flash the board in case the beacon is put in Deep-sleep mode, press the **ADVSW** or **RESET** button. After this step, any attached debugger loses its connection. The default configuration of the application enables low power, which disables LED support. The user can manually change the configuration and enable LED support, otherwise all subsequent LED behavior references are ignored.

By default, the application uses extended advertising. However, it can be configured to use legacy advertising by setting the `gBeaconAE_c` define to `0`. In the legacy configuration, the first press of the advertising switch starts the legacy advertising and the second press stops it.

Running the demo
================
The beacon can be tested with any Bluetooth® Smart Ready products available on the market. The IoT Toolbox can also be used to showcase the profile functionality.

## Beacon usage with extended advertising

To use the Beacon application with the advertising extensions capabilities, the `gBeaconAE_c` define option must be set to `1`. Doing this enables the usage of extended advertising and periodic advertising. The application cycles between these modes are in the following manner:

- The first **ADVSW** press starts legacy advertising, **CONNLED** turns solid.
- The second **ADVSW** press stops legacy advertising and starts extended advertising, **CONNLED** turns off, **EXTADVLED** turns solid.
- The third **ADVSW** press stops extended advertising carrying data and then starts extended advertising without data and periodic advertising, **EXTADVLED** starts flashing.
- The fourth **ADVSW** press stops periodic advertising and extended advertising without data and starts legacy advertising and extended advertising, both **CONNLED** and **EXTADVLED** turn solid.
- The fifth **ADVSW** press stops them all, both **CONNLED** and **EXTADVLED** turn off.

Not all smartphones support extended advertising, hence a different method to view the AE beacon is to use the ble_shell application. In order to do this, perform the following steps:

1. Flash a board with the beacon application, as described above.
2. Flash a board with the ble_shell application, as described in <u>Bluetooth LE Shell</u> and connect to it using a serial port.
3. Press the **ADVSW** button two times on the beacon to start extended advertising on the coded PHY.
4. To view the advertising data, enter the following commands in the shell terminal to set the scanning PHY to coded and start scanning.
```
BLE Shell>gap scancfg -phy 4
BLE Shell>gap scanstart filter
```
5. To start the periodic advertising, press **ADVSW** button again on the beacon.
6. To sync with the beacon, issue the following commands on the shell terminal.
```
BLE Shell>gap scancfg -phy 4
BLE Shell>gap periodicsync -peer 006037228B55 -type 0
BLE Shell>gap scanstart filter
```

The peer parameter of the `periodicsync` command is the public address of the beacon.

## Extended Advertising with very large data

To use very large advertising data for extended advertising, set the `gBeaconLargeExtAdvData_c` define to `1`. The same steps are used to view the data using ble_shell:

1. Flash a board with the beacon application.
2. Flash a board with the ble_shell application, as described in Bluetooth LE Shell and connect to it using a serial port.
3. Press the **ADVSW** button two times on the beacon to start extended advertising on the coded PHY.
4. To view the advertising data, enter the following commands in the shell terminal to set the scanning PHY to coded and start scanning.
```
BLE Shell>gap scancfg -phy 4
BLE Shell>gap scanstart filter
```
