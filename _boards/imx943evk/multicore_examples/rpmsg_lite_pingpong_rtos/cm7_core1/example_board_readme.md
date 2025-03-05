Hardware requirements
=====================
- Micro USB cable
- IMX943-EVK Board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============
No special is needed.

Prepare the Demo
===============
1. Connect a USB cable between the host PC and the J1401 USB port on the target board.
   Open two serial terminals for A Core and M Core with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
2. Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW301 to power on the board
3. Download the program to the target board.
4. Either re-power up your board or launch the debugger in your IDE to begin running the example.

Enable an option(Cannot enable them at the same time) in prj.conf of the project to select which core as master and which core as remote as the follow table:
|         Option                       |   Master |   Remote   |
|--------------------------------------|----------|------------|
|  CONFIG_RPMSG_M71_M33S=y(default)    |    M71   |    M33S    |
|--------------------------------------|----------|------------|
|  CONFIG_RPMSG_M71_M70=y              |    M71   |    M70     |
|--------------------------------------|----------|------------|
|  CONFIG_RPMSG_M70_M71=y              |    M70   |    M71     |
|--------------------------------------|----------|------------|
|  CONFIG_RPMSG_M70_M33S=y             |    M70   |    M33S    |
|--------------------------------------|----------|------------|
|  CONFIG_RPMSG_M33S_M70=y             |    M33S  |    M70     |
|--------------------------------------|----------|------------|
|  CONFIG_RPMSG_M33S_M71=y             |    M33S  |    M71     |
|--------------------------------------|----------|------------|

Running the demo
===============
After the boot process succeeds, the MCU-Remote terminal displays the following information:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
RPMSG Ping-Pong FreeRTOS RTOS API Demo...
RPMSG Share Base Addr is 0x20330000
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
During boot system terminal displays the following information:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Link is up!
Nameservice announce sent.
Waiting for ping...
Sending pong...
Waiting for ping...
Sending pong...
Waiting for ping...
Sending pong...
......
Waiting for ping...
Sending pong...
Ping pong done, deinitializing...
Looping forever...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The MCU-Master terminal displays the following information:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Demo starts
Primary core received a msg
Message: Size=4, DATA = 1
Primary core received a msg
Message: Size=4, DATA = 3
Primary core received a msg
Message: Size=4, DATA = 5

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================
