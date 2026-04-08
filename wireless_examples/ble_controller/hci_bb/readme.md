# HCI Black Box application

## 1 - overview
The Bluetooth LE HCI Black Box demo application gives access to the Bluetooth LE Controller via a serial interface using the HCI protocol over serial interface. 

Refer to the Bluetooth Specification for the format of HCI commands and events over serial interface and the full list of supported commands and events.

The demo can be used with the Test Tool for Connectivity Products. The Command Console application can be downloaded from the NXP website. 

Alternatively you can also download it using a custom application that supports the HCI protocol and commands and events over serial interface.

Note: The HCI protocol encapsulation is dependent on the type of interface it is being used on. See the Bluetooth Specification for the HCI message format on each type of supported interface.

## 2 - Running the HCI Black Box application

### Supported boards:
- KW45B41Z-EVK
- FRDM-MCXW71
- KW47-EVK
- MCX-W72-EVK

The Bluetooth LE HCI Black Box demo application is designed to be used via serial interface. 

This can be achieved using the TEST Tool for Connectivity Products – Command Console application as described below.
1. Download the demo application to a supported board.
2. Connect the board to a USB port of the PC. The UASB COM port drivers must be installed properly and a COM port corresponding to the board should be available.
3. Open the Test Tool application and connect to the serial port corresponding to the board on which the Bluetooth LE HCI Black Box application runs. 
The serial communication parameters are: baud rate 115200, 8N1, and no flow control.