Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 board
- Personal Computer

Board settings
==============
USART hardware flow control on one board.
The USART sends data to itself(loopback), and hardware flow control function is
enabled on this example. So please make sure the correct connection for example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pin Name    Board Location    connect to     Pin Name    Board Location
USART_TX    J36   pin 6       ----------     USART_RX    J36   pin 8
USART_CTS   J28   pin 9       ----------     USART_RTS   J28   pin 10
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Remove jumper JP15.


Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This is USART hardware flow control example on one board.
This example will send data to itself and will use hardware flow control to avoid the overflow.
Please make sure you make the correct line connection. Basically, the connection is:
      USART_TX    --     USART_RX
      USART_RTS   --     USART_CTS
Data matched! Transfer successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
