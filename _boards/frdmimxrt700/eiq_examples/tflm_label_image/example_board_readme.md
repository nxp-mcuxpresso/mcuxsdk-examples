# tflm_label_image for FRDMIMXRT700

## Overview

This example demonstrates image classification using TensorFlow Lite Micro with the Neutron NPU on FRDMIMXRT700 board.

## Model

The example uses a MobileNet V1 model (0.25 128x128 quantized INT8) compiled for the Neutron NPU.

- **Model file**: `pcq_npu/mobilenet_v1_0.25_128_quant_int8_npu.tflite`
- **Model data**: `pcq_npu/model_data.h`
- **Ops file**: `pcq_npu/model_mobilenet_ops_npu.cpp`

## Building

```bash
# armgcc flash_release
west build -p always -b frdmimxrt700 examples/eiq_examples/tflm_label_image \
    --toolchain armgcc -Dcore_id=cm33_core0 --config flash_release -d build

# iar flash_release
west build -p always -b frdmimxrt700 examples/eiq_examples/tflm_label_image \
    --toolchain iar -Dcore_id=cm33_core0 --config flash_release -d build

# mdk flash_release
west build -p always -b frdmimxrt700 examples/eiq_examples/tflm_label_image \
    --toolchain mdk -Dcore_id=cm33_core0 --config flash_release -d build
```

## Running

The example outputs inference time and top-5 classification labels via UART debug console.

## Board Specific Configuration

- **PMIC**: Uses `BOARD_InitPMICPins()` (uppercase MIC) for PMIC initialization
- **Clock**: 325MHz HSRUN mode with 1.1V VDD2 for optimal NPU performance
- **Pin Configuration**: See `cm33_core0/pin_mux.c` and `cm33_core0/pin_mux.h`