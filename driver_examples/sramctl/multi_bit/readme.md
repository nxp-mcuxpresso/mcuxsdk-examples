# sramctl_multi_bit

## Overview

The SRAMCTL multi-bit example demonstrates SRAMCTL **multi-bit** error injection.

Multi-bit ECC errors are **uncorrectable**. Depending on SoC integration, the CPU read may report `MLTERR` and continue, or it may escalate to a fault/reset/halt.

The purpose of this example is to show how to configure SRAMCTL initialization and multi-bit error injection, and how to observe the uncorrectable ECC indication.

## Supported Boards
- MIMXRT2660-EVK
