Hardware requirements
=====================
- Micro USB cable
- two FRDM-IMX937 boards
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
==============
Connection as below:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER(LPSPI7)                connect to        SLAVE(LPSPI7)
Pin Name    Board Location                      Pin Name    Board Location
PCS0        J18 pin 7                           PCS0        J18 pin 7
SCK         J18 pin 26                          SCK         J18 pin 26
SOUT        J18 pin 31                          SIN         J18 pin 9
SIN         J18 pin 9                           SOUT        J18 pin 31
GND         J18 pin 6                           GND         J18 pin 6
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IMPORTANT: SOUT/SIN (MOSI/MISO) lines MUST be crossed as shown above.

Note: J18 is the 40-PIN GPIO HDR expansion connector.
      - J18-Pin7:  LPSPI7_PCS0 (GPIO_IO04) - Chip Select
      - J18-Pin26: LPSPI7_SCK  (GPIO_IO07) - Clock
      - J18-Pin31: LPSPI7_SOUT (GPIO_IO06) - MOSI (connects to slave SIN)
      - J18-Pin9:  LPSPI7_SIN  (GPIO_IO05) - MISO (connects to slave SOUT)
      - J18-Pin6:  Common Ground (any GND pin on J18 can be used)
      - Each board must be powered independently
      - No IO expander configuration required for this example

Prepare the Demo
================
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW4 to power on the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either cold boot your board or launch the debugger in your IDE to begin running the example.


