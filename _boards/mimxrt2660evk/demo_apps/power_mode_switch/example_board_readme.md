# Power Mode Switch - MIMXRT2662EVK

## Hardware Requirements

- MIMXRT2662EVK board
- USB cable (for debug console, J7)
- Optional: oscilloscope to measure current consumption

## Wakeup Sources

| Power Mode       | Key | Wakeup Source                         | Action                     |
|------------------|-----|---------------------------------------|----------------------------|
| Sleep            | S   | LPUART0 RX interrupt (IRQ 184)        | Press any UART key         |
| Deep Sleep 1     | D   | SW5 / GPIO1_4 (HSP_GPIO1_CH0_IRQn)   | Press SW5 user button      |
| Deep Sleep 2     | E   | SW5 / GPIO1_4 (HSP_GPIO1_CH0_IRQn)   | Press SW5 user button      |
| Deep Sleep 3     | F   | SW5 / GPIO1_4 (HSP_GPIO1_CH0_IRQn)   | Press SW5 user button      |
| Power Down       | P   | SW5 async (WAKE domain GPIO)          | Press SW5 user button      |
| Deep Power Down 1| G   | AON GPIO pad 0 (kPOWER_WakeupTypeAonGpio) | Drive AON GPIO pad 0   |
| Deep Power Down 2| H   | AON GPIO pad 0 (kPOWER_WakeupTypeAonGpio) | Drive AON GPIO pad 0   |

**Note**: Run mode switches (A/B/C) take effect immediately and do not require a wakeup source.

## Expected Console Output

```
Cold boot.

########## RT2660 Power Mode Switch ##########
  Current run mode: Over Drive Run FBB  (0.9V, up to 1 GHz)
----------------------------------------------
  A - Over Drive Run FBB    (switch run mode; 0.9V/FBB, up to 1 GHz)
  B - Normal Drive Run FBB  (switch run mode; 0.8V/FBB, up to 800 MHz)
  C - Normal Drive Run ZBB  (switch run mode; 0.8V/ZBB, up to 500 MHz)
  S - Sleep                 (wakeup: press any UART key)
  D - Deep Sleep 1          (wakeup: press SW5 button)
  E - Deep Sleep 2          (wakeup: press SW5 button)
  F - Deep Sleep 3          (wakeup: press SW5 button)
  P - Power Down            (wakeup: press SW5; system resets)
  G - Deep Power Down 1     (wakeup: VBAT LPTMR auto-wake / on-off button; VBAT SRAM retained; system resets)
  H - Deep Power Down 2     (wakeup: VBAT LPTMR auto-wake / on-off button; VBAT SRAM lost; system resets)
##############################################
Select power mode:
```

After pressing `S` then any UART key:
```
Entering Sleep... (press any UART key to wake up)
Woke from Sleep.
```

After pressing `D` then SW5:
```
Entering Deep Sleep 1... (press SW5 to wake up)
Woke from Deep Sleep 1.
```

After pressing `P` (system resets on wakeup):
```
Entering Power Down... system resets on wakeup.
Press SW5 to wake (WAKE domain GPIO async wakeup).
[... system resets ...]
Woke from Power Down (GPR token = 0x1).
```
