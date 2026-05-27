# rpmsg_lite_pingpong_dsp

## Overview

The Multicore RPMsg-Lite pingpong DSP project is a demonstration program that uses the
MCUXpresso SDK software and the RPMsg-Lite library and shows how to implement the inter-core
communication between multiple CM cores and DSP of the asymmetric multicore system.

The primary core releases the secondary core and DSP Core from the reset.
Then the inter-core communication is established between Primary Core and Secondary core and
between Primary core and DSP core.

The secondary core releases the other DSP from the reset.
Then the inter-core communication is established between Secondary Core and Secondary core

When using `CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM` the example will change the DSP cores 
communication channels. It will use primary core to send messages to Hifi1 core and 
secondary core will send messages to Hifi4 core.

Once the RPMsg is initialized and endpoints are created the message exchange starts, incrementing a
virtual counter that is part of the message payload.

The message pingpong uses following counter values and increments them:
0 to 99 for Primary to Secondary RPSMG-Lite communication.
100 to 199 for Primary to DSP RPSMG-Lite communication.
200 to 299 for Secondary to DSP RPSMG-Lite communication.

## Shared memory usage

This multicore example uses the shared memory for data exchange. The shared memory region is
defined and the size can be adjustable in the linker file.
The shared memory region start address and the size have to be defined in linker file for each core equally.
This shared memory is split in half, first half is used between Primary Core and Secondary core and
second half is used between Primary Core and DSP.

The other shared memory is created in linker file of Secondary Core for communication between other DSP core.
This memory is used whole.

## Building the application

This shows example how to build application for `mimxrt700evk` board with `cm33_core0 `core_id.
Change the `-b <board>`parameter based on board you want to build.
Change the `-Dcore_id=<core_id>` parameter based on board core you want to build. 
For these parameters please see attribute `boards:`in `primary/example.yml`.

```bash
west build -b mimxrt700evk examples/multicore_examples/rpmsg_lite_pingpong_dsp/primary/ -Dcore_id=cm33_core0 -p always --sysbuild --config flash_debug
```

To build the switched communication use the config option `CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM`.

```bash
west build -b mimxrt700evk examples/multicore_examples/rpmsg_lite_pingpong_dsp/primary/ -Dcore_id=cm33_core0 -p always --sysbuild --config flash_debug -DCONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM=y -Drpmsg_lite_pingpong_dsp_secondary_core_CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM=y -Drpmsg_lite_pingpong_dsp_hifi4_CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM=y -Drpmsg_lite_pingpong_dsp_hifi1_CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM=y
```

## Supported Boards

- [MIMXRT700-EVK](../../_boards/mimxrt700evk/multicore_examples/rpmsg_lite_pingpong_dsp/example_board_readme.md)
- [FRDM-IMXRT700](../../_boards/frdmimxrt700/multicore_examples/rpmsg_lite_pingpong_dsp/example_board_readme.md)
