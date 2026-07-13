# ethosu_apps_rpmsg

## Overview

The example `ethosu_apps_rpmsg` aims to parse the inference request from Cortex-A core,
configure the NPU’s registers, accomplish the inference, and send back the inference result
to Cortex-A core. The inference engine is TensorFlow Lite or TensorFlow Lite Micro (TFLM).

## Toolchains supported

- IAR Embedded Workbench for ARM
- ArmGCC - GNU Tools ARM Embedded

## Supported Boards
- [MCIMX93W-EVK](../../../_boards/mcimx93wevk/eiq_examples/ethosu/ethosu_apps_rpmsg/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/eiq_examples/ethosu/ethosu_apps_rpmsg/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/eiq_examples/ethosu/ethosu_apps_rpmsg/example_board_readme.md)