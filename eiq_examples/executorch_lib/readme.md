Overview - ExecuTorch runtime library build project
========

This project builds ExecuTorch runtime library for RT700. Users can debug the
runtime libary as needed.

The new ExecuTorch runtime library With armgcc/mcux toolchain can place the one
in middleware/eiq/executorch/lib/cm33/armgcc/.

## What is ExecuTorch

ExecuTorch is the PyTorch ecosystem on-device inference engine,
ported for the selected NXP MCU platforms, such as RT700.

ExecuTorch is an end-to-end solution for enabling on-device inference
capabilities across mobile and edge devices including wearables, embedded
devices and microcontrollers. It is part of the PyTorch Edge ecosystem and
enables efficient deployment of PyTorch models to edge devices. For more
information, see [ExecuTorch official page](https://docs.pytorch.org/executorch).

Key value propositions of ExecuTorch are:
- Portability: Compatibility with a wide variety of computing platforms,
  from high-end mobile phones to highly constrained embedded systems and
  microcontrollers.
- Productivity: Enabling developers to use the same toolchains and Developer
  Tools from PyTorch model authoring and conversion, to debugging and
  deployment to a wide variety of platforms.
- Performance: Providing end users with a seamless and high-performance
  experience due to a lightweight runtime and utilizing full hardware
  capabilities such as CPUs, NPUs, and DSPs.

## ExecuTorch workflow

The ExecuTorch workflow works in three steps:

1. Exporting your PyTorch model
2. Compiling the exported program to an ExecuTorch program
3. Running the compiled program on the target device using ExecuTorch runtime library

Steps 1 and 2 are being done ahead of time (usually in Python along with your PyTorch model implementation).
To compile the exported program you can use `executorch` package available via `pip install`.
For more detailed and up-to-date instruction on how to compile your models, visit the
[getting started tutorial page](https://docs.pytorch.org/executorch/stable/getting-started.html).

The 3rd step is performed on the target device using ExecuTorch runtime.
To learn more about the ExecuTorch workflow, refer to the
[workflow documentation](https://docs.pytorch.org/executorch/stable/intro-how-it-works).

## Supported Boards
- MIMXRT700-EVK
- FRDM-IMXRT700
