# tflm_kws for FRDMIMXRT700

## Overview

This example demonstrates keyword spotting using TensorFlow Lite Micro with the Neutron NPU on FRDMIMXRT700 board.

## Model

The example uses a DS-CNN (Depthwise Separable Convolutional Neural Network) model compiled for the Neutron NPU.
The CM33 core offloads inference to the Neutron NPU for acceleration.

## Building

```bash
# armgcc flash_release
west build -p always -b frdmimxrt700 examples/eiq_examples/tflm_kws \
    --toolchain armgcc -Dcore_id=cm33_core0 --config flash_release -d build

# iar flash_release
west build -p always -b frdmimxrt700 examples/eiq_examples/tflm_kws \
    --toolchain iar -Dcore_id=cm33_core0 --config flash_release -d build

# mdk flash_release
west build -p always -b frdmimxrt700 examples/eiq_examples/tflm_kws \
    --toolchain mdk -Dcore_id=cm33_core0 --config flash_release -d build
```

## Running

The example outputs inference time and recognized keywords via UART debug console.

## Board Specific Configuration

- **PMIC**: Uses `BOARD_InitPMICPins()` (uppercase MIC) for PMIC initialization
- **Clock**: 325MHz HSRUN mode with 1.1V VDD2 for optimal NPU performance
- **Pin Configuration**: See `cm33_core0/pin_mux.c` and `cm33_core0/pin_mux.h`
- **NPU**: Neutron NPU runs at 325MHz

## Hardware Requirements

- Type-C USB cable
- FRDMIMXRT700 board
- Personal computer

## Board Settings

1. Example running at 325MHz which requires PMIC power supply.
2. PMIC is required to drive 1.1V on VDD2.

## Prepare the Demo

1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.