# soem_servo_motor_bm

## 1. Overview

This example shows how to use the Simple Open EtherCAT MainDevice (SOEM) library to control
commercial EtherCAT servo drives. It runs bare metal (no RTOS).

| Item | Value |
|------|-------|
| Role | EtherCAT MainDevice (SOEM) |
| Environment | Bare metal |
| Ethernet port | NETC (netc0) |
| SubDevices on the segment | 2 servo drives |

Servo drives used:

| Servo drive | Quantity |
|-------------|----------|
| DELTA_ASDA_B3 | 1 |
| INOVANCE_SV680 | 1 |

## 2. Expected output

```
NETC EP frame loopback example start.
Starting motion task
ec_init on netc0 succeeded.
ec_config_init 0
...
2 slaves found and configured.
ec_config_map_group IOmap:20000664 group:0
>Slave 1, configadr 1001, state  2
...
SM programming
SM2 Type:3
StartAddr:1200 Flags:   10064
...
Slaves mapped, state to SAFE_OP.
Request operational state for all slaves
Calculated workcounter 6
Request operational state for all slaves
Calculated workcounter 6
Operational state reached for all slaves.
```

> **Note:** for detailed SOEM performance data, refer to the EtherCAT benchmark document.

## 3. Supported boards

| Board | Board specific readme |
|-------|-----------------------|
| MIMXRT1180-EVK | [example_board_readme.md](../../../_boards/evkmimxrt1180/soem_examples/soem_servo_motor/bm/example_board_readme.md) |
| FRDM-MIMXRT1186 | [example_board_readme.md](../../../_boards/frdmimxrt1186/soem_examples/soem_servo_motor/bm/example_board_readme.md) |
| i.MX943-EVK | [example_board_readme.md](../../../_boards/imx943evk/soem_examples/soem_servo_motor/bm/example_board_readme.md) |
