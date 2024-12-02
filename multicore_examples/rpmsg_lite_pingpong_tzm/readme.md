# rpmsg_lite_pingpong_tzm

## Overview
The Multicore RPMsg-Lite pingpong TZM project is a simple demonstration program that uses the
MCUXpresso SDK software and the RPMsg-Lite library and shows how to implement the inter-core
communication between cores in the TrustZone-M (TZM) environment.
The TZM is configured in the BOARD_InitTrustZone() function. The secondary core is put into
the secure domain as well as the shared memory for RPMsg messages exchange. The primary core 
boots in the secure domain, it releases the secondary core from the reset and then the inter-core 
communication is established in the secure domain. Once the RPMsg is initialized
and the secure endpoint is created the message exchange starts, incrementing a virtual counter that is part
of the message payload. The message pingpong in the secure domain finishes when the counter reaches 
the value of 50. Then the secure domain is kept running and the non-secure portion of the application
is started. The non-secure RPMsg-Lite endpoint is created in the non-secure domain and the new message 
pingpong is triggered, exchanging data between the secondary core secure endpoint and the primary core 
non-secure endpoint. Veneer functions defined in the secure project are called from the non-secure
domain during this process. This message pingpong finishes when the counter reaches the value of 1050. 
Debug prints from the non-secure portion of the application are routed into the secure domain using
the dedicated veneer function.

## Shared memory usage
This multicore example uses the shared memory for data exchange. The shared memory region is
defined and the size can be adjusted in the linker file. The shared memory region start address
and the size have to be defined in linker file for each core equally. The shared memory start
address is then exported from the linker to the application.
The shared memory assignment to the secure domain is done in the BOARD_InitTrustZone() function.

## Supported Boards
- [FRDM-MCXN947](../../_boards/frdmmcxn947/multicore_examples/rpmsg_lite_pingpong_tzm/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/multicore_examples/rpmsg_lite_pingpong_tzm/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/multicore_examples/rpmsg_lite_pingpong_tzm/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/multicore_examples/rpmsg_lite_pingpong_tzm/example_board_readme.md)
