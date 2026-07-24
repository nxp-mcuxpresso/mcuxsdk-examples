# PF9453 on MIMXRT2660-EVK

## Hardware requirements

- MIMXRT2660-EVK board
- USB Type-C cable (for power and the debug console)
- J-Link debug probe

The PF9453 PMIC is already mounted on the EVK and connected to the HSP LPI2C1 bus
(SDA = PIO2_24, SCL = PIO2_25). No jumper changes are required; BUCK2 supplies VDD_CORE.

## Board settings

No special board settings. The default OTP configuration presents the PMIC at 7-bit I2C
address `0x32`.

## Serial console

- 115200 baud, 8 data bits, no parity, 1 stop bit, no flow control.

## Expected output

```
PF9453 PMIC driver example

------------------------ PF9453 Menu ------------------------
[1]. Read device ID.
[2]. Set BUCK2 (VDD_CORE) run voltage.
[3]. Configure BUCK2 (run + standby voltage, enable mode).
[4]. Dump BUCK2 registers.
```

Selecting `1` prints the PF9453 device ID (e.g. `DEV_ID = 0xB2 (PF9453 detected ...)`).
