# lpsci_dma_rb_transfer

## Overview
The lpsci_dma ring buffer example project is to demonstrate usage of the KSDK lpsci driver.
In the example, LPSCI will receive data from PC side in DMA mode, and data will be stored
in the ring buffer, while IDLE line was detected, routine can read out the ring buffer and send 
them back to the PC side. 

Note that, the ring buffer is implemented by DMA controler, The base address of ring buffer should
be aligned to a 0-modulo-(circular buffer size) boundary.