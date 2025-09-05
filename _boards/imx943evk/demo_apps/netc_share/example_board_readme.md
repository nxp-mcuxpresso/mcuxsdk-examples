## Hardware requirements

- Micro USB cable
- IMX943-EVK board
- 12V~20V power supply
- Personal Computer

## Board settings

This example uses internal NETC ENETC3 pseudo port, which is connected to
NETC switch. The two NETC switch 2.5G SGMII ports are initailized to use.

## Prepare the Demo

1.  Connect 12V~20V power supply to the board.
2.  Connect a micro USB cable between the host PC and the J15(FTDI_DEBUG) USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Program the Linux BSP to SD/eMMC. Then program flash.bin of this demo.
5.  Switch SW1 to power on the board.

Note: the flash.bin is flash_all target with mx94evknetc.cfg System Manager image.
For A core Linux BSP side usage, refer to Linux BSP User Guide "11.6.2 i.MX943 ENETC share use case".

## Running the demo

When the demo runs, the log would be seen on the terminal like below.
Both A core Linux BSP and M core share NETC ENETC3 port to use.

```
Switch is initialized.

Initializing ENETC for demo...
Start SRTM communication
Initializing PHY...

************************************************
 DHCP example
************************************************
 DHCP state       : SELECTING
Switch port 0 link up
Handle Peer Core Linkup
 DHCP state       : BOUND
 IPv4 Address     : 192.168.0.164
 IPv4 Subnet mask : 255.255.255.0
 IPv4 Gateway     : 192.168.0.1
```
