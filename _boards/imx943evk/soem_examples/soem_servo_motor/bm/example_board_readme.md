# soem_servo_motor_bm on i.MX943-EVK

## 1. Hardware requirements

| Item | Note |
|------|------|
| i.MX943-EVK board | Target board |
| Mini/micro USB cable | Debug console and flashing |
| RJ45 network cable (standard) | EtherCAT connection |
| Servo drives: DELTA_ASDA_B3 x1, INOVANCE_SV680 x1 | EtherCAT SubDevice |
| Personal computer | Host PC |

## 2. Board settings

No special settings are required.

## 3. Prepare the demo

1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Power up the servo drive(s) and connect them to the target board (J26) with the Ethernet cable.
3. Open a serial terminal with the following settings:

| Parameter | Value |
|-----------|-------|
| Baud rate | 115200 |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Flow control | None |

4. Write the program into the flash of the target board.
5. Start the demo and test the SOEM performance.
