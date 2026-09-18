# Power Mode Switch - MIMXRT2662EVK

## Hardware Requirements

- MIMXRT2662EVK board(SCH-96037 REV B1)
- MIMXRT2660-EVK board
- Optional: oscilloscope to measure current consumption

## Wakeup Sources

After picking a low-power mode from the top-level menu, a Level 2 menu offers
the wakeup sources that mode supports - not every source is available in
every mode:

| Wakeup Source            | Sleep | DS1 | DS2 | DS3 | PD | DPD1/2 |
|--------------------------|:-----:|:---:|:---:|:---:|:--:|:------:|
| UART RX                  |   x   |     |     |     |    |        |
| WAKE GPIO / SW6          |   x   |  x  |  x  |  x  |  x |        |
| AON GPIO / SW5           |   x   |  x  |  x  |  x  |  x |   x    |
| VBAT LPTMR               |   x   |  x  |  x  |  x  |  x |   x    |
| RTC alarm                |   x   |  x  |  x  |  x  |  x |   x    |
| DMA (wake on complete)   |   x   |  x  |     |     |    |        |

- SW5 = `PIO0_4` (VBAT/AON domain GPIO, `VBAT_GPIO_CH0_IRQn`); also the PMIC
  on/off button. Available down to Deep Power Down because it is wired into
  the always-on VBAT domain.
- SW6 = `PIO1_0` (WAKE domain GPIO, `WAKE_GPIO_CH0_IRQn`). Not available in
  Deep Power Down - the WAKE domain is off there.
- DMA (wake on complete) arms a WAKE_EDMA3 CH0 memory-to-memory transfer that
  a single WAKE_LPTMR_0 compare triggers in one shot; the transfer's completion
  IRQ is the wakeup. Limited to Sleep and Deep Sleep 1, the two modes that keep
  the WAKE domain alive.

**Note**: Run mode switches (menu keys `1`-`3`) take effect immediately and do
not require a wakeup source.

## Expected Console Output

```
Power Mode Switch Example Started...

==============================================
   RT2660 Power Mode Switch Demo
   Current mode: HP Run
==============================================
   1. Normal Run             (NP - 0.8 V/FBB, 800 MHz)
   2. HP Run                 (HP - 0.9 V/FBB, 1 GHz)
   3. Low Performance Run    (LP - 0.8 V/ZBB, 600 MHz)
   4. Sleep
   5. Deep Sleep 1           (all domains retained)
   6. Deep Sleep 2           (CPU retained; NPU/COMM/MEDIA off)
   7. Deep Sleep 3           (all domains off)
   8. Power Down             (DCDC OFF)
   9. Deep Power Down 1      (VBAT SRAM retained)
   A. Deep Power Down 2      (VBAT SRAM lost)
==============================================
  Select (1-9, A):
```

Picking a low-power mode (`4`-`A`) opens the Level 2 wakeup-source menu, e.g. for Sleep:

```
>>> Sleep

  Wakeup source:
   1. UART RX
   2. WAKE GPIO / SW6
   3. AON GPIO / SW5
   4. VBAT LPTMR
   5. RTC alarm
   6. DMA (wake on complete)
   0. Back
  Select:
```

After picking Sleep + VBAT LPTMR (returns to the menu on wakeup):

```
Transition: HP Run -> Sleep
  [step 1] HP Run -> Sleep  (WFI entered)
  [wake event: VBAT LPTMR]
  [step 2] Sleep -> HP Run  (wake; run mode preserved)

Current mode: HP Run
```

After picking Power Down + RTC alarm (does not return - wakeup is a reset):

```
Transition: HP Run -> Power Down
  [step 1] HP Run -> Power Down  (WFI entered; system resets on wakeup)

Power Mode Switch Example Started...
Boot from Power Down wakeup (RSTSTAT_STICKY=0x00080100)
  wakeup reason: RTC alarm
  VBATCON EVENTS=0x00000000
```

After picking Deep Power Down 1 + VBAT LPTMR (also resets on wakeup):

```
Transition: HP Run -> Deep Power Down 1
  [step 1] HP Run -> Deep Power Down 1  (VBAT SRAM retained; VBAT-domain wakeup; no return)

Power Mode Switch Example Started...
Boot from Deep Power Down wakeup (RSTSTAT_STICKY=0x00000112)
  wakeup reason: VBAT LPTMR
  VBATCON EVENTS=0x00000010
VBAT_SRAM check: marker found -> RETAINED
```
