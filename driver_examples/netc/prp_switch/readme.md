# prp_switch

## Overview
The FreeRTOS PRP (Parallel Redundancy Protocol) project is a demonstration of implementing the IEC 62439-3 PRP protocol using FreeRTOS. This project provides a complete implementation of PRP functionality including frame forwarding, supervision, and node management.

PRP is a network redundancy protocol that provides seamless failover by sending duplicate frames over two independent networks (LAN A and LAN B). This implementation supports both DAN (Doubly Attached Node) and RedBox (Redundancy Box) modes.

## Running the demo
After the demo run on device, first link up all switch ports. Then connect swp0 and swp1 as PRP redundancy ports to PRP LAN A and LAN B, connect swp2 as redbox interlink port to SAN node. The SAN node device can communicate with PRP device in PRP network.

Example output:
Run PRP stack!.

NETC PRP example

Buffer ring initialized with 32 buffers

Wait PHY link up, please link up all switch ports.
Available buffers: 24
Hash table created successfully
Hash table created successfully

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/netc/prp_switch/example_board_readme.md)
- IMX943-EVK
