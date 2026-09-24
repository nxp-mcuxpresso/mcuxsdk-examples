# soem_gpio_pulse_netc_bm

## 1. Overview

This example shows how to use the Simple Open EtherCAT MainDevice (SOEM) library on a NETC
based Ethernet port to communicate with EtherCAT devices. It runs bare metal (no RTOS).

| Item | Value |
|------|-------|
| Role | EtherCAT MainDevice (SOEM) |
| Environment | Bare metal |
| Ethernet port | NETC (enet0) |
| EtherCAT devices on the segment | 3 |

EtherCAT network:

| Position | Device | Function |
|----------|--------|----------|
| Coupler | BECKHOFF EK1100 | EtherCAT coupler |
| SubDevice0 | BECKHOFF EL2008 | Digital output terminal |
| SubDevice1 | BECKHOFF EL1018 | Digital input terminal |

The example drives a stepper motor system through two outputs and one input:

| Direction | Signal | Description |
|-----------|--------|-------------|
| Output | Pulse | Pulse signal, 200 us period, 50 % duty cycle |
| Output | Dir | Direction signal |
| Input | Limit | Comes from the limit switches of the stepper motor system; the Dir signal is toggled once each time a limit switch is touched |

## 2. Expected result

* On success, LED2 of SubDevice0 lights up at 50 % brightness.
* If the stepper motor system is not set up properly, check the console log and the LED4 state
  of SubDevice0.

## 3. Expected output

```
Start the soem_gpio_pulse baremetal example...
ec_init on enet0 succeeded.
ec_config_init 0
3 slaves found and configured.
ec_config_map_group IOmap:80000089 group:0
>Slave 1, configadr 1001, state 12
>Slave 2, configadr 1002, state 12
>Slave 3, configadr 1003, state 12
SII Isize:0
SII Osize:0
ISIZE:0 0 OSIZE:0
SM programming
SII Isize:0
SII Osize:8
SM0 length 8
ISIZE:0 0 OSIZE:8
SM programming
SM0 Type:3 StartAddr: f00 Flags:   90044
SII Isize:8
SM0 length 8
SII Osize:0
ISIZE:8 8 OSIZE:0
SM programming
SM0 Type:4 StartAddr:1000 Flags:   10000
OUTPUT MAPPING
FMMU 0
SM0
slave 2 Outputs 80000089 startbit 0
=Slave 3, INPUT MAPPING
FMMU 0
SM0
Inputs 8000008A startbit 0
IOmapSize 2
Slaves mapped, state to SAFE_OP.
oloop = 1, iloop = 1
segments : 1 : 2 0 0 0
Request operational state for all slaves
Calculated workcounter 3
Operational state reached for all slaves.
```

## 4. Supported boards

| Board | Board specific readme |
|-------|-----------------------|
| MIMXRT1180-EVK | [example_board_readme.md](../../../_boards/evkmimxrt1180/soem_examples/soem_gpio_pulse/bm/example_board_readme.md) |
| FRDM-MIMXRT1186 | [example_board_readme.md](../../../_boards/frdmimxrt1186/soem_examples/soem_gpio_pulse/bm/example_board_readme.md) |
| i.MX943-EVK | [example_board_readme.md](../../../_boards/imx943evk/soem_examples/soem_gpio_pulse/bm/example_board_readme.md) |
