# Hardware setup

-   **Hardware requirements**
    -   FRDM-IMXRT1186 REVC
    -   FRDM-MC-LVPMSM
    -   Teknic 2311P Motor
    -   RJ45 Network cable
    -   Mini/micro universal serial bus \(USB\) cable
    -   Personal Computer on which the TwinCAT3 has been installed(Windows 10 is recommended)
-   **Board settings**
    -   J12(2-3)/J13(2-3): eCAT0 Function Via J57A(Default).
    -   J18(2-3)/J17(2-3): eCAT1 Function Via J57B(Default).
    -   Add zero resistors: R15, R17, R40, R21, R243, R448
    -   Set jumpers: J30 2-3, J33 2-3, J29 2-3, J27 2-3 

## Hardware assembly 

1.  Connect the FRDM-MC-LVPMSM shield on top of the FRDM-IMXRT1186 board.

    **Note:** There is only one possible option.

2.  Connect the 3-phase motor wires to the screw terminals \(J7\) on the Freedom permanent magnet synchronous motor \(PMSM\) power stage.
3.  Plug the 24 V direct current \(DC\) power supply to the DC power connector on the Freedom PMSM power stage.

    ![](../images/image1.svg "Hardware assembly")


## Connection 

1.  RJ45 interface labeled as J57A is the EtherCAT Port0 and RJ45 interface labeled as J57B is the EtherCAT Port1.
2.  Connect a USB cable between the host PC and the OpenSDA USB port on the FRDM-IMXRT1186 board.
3.  Open a serial terminal with the following settings.
    -   115,200 baud rate
    -   8 data bits
    -   No parity
    -   One stop bit
    -   No flow control

