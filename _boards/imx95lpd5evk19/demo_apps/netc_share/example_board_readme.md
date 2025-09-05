## Hardware requirements

- Micro USB cable
- IMX95LPD5-EVK board
- 12V~20V power supply
- Personal Computer

## Board settings

This example uses NETC ENETC2 10G port.

## Prepare the Demo

1.  Connect 12V~20V power supply to the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Program the Linux BSP to SD/eMMC. Then program flash.bin of this demo.
5.  Switch SW4 to power on the board.

Note: the flash.bin is flash_all target with mx95netc.cfg System Manager image.
For A core Linux BSP side usage, refer to Linux BSP User Guide "11.6.1 i.MX95 ENETC share use case".

## Running the demo

When the demo runs, the log would be seen on the terminal like below.
Both A core Linux BSP and M core share NETC ENETC2 10G port to use.

```
Initializing PHY...
[LINK STATE] netif=0, state=up, speed=1000M_full

************************************************
 DHCP example
************************************************
 DHCP state       : SELECTING
 DHCP state       : REQUESTING
 DHCP state       : CHECKING
 DHCP state       : BOUND

 IPv4 Address     : 192.168.0.4
 IPv4 Subnet mask : 255.255.255.0
 IPv4 Gateway     : 192.168.0.1
```
