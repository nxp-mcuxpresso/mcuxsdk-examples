# ethosu_apps_rpmsg

## Overview

The example `ethosu_apps_rpmsg` aims to parse the inference request from Cortex-A core,
configure the NPU’s registers, accomplish the inference, and send back the inference result
to Cortex-A core. The inference engine is TensorFlow Lite or TensorFlow Lite Micro (TFLM).

## Toolchains supported

- IAR Embedded Workbench for ARM
- ArmGCC - GNU Tools ARM Embedded
