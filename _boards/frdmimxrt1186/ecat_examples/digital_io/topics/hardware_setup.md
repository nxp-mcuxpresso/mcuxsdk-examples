# Hardware setup

-   **Hardware requirements**
    -   FRDM-IMXRT1186 RevC
    -   RJ45 Network cable
    -   Mini/micro USB cable
    -   Personal Computer with TwinCAT3(Windows 10 is recommended)
-   **Board settings**

For multiple boards cascades, remove the R142 register.

-   **Connection**
    1.  Connect EtherCAT Port 0 \(J28 RJ45 interface\) on the FRDM-IMXRT1186 board with TwinCAT3 via the Ethernet cable.
    2.  Connect a USB cable between the host computer and the OpenSDA USB port on the FRDM-IMXRT1186 board.
    3.  Open a serial terminal with the following settings.
        -   115,200 baud rate
        -   8 data bits
        -   No parity
        -   One stop bit
        -   No flow control

