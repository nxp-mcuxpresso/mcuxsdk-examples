Hardware requirements
=====================
- USB Type-C cable for debug console
- Network cable RJ45 standard
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
This example uses the on-board COMM_ENET_QOS port (gigabit YT8531 PHY, RGMII), which terminates at RJ45 port J57. Set jumper J52 pins 2-3 to route the on-board analog mux to the external RGMII PHY before running the example.

10BASE-T1S PHY support
======================
This example supports the internal 10BASE-T1S PHY (TENBASET_PHY) as an
alternative to the external RGMII PHY. To enable, define in board.h:

    #define BOARD_NETWORK_USE_TENBASET_PHY (1U)

Hardware setup for 10BASE-T1S mode:
- Set jumper J52 to position 1-2 to route the board analog mux to the
  T1S PHY.
- Attach the T1S link to RJ45 port J40 (the T1S connector dedicated to
  COMM_ENET_QOS on this EVK).

Hardware setup for RGMII mode:
- Set jumper J52 to position 2-3 to route the board analog mux to the
  external RGMII PHY.
- Use RJ45 port J57 for the RGMII link.
