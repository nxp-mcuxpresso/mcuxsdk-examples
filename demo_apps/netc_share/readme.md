# netc_share

## Overview

The netc_share demo application demonstrates a DHCP demo on the lwIP TCP/IP stack
with NETC PSI, meanwhile, the NETC VSIs could also be used by other cores independently.
For example, A core Linux BSP is using NETC VSIs.
The application acts as a DHCP client and prints the status as it is progressing.
Once the interface is being bound to an IP address obtained from DHCP server, address information is printed.
It does so continuously - it prints the client's state even as the link state changes.
Those link state changes are also printed.

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

## Running the demo
When the demo runs, the log would be seen on the terminal like below.
Both A core Linux BSP and M core share NETC ENETC2 10G port to use.

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

## Supported Boards
- [IMX95LPD5EVK-19](../../_boards/imx95lpd5evk19/demo_apps/netc_share/example_board_readme.md)
