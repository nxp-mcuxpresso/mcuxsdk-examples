# KW47/MCXW72 Low Power Configuration Guide

## Overview

The KW47/MCXW72 offers several low power configuration options that must be carefully tuned based on your board design and application requirements. The main configuration flags are interdependent and affect radio timing, power domain management, and DCDC behavior.

## Key Configuration Parameters

### 1. BOARD_LL_32MHz_WAKEUP_ADVANCE_HSLOT

**Purpose**: Controls how many half-slots (312.5μs each) before a Link Layer interrupt the 32MHz crystal should wake up.

**Default Value**: 3
**Range**: Typically 2-3
**Dependencies**: Affects `BOARD_RADIO_DOMAIN_WAKE_UP_DELAY` timing

```c
#define BOARD_LL_32MHz_WAKEUP_ADVANCE_HSLOT 3
```

**Configuration Guidelines**:
- **Standard crystal boards**: Use `3` (default), `2` (power optimized)
- **FRO 32KHz boards**: Use `3` (mandatory)
- **Custom boards**: May need adjustment based on crystal characteristics

### 2. BOARD_RADIO_DOMAIN_WAKE_UP_DELAY

**Purpose**: Defines the delay (in 30.5μs periods) from 32MHz wake-up to radio domain wake-up.

**Default Value**: 0x10 (16 decimal)
**Dependencies**: Must be coordinated with `BOARD_LL_32MHz_WAKEUP_ADVANCE_HSLOT`

```c
#define BOARD_RADIO_DOMAIN_WAKE_UP_DELAY 0x10U
```

**Configuration Guidelines**:
- **Lower values**: Safer timing but higher power consumption
- **Higher values**: Better power optimization but risk of LL scheduling issues
- **FRO 32KHz boards**: Must use `0x0F` (mandatory)
- **Standard EVK with HSLOT=2**: Can optimize to `0x14` (not safe, needs to be validated)

### 3. BOARD_32KHZ_SRC_CLK_ACCURACY

**Purpose**: Defines the accuracy of the 32KHz clock source, affecting timing precision and power consumption trade-offs.

**Default Value**: Not defined (uses system default)
**Range**: 0-7 (see accuracy table below)
**Dependencies**: Must be set to 0 when using FRO 32KHz

```c
#define BOARD_32KHZ_SRC_CLK_ACCURACY 0
```

**Accuracy Values**:

| Value | Accuracy Range |
|-------|----------------|
| 0 | 251-500 ppm |
| 1 | 151-250 ppm |
| 2 | 101-150 ppm |
| 3 | 76-100 ppm |
| 4 | 51-75 ppm |
| 5 | 31-50 ppm |
| 6 | 21-30 ppm |
| 7 | 0-20 ppm  |

### 4. gBoardDcdcRampTrim_c

**Purpose**: Enables smooth DCDC voltage ramping to avoid current peaks during voltage transitions.

**Default Value**: Disabled (0)
**Range**: 0-7 (0=disabled, 1=smoothest, 7=steepest)
**Dependencies**: Requires `gBoardDcdcEnableHighPowerModeOnNbu_d` to be enabled

```c
#define gBoardDcdcRampTrim_c 3
```

**Configuration Guidelines**:
- **Value 0**: Feature disabled
- **Value 1**: Smoothest ramp, longest transition time
- **Value 3**: Good trade-off between current peak and transition time (recommended)
- **Value 7**: Steepest ramp, shortest transition time
- **Constraint**: Cannot be used with TX power > 7dBm

### 5. gBoardDcdcEnableHighPowerModeOnNbu_d

**Purpose**: Enables dynamic DCDC voltage switching for TX power ≤ 7dBm. Handled by the NBU core.

**Default Value**: Disabled (0)
**Range**: 0 (disabled) or 1 (enabled)
**Dependencies**: Required for `gBoardDcdcRampTrim_c` functionality

```c
#define gBoardDcdcEnableHighPowerModeOnNbu_d 1
```

**Configuration Guidelines**:
- **Limitations**: Cannot handle TX power > 7dBm (requires 2.5V DCDC output)

## Parameter Interdependencies

| Parameter | Constraints |
|-----------|-------------|
| `gBoardDcdcRampTrim_c` | `gBoardDcdcEnableHighPowerModeOnNbu_d` must be enabled |
| `BOARD_RADIO_DOMAIN_WAKE_UP_DELAY` | `BOARD_LL_32MHz_WAKEUP_ADVANCE_HSLOT` needs timing coordination |
| `gBoardUseFro32k_d` | `BOARD_32KHZ_SRC_CLK_ACCURACY` must be 0 (500ppm) |
| `BOARD_32KHZ_SRC_CLK_ACCURACY` | must match actual crystal accuracy

## Quick Reference Table

| Use case | 32MHz wakeup advance hslot | Domain wakeup delay | 32kHz clock accuracy | High Power Mode | Ramp Trim |
|------------|-------|-------|----------|-----------------|-----------|
| Standard EVK | 3 | 0x10 | 0 | Optional | Optional |
| Power Optimized | 2 | 0x10 | 4 | 1 | 3 |
| FRO 32KHz | 3 | 0x0F | 0 | Optional | Optional | 
| High Power (>7dBm) | 2 | 0x10 | 5 | 0 | 0 |disabled |