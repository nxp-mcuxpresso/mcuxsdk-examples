# Overview

This application is based on a GATT Temperature Service and demonstrates power consumption optimization in BLE.
Power consumption is optimized during advertising, connected and no activity states.
For more information, please consult the README.md of the application.

# Toolchain supported

- IAR Embedded Workbench
- MCUXpresso IDE

# Hardware requirements

- Micro USB cable
- kw45b41zloc board

# Board settings

No special board setting.

# Prepare the Demo

1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Download the program to the target board.
3.  Press the reset button on your board to begin running the demo.

# Running the demo

At power on, device starts to scan. A device with Temperature Sensor application or the Low Power Peripheral reference design application could be used to get connected.

