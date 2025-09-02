# Hardware setup

-   **Hardware requirements**
    -   FRDM-IMXRT1186 RevC
    -   RJ45 Network cable
    -   Mini/micro USB cable
    -   Personal Computer with TwinCAT3(Windows 10 is recommended)
-   **Board settings**
    -   J12(2-3)/J13(2-3): eCAT0 Function Via J57A(Default).
    -   J18(2-3)/J17(2-3): eCAT1 Function Via J57B(Default).

-   **Connection**
    1.  RJ45 interface labeled as J57A is the EtherCAT Port0 and RJ45 interface labeled as J57B is the EtherCAT Port1.
    2.  Connect a USB cable between the host computer and the OpenSDA USB port on the FRDM-IMXRT1186 board.
    3.  Open a serial terminal with the following settings.
        -   115,200 baud rate
        -   8 data bits
        -   No parity
        -   One stop bit
        -   No flow control

