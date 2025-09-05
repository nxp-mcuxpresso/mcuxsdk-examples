# netc_share

## Overview

The netc_share demo application demonstrates a DHCP demo on the lwIP TCP/IP stack
with NETC PSI, meanwhile, the NETC VSIs could also be used by other cores independently.
For example, A core Linux BSP is using NETC VSIs.

The application acts as a DHCP client and prints the status as it is progressing.
Once the interface is being bound to an IP address obtained from DHCP server, address information is printed.
It does so continuously - it prints the client's state even as the link state changes.
Those link state changes are also printed.

## Supported Boards
- [IMX95LPD5EVK-19](../../_boards/imx95lpd5evk19/demo_apps/netc_share/example_board_readme.md)
- [IMX943EVK](../../_boards/imx943evk/demo_apps/netc_share/example_board_readme.md)
