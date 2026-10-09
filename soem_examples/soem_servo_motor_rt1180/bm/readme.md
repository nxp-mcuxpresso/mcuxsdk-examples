# soem_servo_motor_rt1180_bm

## 1. Overview

This example shows how to use the Simple Open EtherCAT MainDevice (SOEM) library to control a
motor over EtherCAT. It runs bare metal (no RTOS) and needs two NXP boards: one running this
SOEM MainDevice example and one running an NXP EtherCAT servo SubDevice example.

| Item | Value |
|------|-------|
| Role | EtherCAT MainDevice (SOEM) |
| Environment | Bare metal |
| Ethernet port | NETC (netc0) |
| SubDevices on the segment | 1 |

Boards in the setup:

| Role | Software | Board |
|------|----------|-------|
| EtherCAT MainDevice | this SOEM example | i.MXRT1180 EVK / FRDM-1186 / i.MX943 EVK |
| EtherCAT SubDevice | `ecat_servo_motor` example | i.MXRT1180 EVK / XSERVO-MTR-DRV RT1180 / i.MX943 EVK |

SubDevice example locations:

| SubDevice board | Location |
|-----------------|----------|
| i.MXRT1180 EVK | https://docs.mcuxpresso.nxp.com/mcuxsdk/latest/examples/ecat_examples/servo_motor/readme.html |
| XSERVO-MTR-DRV RT1180 | https://github.com/nxp-appcodehub/rd-motion-control-slave-servo-mimxrt1180 |
| i.MX943 EVK | https://docs.mcuxpresso.nxp.com/mcuxsdk/latest/examples/ecat_examples/dual_cores_servo_motor/readme.html |

## 2. Expected result

If the test passes, the motor starts to rotate.

## 3. Expected output

```
NETC EP frame loopback example start.
Starting motion task
ec_init on netc0 succeeded.
ec_config_init 0
...
1 slaves found and configured.
ec_config_map_group IOmap:20000664 group:0
>Slave 1, configadr 1001, state  2
...
Slaves mapped, state to SAFE_OP.
Request operational state for all slaves
Calculated workcounter 3
Request operational state for all slaves
Calculated workcounter 3
Operational state reached for all slaves.
```

## 4. Supported boards

| Board | Board specific readme |
|-------|-----------------------|
| MIMXRT1180-EVK | [example_board_readme.md](../../../_boards/evkmimxrt1180/soem_examples/soem_servo_motor_rt1180/bm/example_board_readme.md) |
| FRDM-MIMXRT1186 | [example_board_readme.md](../../../_boards/frdmimxrt1186/soem_examples/soem_servo_motor_rt1180/bm/example_board_readme.md) |
| i.MX943-EVK | [example_board_readme.md](../../../_boards/imx943evk/soem_examples/soem_servo_motor_rt1180/bm/example_board_readme.md) |
