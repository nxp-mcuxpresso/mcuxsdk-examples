# ecat_servo_motor on FRDM-IMXRT1186

## 1. Hardware requirements

| Item | Note |
|------|------|
| FRDM-IMXRT1186 A2 | Target board |
| FRDM-MC-LVPMSM | Motor control shield |
| Teknic2311P motor | Motor with quadrature encoder |
| 24 V power supply | Motor supply |
| RJ45 network cable | Board EtherCAT Port0 to the host PC |
| Type-C USB cable | Debug console and flashing |
| Host PC with TwinCAT 3 | EtherCAT MainDevice |

## 2. Board settings

Jumper settings (all in position 2-3):

| Jumper | Setting | Jumper | Setting |
|--------|---------|--------|---------|
| J12 | 2-3 | J27 | 2-3 |
| J13 | 2-3 | J29 | 2-3 |
| J17 | 2-3 | J30 | 2-3 |
| J18 | 2-3 | J33 | 2-3 |

> Detailed motor wiring and the motor control user guide are available at
> http://www.nxp.com/sdkmotorcontrol

## 3. EtherCAT and console connections

| Connector | Function |
|-----------|----------|
| J57A | EtherCAT Port0 - connect this one to the TwinCAT PC |
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

FRDM-IMXRT1186 can emulate the ESC EEPROM in software. **Skip this chapter if you use the
on board EEPROM hardware.**

1. Rework the board:

   | Action | Components |
   |--------|-----------|
   | Remove jumper | J31 pin1-pin2, J35 pin1-pin2 |
   | Remove resistor | R297 pin1-pin2, R299 pin1-pin2 |
   | Add resistor | R30, R31, R297 pin1-pin3, R299 pin1-pin3 |

2. Wiring:

   | From | To |
   |------|----|
   | J31 pin 1 | J2 pin 18 |
   | J35 pin 1 | J2 pin 20 |

3. Define the `EEPROM_I2C_EMULATOR` macro in the project to enable the emulator.
4. Continue with [5. Prepare the demo](#5-prepare-the-demo).

## 5. Prepare the demo

1. Connect the Teknic2311P motor to the FRDM-MC-LVPMSM shield.
2. Plug the FRDM-MC-LVPMSM shield onto the motor control interface of the FRDM-IMXRT1186 board.
3. Connect J57A (EtherCAT Port0) to the TwinCAT 3 PC with the RJ45 cable.
4. Connect the OpenSDA USB port to the host PC and open the serial terminal
   (see [3. EtherCAT and console connections](#3-ethercat-and-console-connections)).
5. Generate the SSC code and apply the patch as described in the
   [common readme](../../../../ecat_examples/servo_motor/readme.md), then build and download
   the program.
6. Press the reset button or launch the debugger to start the demo.
7. Continue with the TwinCAT steps in the
   [common readme](../../../../ecat_examples/servo_motor/readme.md).
