# rpmsg_lite_str_echo_rtos_remote_core

## Overview

The Multicore RPMsg-Lite string echo project is a simple demonstration program
that uses the MCUXpresso SDK software and the RPMsg-Lite library and shows how
to implement the inter-core communication between cores of the multicore system.
It works with Linux RPMsg master peer to transfer string content back and forth.
The name service handshake is performed first to create the communication
channels. Next, Linux OS waits for user input to the RPMsg virtual tty. Anything
which is received is sent to Cortex-M. Cortex-M displays what is received, and
echoes back the same message as an acknowledgement. The tty reader on the Linux
side can get the message, and start another transaction. The demo demonstrates
RPMsg’s ability to send arbitrary content back and forth. Note - The maximum
message length supported by RPMsg is 496 bytes. String longer than 496 will be
divided by virtual tty into several messages. Shared memory usage This multicore
example uses the shared memory for data exchange. The shared memory region is
defined and the size can be adjustable in the linker file. The shared memory
region start address and the size have to be defined in linker file for each
core equally. The shared memory start address is then exported from the linker
to the application.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [EVK-MIMX8MM](../../../_boards/evkmimx8mm/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [EVK-MIMX8MN](../../../_boards/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../../_boards/evkmimx8mnddr3l/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [EVK-MIMX8MP](../../../_boards/evkmimx8mp/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [EVK-MIMX8MQ](../../../_boards/evkmimx8mq/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/multicore_examples/rpmsg_lite_str_echo_rtos/remote/example_board_readme.md)
