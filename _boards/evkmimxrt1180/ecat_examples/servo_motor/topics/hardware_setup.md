# Hardware setup

-   **Hardware requirements**
    -   MIMXRT1180-EVK REVC
    -   FRDM-MC-LVPMSM
    -   Teknic 2311P Motor
    -   RJ45 Network cable
    -   Mini/micro universal serial bus \(USB\) cable
    -   Personal Computer on which the TwinCAT3 has been installed
-   **Board settings**

    For multiple boards cascades, remove the R142 register.


|Jumper|Setting|Jumper|Setting|Jumper|Setting|
|------|-------|------|-------|------|-------|
|JP1|1-2|J58|2-3|J78|1-2|
|J4|1-2|J59|2-3|J79|1-2|
|J6|1-2|J63|1-2|J90|1-2|
|JP6|1-2|J65|1-2|J91|1-2|
|J9|1-2|J72|1-2|J93|1-2|
|J11|2-3|J73|1-2|J97|1-2|
|J12|1-2|J75|2-3|J98|1-2|
|J14|1-2|J76|1-2|J99|1-2|
|J57|2-3|J77|1-2|J100|1-2|

For a correct connection, the servo motor application requires you to remove and solder some zero-ohm resistors. Remove and solder zero-ohm resistors according to [Table 2](#table_ec66a671-27fc-496e-9eb1-036b287fb6f5).

|Add resistors|Remove resistors|
|-------------|----------------|
|R747|R767|R104|R146|
|R749|R1049|R105|R148|
|R760|R1051|R107|R1089|
|R763||R126|R1090|
|||R135||

## Hardware assembly 

1.  Connect the FRDM-MC-LVPMSM shield on top of the MIMXRT1180-EVK board.

    **Note:** There is only one possible option.

2.  Connect the 3-phase motor wires to the screw terminals \(J7\) on the Freedom permanent magnet synchronous motor \(PMSM\) power stage.
3.  Plug the 24 V direct current \(DC\) power supply to the DC power connector on the Freedom PMSM power stage.

    ![](../images/image1.svg "Hardware assembly")


## Connection 

1.  Connect EtherCAT Port 0 \(J28 RJ45 Interface\) on the MIMXRT1180-EVK board with TwinCAT3 via the Ethernet cable.
2.  Connect a USB cable between the host PC and the OpenSDA USB port on the MIMXRT1180-EVK board.
3.  Open a serial terminal with the following settings.
    -   115,200 baud rate
    -   8 data bits
    -   No parity
    -   One stop bit
    -   No flow control

