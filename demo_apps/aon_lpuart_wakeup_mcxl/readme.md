# aon_lpuart_wakeup_mcxl

## Overview
This demo shows how to use the AON (Always-On) domain LPUART as a wakeup source across
multiple low power modes on the MCXL dual-core platform (CM33 + CM0+). The user selects a
power mode from the CM33 debug console menu, and sends any character on the AON LPUART
(CM0+ debug console) to wake the system back to Active mode.

> **Note:** This example is a **functional demonstration** only. It shows how to configure
> the AON LPUART as a wakeup source for each power mode. The low power mode configurations
> (voltage levels, clock gating, RAM retention, etc.) are **not optimized for lowest power
> consumption**. Do not use the current values measured in this demo as reference data for
> power consumption. For power-optimized configurations, refer to the `power_mode_switch`
> demo instead.

Supported power modes:
- **A** Sleep
- **B** Deep Sleep
- **C** Power Down 1 (PD1)
- **D** Power Down 2 (PD2)
- **E** Deep Power Down 1 (DPD1)
- **F** Deep Power Down 2 (DPD2)

## How It Works
- CM33 (primary core) presents a menu on its debug console (main domain LPUART).
  The user selects a power mode and the system enters that mode.
- The AON LPUART (CM0+ debug console) is configured as the wakeup source.
  Sending any character on the AON LPUART serial port wakes the system back to
  Active mode.
- For Sleep and Deep Sleep, the LPUART NVIC interrupt on CM33 directly wakes
  the core from WFI.
- For PD1, PD2, DPD1, and DPD2, the SMM (Sleep Mode Manager) wakeup controller
  detects the LPUART interrupt signal and triggers the main domain power-up sequence.
- DPD1 and DPD2 do **not** use context saving in this demo. Both modes perform a
  full reboot on wakeup (CM33 re-initializes hardware, CM0+ is reset and re-loaded).

Tips:
Once wakeup from DPD2, the interrupt which causes wakeup should always be handled in
the CM0+ (secondary) project, otherwise the system may not work as expected.

## Supported Boards
