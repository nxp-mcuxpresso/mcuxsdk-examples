# rpmsg_lite_pingpong_rtos_remote_core

## Overview

The Multicore RPMsg-Lite pingpong RTOS project is a simple demonstration program
that uses the MCUXpresso SDK software, the RPMsg-Lite library and Linux rpmsg
driver and shows how to implement the inter-core communication between cores of
the multicore system. The Master core run on Linux OS(MPU), and the remote core
run on MCU(MCU). After soc reset and Linux boot up, the inter-core communication
is established. Once the RPMsg is initialized and endpoints are created the
message exchange starts, incrementing a virtual counter that is part of the
message payload. The message pingpong finishes when the counter reaches the
value of 100. Then the RPMsg-Lite is deinitialized and the procedure of the data
exchange is repeated again. Shared memory usage This multicore example uses the
shared memory for data exchange. The shared memory region is defined and the
size can be adjustable by RL_BUFFER_COUNT and RL_BUFFER_PAYLOAD_SIZE in
rpmsg_config.h. The shared memory region start address have to be defined in
board.h or app.h. The size and start address must match with the config of
DeviceTree of Linux.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [EVK-MIMX8MM](../../../_boards/evkmimx8mm/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [EVK-MIMX8MN](../../../_boards/evkmimx8mn/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../../_boards/evkmimx8mnddr3l/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [EVK-MIMX8MP](../../../_boards/evkmimx8mp/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [EVK-MIMX8MQ](../../../_boards/evkmimx8mq/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/example_board_readme.md)
