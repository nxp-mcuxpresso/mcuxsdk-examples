# flexcan_loopback_edma_transfer

## Overview
The flexcan_loopback_edma example shows how to use the EDMA version transactional driver to receive
CAN Message from Rx FIFO:

To demonstrates this example, only one board is needed. The example will config one FlexCAN Message
Buffer to Tx Message Buffer and also setup Rx FIFO. After that, the example will send 4 CAN Messages
from Tx Message Buffer to Rx FIFO through internal loopback interconnect and read them out using
EDMA version FlexCAN transactional driver. The Sent and received message will be print out to terminal
at last.
