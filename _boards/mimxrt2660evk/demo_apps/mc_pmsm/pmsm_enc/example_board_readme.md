# PMSM FOC with Encoder (pmsm_enc) on MIMXRT2660-EVK

## Overview

This example demonstrates Field-Oriented Control (FOC) of a three-phase
Permanent Magnet Synchronous Motor (PMSM) with quadrature encoder feedback
running on the NXP MIMXRT2660-EVK board (i.MX RT2660, Cortex-M85 @ 1000 MHz).

The Motor Identification (MID) algorithm is included to allow auto-tuning of
motor parameters without manual measurement.

FreeMASTER is used for real-time variable monitoring, recorder data capture,
and runtime parameter adjustment via the MCAT 2.0 motor control application
tuning tool.

## Supported Boards

- MIMXRT2660-EVK (mimxrt2660evk)

## Hardware Requirements

- MIMXRT2660-EVK board
- NXP MCTRL-LVHB motor control shield (or equivalent 3-phase inverter)
- Supported PMSM motor with quadrature encoder (default: Teknic M-2311P-LN-08D)
- USB-UART bridge (LPUART1, J-Link on-board CDC or external)
- FreeMASTER desktop application

## Peripheral Mapping

| Function       | Peripheral  | Notes                                  |
|----------------|-------------|----------------------------------------|
| 3-phase PWM    | PWM1 SM0-2  | 16 kHz center-aligned, dead-time 1 us  |
| Phase A/B ADC  | ADC1        | HW triggered by PWM1 VAL4             |
| Phase C ADC    | ADC2        | HW triggered by PWM1 VAL4             |
| Slow loop timer| TMR1 CH0    | 1 kHz (1 ms period)                   |
| Encoder        | EQDC1       | A/B channels, modulo mode              |
| DC bus OCP     | CMP3        | Analog comparator for fault protection |
| FreeMASTER UART| LPUART1     | 115200 baud, poll mode                 |
| User button    | SW5 / GPIO  | HSP_GPIO1 pin 4, start/stop demo       |

## Building and Running

```bash
west build -p always mcuxsdk/examples/demo_apps/mc_pmsm/pmsm_enc \
    --toolchain iar --config debug -b mimxrt2660evk
```

Flash the resulting binary to the board and connect FreeMASTER to LPUART1 at
115200 baud to access the MCAT 2.0 motor tuning interface.

## Notes

- Motor parameters are pre-configured for the Teknic M-2311P-LN-08D (M1)
  and Linix 45ZWN24-40 (M2) motors via `m1_pmsm_appconfig.h` and
  `m2_pmsm_appconfig.h`.
- Press SW5 to start/stop the speed demo mode.
- See the Motor Control Application User Guide for full tuning instructions.
