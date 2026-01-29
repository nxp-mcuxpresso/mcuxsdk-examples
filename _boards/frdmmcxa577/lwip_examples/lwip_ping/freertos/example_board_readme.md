Hardware requirements
=====================
- Type-C USB cable
- RJ45 Network cable (for 100BASE-TX mode)
- 10BASE-T1S network adapter and cable (optional for 10BASE-T1S mode)
- FRDM-MCXA577 board
- Personal Computer

Board settings
============
Short position 2-3 on J29.

Use with 10BASE-T1S Ethernet
============================
The example uses 100BASE-TX Ethernet over RJ45 by default. If you want to use the internal 10BASE-T1S digital PHY,
redefine BOARD_NETWORK_USE_TENBASET_PHY from board.h to 1 and rebuild. Connect to a PC with a 10BASE-T1S network
adapter instead of RJ45 and enable PLCA on a PC with the following parameters:
    - node ID: 0 (PLCA coordinator)
    - node count: 8
    - TO timer: 32 BT
    - burst count: 0
    - burst timer: 128 BT
