Hardware requirements
=====================
- Micro USB cable
- MIMX8ULP-EVK/EVK9 Board
- J-Link Debug Probe
- 5V power supply
- Personal Computer

Board settings
============
No special is needed.



Prepare the Demo
===============
1.  Connect 5V power supply and J-Link Debug Probe to the board, switch SW10 to power on the board
2.  Connect a USB cable between the host PC and the J17 USB port on the target board.
3.  Open two serial terminals for A Core and M Core with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    (e.g. /dev/ttyUSB0~3, /dev/ttyUSB2 for A Core, /dev/ttyUSB3 for M Core)
4.  Flash flash.bin to emmc/flexspi0 nor flash using u-boot/UUU/JLink.(Don't use IAR/GDB to download m33 binary to SSRAM to test it)
5.  Repower on board to boot system
6.  Login to linux
7.  Change log level of linux with below command:
    # echo '7 4 1 7' > /proc/sys/kernel/printk
8.  After login, make sure imx_rpmsg_pingpong kernel module is inserted (lsmod) or insert it (modprobe imx_rpmsg_pingpong).
Running the demo

===============
After the boot process succeeds, the ARM Cortex-M33 terminal displays the following information:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
RPMSG Ping-Pong FreeRTOS RTOS API Demo...
RPMSG Share Base Addr is 0xb8000000
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
During boot the Kernel,the ARM Cortex-M33 terminal displays the following information:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Link is up!
Nameservice announce sent.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
After the Linux RPMsg pingpong module was installed, the ARM Cortex-M33 terminal displays the following information:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
The Cortex-A terminal displays the following information:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
get 1 (src: 0x1e)
get 3 (src: 0x1e)
......
get 99 (src: 0x1e)
get 101 (src: 0x1e)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================
