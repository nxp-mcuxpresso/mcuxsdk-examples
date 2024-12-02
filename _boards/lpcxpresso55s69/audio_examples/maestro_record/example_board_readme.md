Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55s69 board
- Personal Computer
- headphones with 3.5 mm stereo jack
- source of sound (line output to 3.5 mm stereo jack)

Board settings
============
Insert the card into the card slot

Prepare the Demo
===============
1. Connect headphones to Audio HP / Line-Out connector.
2. Connect source of sound to Audio Line-In connector.
3. Connect a micro USB cable between the PC host and the CMSIS DAP USB port (P6) on the board
4. Open a serial terminal with the following settings :
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
5. Download the program to the target board.
6. Either press the reset button on your board or launch the debugger in your IDE to begin
   running the demo.

### Notes
- MCUXpresso IDE project default debug console is semihost
- VIT is not enabled by default.
- In order to enable VIT in the MCUXpresso IDE:
   1. define VIT_PROC symbol
      (Project -> Properties -> C/C++ Build -> Settings -> MCU C Compiler -> Preprocessor)
   2. remove SD_ENABLED and STREAMER_ENABLE_FILE_SINK symbols
   3. change the DEMO_MIC_CHANNEL_NUM symbol value from 2 to 1 in the app_definitions.h file
