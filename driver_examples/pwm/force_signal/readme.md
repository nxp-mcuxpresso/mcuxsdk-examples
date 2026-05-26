# eFlexPWM Force Signal Update Example

## Overview

This example demonstrates how to update eFlexPWM PWM parameters glitch-free while
the PWM output is running, using the FORCE_OUT mechanism to apply one or more changes
atomically.

The two parameters demonstrated are:

| Parameter | Register | Description |
|-----------|----------|-------------|
| `DTSRCSEL[SMxSEL23]` | PWM Source Select | Selects the signal fed into the deadtime generator (generated, inverted, software, or external) |
| `MCTRL[IPOL]` | Master Control | Selects whether PWM23 (VAL2/VAL3) or PWM45 (VAL4/VAL5) drives the complementary output pair |

Both registers are **double-buffered**: a write goes into a shadow register and
only propagates to the active register when a **FORCE_OUT event** occurs in the
corresponding submodule. This guarantees glitch-free, synchronous updates.

### Demo Sequence

The example cycles through four phases (2 seconds each):

```
Phase 0  Normal            DTSRCSEL=Generated   IPOL=PWM23  30 % duty on PWM_A0
Phase 1  DTSRCSEL update   DTSRCSEL=Inverted    IPOL=PWM23  70% duty (inverted source)
Phase 2  Atomic update     DTSRCSEL=Generated   IPOL=PWM45  30% duty (unchanged), DTSRCSEL+IPOL via 1 FORCE
Phase 3  Restore           DTSRCSEL=Generated   IPOL=PWM23  Back to Phase 0
```

### Waveform Diagram

The GPIO is toggled immediately before each `FLEXPWM_SetLocalForceOut()` call; the GPIO
edge marks the exact instant the FORCE_OUT event applies the
pending shadow register values.

```
              T1: GPIO↓ + FORCE_OUT    T2: GPIO↑ + FORCE_OUT    T3: GPIO↓ + FORCE_OUT
                        |                       |                         |
                        v                       v                         v
GPIO    ────────────────\_______________________/─────────────────────────\__________
(HIGH)       Phase 0    ↓         Phase 1       ↑        Phase 2          ↓  Phase 3

PWM_A0    [30% →  70% at T1]               [ 70% → 30% at T2]      [30%, no change at T3]
           _   _   _    :   _______   _______    :   _   _   _   _   :   _   _   _
(1kHz)    | | | | | |   :  |       | |       |   :  | | | | | | | |  :  | | | | | |
          ┘ └─┘ └─┘ └───:──┘       └─┘       └───:──┘ └─┘ └─┘ └─┘ └──:──┘ └─┘ └─┘ └─
           ←── 30% ────→T1←──────  70% ─────────→T2←────── 30% ──────T3←── 30% ──→
          DTSRCSEL=Generated DTSRCSEL=Inverted     DTSRCSEL=Generated   DTSRCSEL=Generated
          IPOL=PWM23       IPOL=PWM23 (unchanged)  IPOL=PWM45           IPOL=PWM23

PWM_B0    [ 70% →  30% at T1]              [ 30% →  70% at T2]     [ 70%, no change at T3]
           _________  __:   _   _   _   _   _  :   _________   ____:   _________   _
(1kHz)    |         ||  :  | | | | | | | | | | :  |         | |    :  |         | | |
          ┘         └┘  :──┘ └─┘ └─┘ └─┘ └─┘ └:──┘         └─┘   :──┘         └─┘
           ←──  70% ───→T1←──────  30% ────────T2←──────  70% ────T3←──  70% ──→
                               (complement of PWM_A0)
```

## Supported Boards
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/pwm/force_signal/example_board_readme.md)
