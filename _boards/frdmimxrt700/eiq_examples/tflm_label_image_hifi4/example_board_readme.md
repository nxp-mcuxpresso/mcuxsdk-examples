# tflm_label_image_hifi4 for FRDMIMXRT700

## Overview

This example demonstrates image classification using TensorFlow Lite Micro with the HiFi4 DSP on FRDMIMXRT700 board.

## Model

The example uses a MobileNet V1 model (0.25 128x128 quantized INT8) compiled for the HiFi4 DSP.
The CM33 core offloads inference to the HiFi4 DSP for acceleration.

## Building

```bash
# armgcc flash_release
west build -p always --sysbuild examples/eiq_examples/tflm_label_image_hifi4/cm \
    --toolchain armgcc -b frdmimxrt700 -Dcore_id=cm33_core0 --config flash_release -d build

# iar flash_release
west build -p always --sysbuild examples/eiq_examples/tflm_label_image_hifi4/cm \
    --toolchain iar -b frdmimxrt700 -Dcore_id=cm33_core0 --config flash_release -d build

# mdk flash_release
west build -p always --sysbuild examples/eiq_examples/tflm_label_image_hifi4/cm \
    --toolchain mdk -b frdmimxrt700 -Dcore_id=cm33_core0 --config flash_release -d build
```

## Running

The example outputs inference time and top-5 classification labels via UART debug console.

## Board Specific Configuration

- **PMIC**: Uses `BOARD_InitPMICPins()` (uppercase MIC) for PMIC initialization
- **Clock**: 325MHz HSRUN mode with 1.1V VDD2 for optimal DSP performance
- **Pin Configuration**: See `cm33_core0/pin_mux.c` and `cm33_core0/pin_mux.h`
- **DSP**: HiFi4 DSP runs at 325MHz, DSP binary images are loaded from `hifi4/binary/`