# rpmsg_lite_pingpong_rtos_no_mcmgr

## Overview

The Multicore RPMsg-Lite pingpong FreeRTOS project is a simple demonstration program that uses the
MCUXpresso SDK software and the RPMsg-Lite library and shows how to implement the inter-core
communication between cores of the multicore system without multicore manager.

Once the RPMsg is initialized and endpoints are created the message exchange starts, incrementing a
virtual counter that is part of the message payload.

The message pingpong finishes when the counter reaches the value of 100.
Then the RPMsg-Lite is deinitialized and the procedure of the data exchange is repeated again.

## Shared memory usage

This multicore example uses the shared memory for data exchange. The shared memory region is
defined and the size can be adjustable in the linker file.
The shared memory region start address and the size have to be defined in linker file for each core equally.
The shared memory start address is then exported from the linker to the application.

## Building the application

This shows example how to build application for `imx943evk` board with `cm7_core1` and `cm33_core1` core_id.
Change the `-b <board>` parameter based on board you want to build.
Change the `-Dcore_id=<core_id>` parameter based on board core you want to build.
For these parameters please see attribute `boards:` in `master/example.yml` and `remote/example.yml`.

```
west build examples/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr/master --toolchain armgcc --config debug -b imx943evk -Dcore_id=cm7_core1
west build examples/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr/remote --toolchain armgcc --config debug -b imx943evk -Dcore_id=cm33_core1
```

## Supported Boards
- [IMX943-EVK](../../_boards/imx943evk/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr/example_board_readme.md)
- [IMX943-ORANGEBOX](../../_boards/imx943_orangebox/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr/example_board_readme.md)
