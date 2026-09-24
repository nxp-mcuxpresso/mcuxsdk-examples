# soem_servo_motor_bm on FRDM-IMXRT1186

## 1. Hardware requirements

| Item | Note |
|------|------|
| FRDM-IMXRT1186 board | Target board |
| Type-C USB cable | Debug console and flashing |
| RJ45 network cable (standard) | EtherCAT connection |
| Servo drives: DELTA_ASDA_B3 x1, INOVANCE_SV680 x1 | EtherCAT SubDevice |
| Personal computer | Host PC |

## 2. Board settings

| Jumper setting | Function |
|----------------|----------|
| J12 (1-2), J13 (2-3) | ETH0 routed to J56A |
| J18 (1-2), J17 (2-3) | ETH2 routed to J56B |

## 3. Prepare the demo

1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Power up the servo drive(s) and connect them to the target board (J56-A) with the Ethernet cable.
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
