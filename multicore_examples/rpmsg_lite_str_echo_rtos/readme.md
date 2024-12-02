# rpmsg_lite_str_echo_rtos

## Overview
The Multicore RPMsg-Lite string echo project is a simple demonstration program that uses the
MCUXpresso SDK software and the RPMsg-Lite library and shows how to implement the inter-core
communicaton between cores of the multicore system.

It works with Linux RPMsg master peer to transfer string content back and forth. The name service
handshake is performed first to create the communication channels. Next, Linux OS waits for user
input to the RPMsg virtual tty. Anything which is received is sent to Cortex-M. Cortex-M displays 
what is received, and echoes back the same message as an acknowledgement. The tty reader on the 
Linux side can get the message, and start another transaction. The demo demonstrates RPMsg’s ability
to send arbitrary content back and forth. Note: The maximum message length supported by RPMsg is now 
496 bytes. String longer than 496 will be divided by virtual tty into several messages.
## Shared memory usage
This multicore example uses the shared memory for data exchange. The shared memory region is
defined and the size can be adjustable in the linker file. The shared memory region start address
and the size have to be defined in linker file for each core equally. The shared memory start
address is then exported from the linker to the application.
