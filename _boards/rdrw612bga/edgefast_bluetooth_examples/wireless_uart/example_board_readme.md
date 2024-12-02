Hardware requirements
=====================
- Micro USB cable
- rdrw610 board
- Personal Computer

Board settings
============
U38 DIP 1,2,3,4 all off
HD12 1-2, 3-4 connected
Remove R520, solder R518

Prepare BLE controller
============================
Download BLE controller FW according to the document component\conn_fwloader\readme.txt.

Note:
To ensure that the LITTLEFS flash region has been cleaned,
all flash sectors need to be erased before downloading example code.
After downloaded binary to the target board, 
please reset the board by pressing SW5 or power off and on the board to run the application.
