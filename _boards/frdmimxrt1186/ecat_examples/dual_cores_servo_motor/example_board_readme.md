# dual_cores_ecat_servo_motor on FRDM-IMXRT1186

## 1. Hardware requirements

| Item | Note |
|------|------|
| FRDM-IMXRT1186 | Target board |
| FRDM-EXP-LVPMSM RevB | Motor expansion board |
| FRDM-LVPMSM-FA RevB | Motor control shield (one per motor) |
| TGT3-0065-60-48 motor | Servo motor |
| RJ45 network cable | Board EtherCAT Port0 to the host PC |
| 24 V DC power supply | Motor supply |
| Host PC with TwinCAT 3 | EtherCAT MainDevice |

## 2. Board settings

### 2.1 FRDM-LVPMSM-FA board

| Setting | Value |
|---------|-------|
| SW90[1:4] | OFF-ON-ON-ON |
| SW30[1:4] | OFF-ON-OFF-OFF |
| J300 | 2-3 |

Encoder connection:

| Pin name (FRDM-LVPMSM-FA) | Pin location (FRDM-LVPMSM-FA) | Sensor signal name |
|---------------------------|-------------------------------|--------------------|
| ENC_CLK_P | J70-2 | CLOCK_P |
| ENC_CLK_N | J70-7 | CLOCK_N |
| ENC_DATA_IO_P | J70-4 | DATA_P |
| ENC_DATA_IO_N | J70-9 | DATA_N |
| VENC | J70-1 | UP - power supply voltage |
| GND | J70-6 | GND |

Motor connection:

| FRDM-LVPMSM-FA | Motor cable A |
|----------------|---------------|
| J140 pin 3 | 1 = U |
| J140 pin 2 | 3 = W |
| J140 pin 1 | 4 = V |

### 2.2 FRDM-IMXRT1186 board

Jumper settings: J12 (2-3), J13 (2-3), J17 (2-3), J18 (2-3), J27 (2-3), J29 (2-3),
J30 (2-3), J33 (2-3), J42 (1-2), J49 (2-3)

| Action | Components |
|--------|-----------|
| Add zero ohm resistors | R2, R3, R243, R448, R34, R40, R83 (1-3) |
| Remove resistors | R407 |

### 2.3 FRDM-EXP-LVPMSM board

| Action | Components |
|--------|-----------|
| Add zero ohm resistors | SJ1, SJ2, SJ4, SJ5, SJ6, SJ8, SJ9, SJ10, SJ11, SJ12, SJ13, SJ14, SJ15, SJ16, SJ17, SJ18, SJ19, SJ20, SJ27, SJ28, SJ29, SJ36, SJ37, SJ38, SJ39, SJ42, SJ44, SJ45, SJ47, SJ52, SJ53 |
| Remove resistors | SJ21, SJ23, SJ25, SJ26, SJ31, SJ32, SJ49 |

## 3. EtherCAT and console connections

| Connector | Function |
|-----------|----------|
| J57A | EtherCAT Port0 - connect this one to the TwinCAT PC |
| J23 (FTDI_DEBUG USB) | Debug console |

Serial terminal settings:

| Parameter | Value |
|-----------|-------|
| Baud rate | 115200 |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Flow control | None |

## 4. Prepare the demo

1. Connect the FRDM-EXP-LVPMSM board to the FRDM-IMXRT1186 board.
2. Connect the FRDM-LVPMSM-FA board (with its motor) to the Motor1 interface of the
   FRDM-EXP-LVPMSM board.
3. For the two motor demo, connect the second FRDM-LVPMSM-FA board (with its motor) to the
   Motor2 interface.
4. Plug the USB cable into the FTDI_DEBUG connector J23 and open the serial terminal
   (see [3. EtherCAT and console connections](#3-ethercat-and-console-connections)).
5. Plug the 24 V DC power supply into the J10 connector of each FRDM-LVPMSM-FA board.
6. Connect J57A (EtherCAT Port0) to the TwinCAT 3 PC with the RJ45 cable.
7. Generate the SSC code and apply the patch as described in the
   [common readme](../../../../ecat_examples/dual_cores_servo_motor/readme.md), then build and
   download the images for both cores.
8. Continue with the TwinCAT steps in the
   [common readme](../../../../ecat_examples/dual_cores_servo_motor/readme.md).
