Hardware requirements
=====================
- Mini/micro USB cable
- IMX952-EVK board
- Personal Computer

Board settings
==============
In default, the example uses only ENETC0 (EP0).
To enable ENETC1 (EP1), install M.2 2.5G ETH card and change EXAMPLE_EP_NUM to 2U in app.h.
ENETC1 supports up to 1G speed.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the USB DBG port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Program flash.bin to boot source and start the board.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like below.
(No EP1 log if it's not enabled.)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NETC EP0 frame loopback example start.
Wait for PHY link up...
The 1 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
The 2 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
...
The 19 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
The 20 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00

NETC EP1 frame loopback example start.
Wait for PHY link up...
The 1 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
The 2 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
...
The 19 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
The 20 frame transmitted success!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
