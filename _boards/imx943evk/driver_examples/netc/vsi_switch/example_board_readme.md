Hardware requirements
=====================
- Micro USB cable
- IMX943-EVK board
- 12V~20V power supply
- Personal Computer
- Ethernet cable

Board settings
==============
This example uses the NETC Virtual Station Interface (VSI) on the M7_0 core to send
Ethernet frames through the NETC switch. The NETC switch and ENETC3 pseudo port must
be configured by the M33S core running the netc-share application before M7_0 can
operate the VSI.

NETC switch performs L2 switching on the SGMII ports. Connect an Ethernet cable to
SWP0 and the host PC to capture broadcast frames.

System Manager configuration
=============================
This example requires a custom System Manager (SM) configuration that grants M7_0
ownership of the VSI resources and shared access to the NETC IERB, ENETC3, and ECAM.
The following changes must be applied to the SM configuration file (mx94evk.cfg or
equivalent) before building the flash.bin:

Under the M33S EENV (LM1), add ACCESS permissions for VSI so M33S can configure
them on behalf of M7_0:

    NETC_VSI0           ACCESS
    NETC_VSI1           ACCESS
    NETC_VSI2           ACCESS

Under the M7_0 EENV (LM2), add OWNER of VSI and ACCESS to shared NETC resources:

    NETC_LDID9          OWNER, sid=0x29
    NETC_LDID10         OWNER, sid=0x2A
    NETC_LDID11         OWNER, sid=0x2B
    NETC_IERB           ACCESS
    NETC3               ACCESS
    NETC_ECAM           ACCESS
    NETC_VSI0           OWNER
    NETC_VSI1           OWNER
    NETC_VSI2           OWNER

Note: In the default mx94evknetc.cfg, NETC_VSI0/1/2 and NETC_LDID9/10/11 are
assigned to the A55 non-secure domain. The above reassignment moves VSI ownership
to M7_0 and gives M33S the ACCESS needed to initialize them during switch setup.

Prepare the Demo
================
1.  Connect 12V~20V power supply to the board.
2.  Connect a micro USB cable between the host PC and the J15(FTDI_DEBUG) USB port
    on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Build the SM image using the modified mx94evk.cfg described in the
    "System Manager configuration" section above.
5.  Program the flash.bin (flash_all target with the modified mx94evk.cfg SM image)
    to SD/eMMC. This image bundles both the netc-share (M33S) and vsi_switch (M7_0)
    firmware.
6.  Connect an Ethernet cable between SWP0 and the host PC.
    Open an Ethernet sniffer (Wireshark / tcpdump) on the host to observe broadcast
    frames.
7.  Switch SW1 to power on the board.

Boot sequence
=============
The M33S core must complete NETC switch and ENETC3 initialization before M7_0 starts
the VSI example. The expected startup order is:

  1. M33S boots and runs netc-share → initializes NETC switch, ENETC3 pseudo port,
     and enables VSI0/1/2 for M7_0 use.
  2. M7_0 boots and runs vsi_switch → waits for M33S (PSI) to signal readiness.
  3. Once the PSI signals completion, M7_0 proceeds with VSI initialization and
     starts sending broadcast frames through the switch.

Running the demo
================
When the demo runs successfully, broadcast frames can be detected on the switch
ports and the serial terminal of M7_0 looks like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MCUX SDK version: 2026.09.00-pvw2
Wait for PSI core finish the switch configuration

NETC VSI Switch example start.

Sending broadcast frames using VSI.

Frame 0, sent!

Frame 1, sent!

Frame 2, sent!

Frame 3, sent!

Frame 4, sent! ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On the host PC (Wireshark / tcpdump), broadcast Ethernet frames sourced from the
VSI MAC address should be visible arriving on the connected switch port.
