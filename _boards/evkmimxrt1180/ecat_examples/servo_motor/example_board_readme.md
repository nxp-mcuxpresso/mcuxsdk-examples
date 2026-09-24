# ecat_servo_motor on MIMXRT1180-EVK

## 1. Hardware requirements

| Item | Note |
|------|------|
| MIMXRT1180-EVK RevC | Target board |
| FRDM-MC-LVPMSM | Motor control shield |
| Teknic2311P motor | Motor with quadrature encoder |
| RJ45 network cable | Board EtherCAT Port0 to the host PC |
| Mini/micro USB cable | Debug console and flashing |
| Host PC with TwinCAT 3 | EtherCAT MainDevice |

## 2. Board settings

### 2.1 Jumper settings

| Jumper | Setting | Jumper | Setting | Jumper | Setting |
|--------|---------|--------|---------|--------|---------|
| JP1 | 1-2 | J58 | 2-3 | J78 | 1-2 |
| J4 | 1-2 | J59 | 2-3 | J79 | 1-2 |
| J6 | 1-2 | J63 | 1-2 | J90 | 1-2 |
| JP6 | 1-2 | J65 | 1-2 | J91 | 1-2 |
| J9 | 1-2 | J72 | 1-2 | J93 | 1-2 |
| J11 | 2-3 | J73 | 1-2 | J97 | 1-2 |
| J12 | 1-2 | J75 | 2-3 | J98 | 1-2 |
| J14 | 1-2 | J76 | 1-2 | J99 | 1-2 |
| J57 | 2-3 | J77 | 1-2 | J100 | 1-2 |

### 2.2 Zero ohm resistors

| Action | Resistors |
|--------|-----------|
| Add | R747, R749, R760, R763, R767, R1049, R1051 |
| Remove | R104, R105, R107, R126, R142, R146, R148, R1089, R1090 |

> Detailed motor wiring and the motor control user guide are available at
> http://www.nxp.com/sdkmotorcontrol

## 3. EtherCAT and console connections

| Connector | Function |
|-----------|----------|
| J28 | EtherCAT Port0 - connect this one to the TwinCAT PC |
| OpenSDA USB | Debug console and flashing |

Serial terminal settings:

| Parameter | Value |
|-----------|-------|
| Baud rate | 115200 |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Flow control | None |

## 4. Optional: EEPROM LPI2C emulator

i.MX RT1180 can emulate the ESC EEPROM in software. **Skip this chapter if you use the on board
EEPROM hardware.**

1. Wiring:

   | From | To |
   |------|----|
   | J51 pin 4 | J44 pin 20 |
   | J51 pin 6 | J44 pin 18 |

2. Remove the EEPROM device U48.
3. Define the `EEPROM_I2C_EMULATOR` macro in the project to enable the emulator.
4. Continue with [5. Prepare the demo](#5-prepare-the-demo).

## 5. Prepare the demo

1. Connect the Teknic2311P motor to the FRDM-MC-LVPMSM shield.
2. Plug the FRDM-MC-LVPMSM shield onto the motor control interface of the MIMXRT1180-EVK board.
3. Connect J28 (EtherCAT Port0) to the TwinCAT 3 PC with the RJ45 cable.
4. Connect the OpenSDA USB port to the host PC and open the serial terminal
   (see [3. EtherCAT and console connections](#3-ethercat-and-console-connections)).
5. Generate the SSC code and apply the patch as described in the
   [common readme](../../../../ecat_examples/servo_motor/readme.md), then build and download
   the program.
6. Press the reset button or launch the debugger to start the demo.
7. Continue with the TwinCAT steps in the
   [common readme](../../../../ecat_examples/servo_motor/readme.md).
