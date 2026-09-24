# dual_cores_ecat_servo_motor on i.MX943-EVK

## 1. Hardware requirements

| Item | Note |
|------|------|
| i.MX943-EVK board | Target board |
| FRDM-LVPMSM-FA RevB | Motor control shield (one per motor) |
| Servo motor(s) | See [1.1 Supported motors](#11-supported-motors) |
| RJ45 network cable | Board EtherCAT Port0 to the host PC |
| 24 V 5 A DC power supply | Motor supply |
| 12 V DC power supply | Board supply (connector P1) |
| Host PC with TwinCAT 3 | EtherCAT MainDevice |

### 1.1 Supported motors

| Motor | Integrated encoder |
|-------|--------------------|
| T5PUXS4-H09 | ECN 1325 EnDat 2.2 |
| T5PUXS4-H18 | ECN 1325 EnDat 3 |
| HMD06-011-048-60-OPB1MW230 | BiSS |

Supported combinations:

| Demo | Allowed motor sets |
|------|--------------------|
| One motor | T5PUXS4-H09, or T5PUXS4-H18, or HMD06-011-048-60-OPB1MW230 |
| Two motors | Two T5PUXS4-H09; T5PUXS4-H09 + T5PUXS4-H18; T5PUXS4-H09 + HMD06-011-048-60-OPB1MW230; T5PUXS4-H18 + HMD06-011-048-60-OPB1MW230 |

> **Note:** the i.MX943-EVK board supports only one EnDat 3 motor or one BiSS motor.

## 2. Board settings

### 2.1 i.MX943-EVK board

| Switch | Setting |
|--------|---------|
| SW4 | According to the used BOOT_MODE |
| SW7[1:4] | ON-ON-ON-ON |
| SW8 | All OFF |

Debug console:

* Replace `BOARD_DEBUG_UART_INSTANCE 8` with `BOARD_DEBUG_UART_INSTANCE 1` in `board.h`.
* The board has four serial ports; port1 carries the m70 core output and port2 the m33s core
  output.

### 2.2 FRDM-LVPMSM-FA with T5PUXS4-H09 (EnDat 2.2)

| Setting | Value |
|---------|-------|
| SW90[1:4] | OFF-ON-ON-ON |
| SW30[1:4] | OFF-OFF-ON-OFF |
| Jumper | J300 (1-2) |

Encoder connection:

| Pin name (FRDM-LVPMSM-FA) | Pin location (FRDM-LVPMSM-FA) | Sensor signal name | Pin location (T5PUXS4-H09) |
|---------------------------|-------------------------------|--------------------|----------------------------|
| ENC_CLK_P | J70-2 | CLOCK | 5 |
| ENC_CLK_N | J70-7 | CLOCK- | 6 |
| ENC_DATA_IO_P | J70-4 | DATA | 3 |
| ENC_DATA_IO_N | J70-9 | DATA- | 4 |
| VENC | J70-1 | Vcc (+5 V) | 1 |
| GND | J70-6 | GND | 2 |

### 2.3 FRDM-LVPMSM-FA with T5PUXS4-H18 (EnDat 3)

| Setting | Value |
|---------|-------|
| SW90[1:4] | OFF-ON-ON-ON |
| SW30[1:4] | OFF-OFF-OFF-ON |
| Jumpers | J72 (1-3), J72 (2-4), J73 (1-2), J300 (1-2) |

Encoder connection:

| Pin name (FRDM-LVPMSM-FA) | Pin location (FRDM-LVPMSM-FA) | Sensor signal name | Pin location (T5PUXS4-H18) |
|---------------------------|-------------------------------|--------------------|----------------------------|
| ENC_DATA_IO_VENC_P | J70-5 | P_SD+ | 1 |
| ENC_DATA_IO_VENC_N | J70-10 | P_SD- | 2 |

### 2.4 FRDM-LVPMSM-FA with HMD06-011-048-60-OPB1MW230 (BiSS)

| Setting | Value |
|---------|-------|
| SW90[1:4] | ON-OFF-OFF-ON |
| SW30[1:4] | OFF-ON-OFF-OFF |
| Jumpers | J72 (1-3), J72 (2-4), J73 (1-2), J300 (1-2) |

Encoder connection:

| Pin name (FRDM-LVPMSM-FA) | Pin location (FRDM-LVPMSM-FA) | Sensor signal name | Pin location (HMD06-011-048-60-OPB1MW230) |
|---------------------------|-------------------------------|--------------------|-------------------------------------------|
| ENC_CLK_P | J70-2 | MASTER_CLOCK_INPUT_P | 9 |
| ENC_CLK_N | J70-7 | MASTER_CLOCK_INPUT_N | 11 |
| ENC_DATA_IN_P | J70-3 | SLAVE_DATA_P | 7 |
| ENC_DATA_IN_N | J70-8 | SLAVE_DATA_N | 8 |
| VENC | J70-1 | VDC | 5 |
| GND | J70-6 | GND | 6 |

### 2.5 Motor power connection (all motors)

| FRDM-LVPMSM-FA | Motor cable A |
|----------------|---------------|
| J140 pin 3 | 1 = U |
| J140 pin 2 | 3 = W |
| J140 pin 1 | 4 = V |

## 3. EtherCAT and console connections

| Connector | Function |
|-----------|----------|
| J24B | EtherCAT Port0 - connect this one to the TwinCAT PC |
| J15 (FTDI_DEBUG USB) | Debug console |

Serial terminal settings:

| Parameter | Value |
|-----------|-------|
| Baud rate | 115200 |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Flow control | None |

## 4. Prepare the demo

### 4.1 Assemble the hardware

1. Connect the FRDM-LVPMSM-FA shield (with its motor) to the M1 connector on the front of the
   i.MX943-EVK board.
2. For the two motor demo, connect the second FRDM-LVPMSM-FA shield to the M2 connector on the
   back of the board.
3. Plug the USB cable into the FTDI_DEBUG connector J15 and open the serial terminal
   (see [3. EtherCAT and console connections](#3-ethercat-and-console-connections)).
4. Plug the 12 V DC power supply into connector P1 of the i.MX943-EVK board.
5. Plug the 24 V DC power supply into connector J10 of each FRDM-LVPMSM-FA board.
6. Connect J24B (EtherCAT Port0) to the TwinCAT 3 PC with the RJ45 cable.

### 4.2 Select the encoder type

Set the `M1_ENCODER` and `M2_ENCODER` macros in `mc_periph_init.h`:

| Encoder | M1_ENCODER | M2_ENCODER |
|---------|------------|------------|
| EnDat 2.2 | `ENCODER_ENDAT2P2_2` | `ENCODER_ENDAT2P2_1` |
| EnDat 3 | `ENCODER_ENDAT3` | `ENCODER_ENDAT3` |
| BiSS | `ENCODER_BISS` | `ENCODER_BISS` |

Defaults: `M1_ENCODER = ENCODER_BISS`, `M2_ENCODER = ENCODER_ENDAT3`.

> **Note:** the i.MX943-EVK board supports only one EnDat 3 motor or one BiSS motor.

### 4.3 Build and run

1. Generate the SSC code and apply the patch as described in the
   [common readme](../../../../ecat_examples/dual_cores_servo_motor/readme.md), then build and
   download the images for both cores.
2. Continue with the TwinCAT steps in the
   [common readme](../../../../ecat_examples/dual_cores_servo_motor/readme.md).
