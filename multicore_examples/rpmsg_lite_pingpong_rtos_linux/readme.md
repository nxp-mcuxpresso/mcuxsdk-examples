# rpmsg_lite_pingpong_rtos

## Overview
The Multicore RPMsg-Lite pingpong RTOS project is a simple demonstration program that uses the
MCUXpresso SDK software, the RPMsg-Lite library and Linux rpmsg driver and shows how to
implement the inter-core communicaton between cores of the multicore system. The Master core
run on Linux OS(MPU), and the remote core run on FreeRTOS(MCU), After soc reset and Linux boot up,
the inter-core communication is established. Once the RPMsg is initialized and endpoints are created
the message exchange starts, incrementing a virtual counter that is part of the message payload.
The message pingpong finishes when the counter reaches the value of 100.  Then the RPMsg-Lite is
deinitialized and the procedure of the data exchange is repeated again.

## Shared memory usage
This multicore example uses the shared memory for data exchange. The shared memory region is
defined and the size can be adjustable by RL_BUFFER_COUNT and RL_BUFFER_PAYLOAD_SIZE in rpmsg_config.h.
The shared memory region start address have to be defined in board.h or app.h. The size and start 
address must match with the config of DeviceTree of Linux.
