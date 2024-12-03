# Hardware Requirements

- Micro USB cable
- LPCXpresso860MAX board
- Personal Computer

# Board Settings

No special settings are required.

# Prepare the Demo

1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J4) on the board
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Press reset button to begin running the demo.

# Running the demo

Monitor the information on the debug console.

# Expected Result

- Board will print some log infomation first, and prompt users to input some character,
the received character will be echo back immediately. Once users press the [Enter] key,
board will print all the recieved character.
- Due to the value of the receive buffer is only 32, once the size of received characters
is larger than 32(contain [Enter] key), the overflow infomation will be printed. 
So, don't input too many characters in one time. 
