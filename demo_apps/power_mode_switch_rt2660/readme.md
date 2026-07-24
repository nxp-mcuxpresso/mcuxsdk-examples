# Power Mode Switch - RT2660

## Overview

This demo exercises all 10 power modes supported by the RT2660:

| # | Mode name | Key | Category | Returns? |
|---|-----------|-----|----------|----------|
| 1 | Over Drive Run FBB | A | Run | — |
| 2 | Normal Drive Run FBB | B | Run | — |
| 3 | Normal Drive Run ZBB | C | Run | — |
| 4 | Sleep | S | Standby | Yes |
| 5 | Deep Sleep 1 | D | Standby | Yes |
| 6 | Deep Sleep 2 | E | Standby | Yes |
| 7 | Deep Sleep 3 | F | Standby | Yes |
| 8 | Power Down | P | Standby | No (PoR) |
| 9 | Deep Power Down 1 | G | Standby | No (PoR) |
| 10 | Deep Power Down 2 | H | Standby | No (PoR) |

A menu-driven console interface lets the user select the target mode. After
each Sleep or Deep Sleep entry the example reports the wakeup event and
returns to the menu. After Power Down or Deep Power Down the system resets;
the boot reason is detected by reading the CMC reset-status register at
startup and the appropriate message is printed.

## Driver Stack

```
fsl_power (coordinator — this example)
  ├── fsl_pmu       (PMU body bias, DCDC, LDO configuration)
  ├── fsl_powercon  (CMC/SSC step sequencer, P-Channel, wakeup masks)
  ├── fsl_pdcon     (power domain events per standby mode)
  └── fsl_memcon    (OCRAM0 / TCM retention sizing)
```

## Key APIs Used

| API | Purpose |
|-----|---------|
| `POWER_GetDefaultInitConfig()` | Fill `power_init_config_t` with reset defaults |
| `POWER_Init()` | One-time power subsystem initialisation |
| `POWER_EnterHpRun()` | Switch to Over Drive Run (0.9 V, FBB) |
| `POWER_EnterNormalRun()` | Switch to Normal Drive Run (0.8 V, FBB) |
| `POWER_EnterLpRun()` | Switch to Low Power Run (0.8 V, ZBB) |
| `POWER_EnableWakeupSource()` | Accumulate wakeup source before standby entry |
| `POWER_EnterSleep()` | Enter Sleep; returns on wakeup |
| `POWER_EnterDeepSleep()` | Enter Deep Sleep 1/2/3; returns on wakeup |
| `POWER_EnterPowerDown()` | Enter Power Down; does not return |
| `POWER_EnterDeepPowerDown()` | Enter Deep Power Down 1/2; does not return |

## Run Mode Notes

- `POWER_EnterHpRun()` / `POWER_EnterNormalRun()` / `POWER_EnterLpRun()`
  adjust VDD_CORE, DCDC, and body bias only.  The application is responsible
  for updating CCM clock frequencies so that the CPU and bus clocks stay
  within the new voltage rail's operating range.  Recommended order: reduce
  frequency first when going HP→LP; increase voltage first (by calling the
  target-mode entry API) when going LP→HP.
- Transitioning HP Run <--> LP Run must pass through Normal Run (ZBB).
  The driver handles the intermediate step automatically.
- FBB body bias requires valid OTP trim values for `nwellVolSel` /
  `pwellVolSel` in `pmu_body_bias_config_t`. Leaving both at zero produces
  ZBB-equivalent bias. This demo leaves them at zero (ZBB bias) because
  board-specific trim data is not yet available.

## Power Down / Deep Power Down Notes

- These modes do not return. Wakeup triggers a Power-On-Reset.
- Power Down wakeup via the SW6 button uses the WAKE domain GPIO async path.
  The GPIO async mode register (GPIO_INTTYPE_ASYNC per RM) must be configured
  before entry; this is TBD pending RT2660 GPIO async documentation.
- Deep Power Down 1/2 wakeup is driven from the always-on VBAT domain. The DPD
  Level 2 menu lets you pick between two VBAT-domain sources: the **VBAT LPTMR**
  (clocked from ULP32K, ~32.768 kHz) armed before entry so the device auto-wakes
  after the programmed interval with no external stimulus, or **SW5** (PIO0_4 =
  VBAT/AON GPIO, also the PMIC on/off button) whose edge asserts the VBATCON
  wakeup request. `POWER_EnableWakeupSource()` does not apply in DPD (the
  WAKE/MAIN domains it masks are off).
- DPD2 removes the VDD_PMU rail; the VBAT domain (and hence the VBAT LPTMR)
  stays powered, so timer wakeup still works.

## Pre-defined Deep Sleep Configs

The driver (fsl_power) provides three `const power_deep_sleep_config_t`
objects covering the three DS variants from the RT2660 Power Mode Spec:

| Constant | DCDC | LDO_VDD_0V8 | CPU domain | NPU/COMM/MEDIA |
|----------|------|-------------|------------|----------------|
| `POWER_DEEP_SLEEP_CONFIG_DS1` | PWM  | HP | On       | On‡       |
| `POWER_DEEP_SLEEP_CONFIG_DS2` | PFM | LP | On       | Power off  |
| `POWER_DEEP_SLEEP_CONFIG_DS3` | PFM | LP | Power off  | Power off  |

† PFM mode is tentative pending silicon characterisation.
‡ RT2660 does not integrate PDCON domain retention (omitted for die size); a domain is only
  ever On or powered off. DS1/DS2 keep these domains On — Deep Sleep power saving comes from PMU
  analog/clock state, not from domain retention. (Memory/SRAM retention is a separate feature.)


## Supported Boards

- [MIMXRT2660-EVK](../../_boards/mimxrt2660evk/demo_apps/power_mode_switch/example_board_readme.md)
