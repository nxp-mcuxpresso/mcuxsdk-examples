# TFLM Power Consumption Example

## Overview

This example demonstrates power consumption measurement while running TensorFlow Lite Micro (TFLM) inference on RT700. It combines the power mode testing framework with TFLM label image inference to measure power consumption in various low power modes.

## Features

- **Power Modes Supported:**
  - Sleep mode
  - Deep Sleep mode
  - Deep Sleep Retention (DSR) mode
  - Deep Power Down (DPD) mode
  - Full Deep Power Down (FDPD) mode
  - Active test mode (TFLM inference)

- **TFLM Inference:**
  - MobileNet V1 model for image classification
  - Support for NPU (Neutron) acceleration
  - Support for CPU-only inference

## Supported Boards

- [MIMXRT700-EVK](../../_boards/mimxrt700evk/eiq_examples/tflm_power_consumption/example_board_readme.md)

## Requirements

- MCUXpresso SDK with eIQ TFLM middleware
- Neutron NPU support library (for NPU acceleration)

## Building the Example

This example uses sysbuild to build both primary and secondary cores.

### Using West

```bash
west build -b mimxrt700evk/cm33_core0 examples/eiq_examples/tflm_power_consumption/primary
```

## Running the Example

1. Connect the board to your PC
2. Open a serial terminal (115200 baud rate)
3. Flash the application to the board
4. The demo will display a menu with power mode options

## Configuration

The following macros can be modified in `demo_config.h`:

| Macro | Default | Description |
|-------|---------|-------------|
| `TIMING` | 0 | Enable timing measurement |
| `RUN_CPU` | 0 | 0=Use NPU, 1=Use CPU only |
| `CLOCK_HS` | 1 | High speed clock configuration |
| `DELAY` | 0 | Add delay between inference |

## Power Measurement

To measure power consumption:
1. Connect a power analyzer to the board's power rails
2. Run the desired power mode test
3. Record the current consumption

## Notes

- Core1 (Sense core) enters deep sleep mode automatically
- Core0 (Compute core) handles user interaction and TFLM inference
- The example uses PMIC (PCA9422) for power supply management

## Supported Boards
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/eiq_examples/tflm_power_consumption/example_board_readme.md)
