Hardware requirements
=====================
- Type-C cable
- frdmrw612 board
- Personal Computer

Board settings
=====================
SJ21 1-2 disconnected, 2-3 connected.
SJ22 1-2 disconnected, 2-3 connected.

Prepare BLE controller
=====================
Download BLE controller FW according to the document component\conn_fwloader\readme.txt.

Note:
To ensure that the LITTLEFS flash region has been cleaned,
all flash sectors need to be erased before downloading example code.
After downloaded binary to the target board, 
please reset the board by pressing SW1 or power off and on the board to run the application.
