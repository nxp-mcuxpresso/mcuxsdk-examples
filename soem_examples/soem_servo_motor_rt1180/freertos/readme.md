# soem_servo_motor_rt1180_freertos

## 1. Overview

This example shows how to use the Simple Open EtherCAT MainDevice (SOEM) library to control a
motor over EtherCAT. It runs on FreeRTOS and needs two NXP EVK boards: one running this SOEM
MainDevice example and one running the NXP EtherCAT servo SubDevice example.

| Item | Value |
|------|-------|
| Role | EtherCAT MainDevice (SOEM) |
| Environment | FreeRTOS |
| Ethernet port | NETC (netc0) |
| SubDevices on the segment | 1 |

Boards in the setup:

| Role | Software | Board |
|------|----------|-------|
| EtherCAT MainDevice | this SOEM example | i.MXRT1180 EVK / FRDM-1186 / i.MX943 EVK |
| EtherCAT SubDevice | `ecat_servo_motor` example | i.MXRT1180 EVK |

> **Note:** `ecat_servo_motor` path: `.../boards/evkmimxrt1180/ecat_examples/servo_motor`

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

## 4. Notes

* Set the GPT timer frequency to 200 MHz or higher to get a more accurate timer.
* For detailed SOEM performance data, refer to the EtherCAT benchmark document.

## 5. Supported boards

No board specific readme is linked for this configuration yet.
