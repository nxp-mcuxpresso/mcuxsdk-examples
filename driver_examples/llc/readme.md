# llc

## Overview

The llc example shows how to use the Last Level Cache (LLC) driver.

The common example is memory-type agnostic: it never references PSRAM, SDRAM,
HyperRAM or any specific external-memory technology, and it never hard-codes an
LLC instance or a memory address. Everything platform-specific is supplied at
run time by the board port through a small contract
(`llc_example_platform_t` + `LLC_ExampleGetPlatform` / `LLC_ExamplePlatformInit`
in `llc_example_platform.h`).

Using only public LLC driver APIs, the example:

- reads and prints the LLC version, capabilities and geometry;
- ensures the LLC is enabled before the cacheable region is accessed (it does
  not re-initialize an LLC that boot code already brought up);
- generates cacheable traffic against a board-provided memory region;
- demonstrates clean/invalidate maintenance over an address range and the whole
  cache;
- configures, runs and reads the LLC performance monitor and prints the
  collected counters;
- checks every API return value and prints a final PASS / FAIL.

A new board can support this example by providing a board port (LLC instance,
a valid LLC-cacheable test region, memory attributes and any device-specific
init) without modifying the common source.

## Supported Boards
- [MIMXRT2660-EVK](../../_boards/mimxrt2660evk/driver_examples/llc/example_board_readme.md)
