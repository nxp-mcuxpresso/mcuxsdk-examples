Hardware requirements
=====================

- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
==============

Make sure you have solder bridges SJ1 and SJ2 set to

- 2-1 for the main core (cm33)

Prepare the Demo
================

1. Connect a USB cable between the host PC and the MCU-Link USB port on the target board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

The log below shows the output of the rtc_timedate example in the terminal window:

```
==================================================
RTC Time/Date Mode Example
==================================================
Current Date/Time: 2025-5-31 21:40:0:5
Tamper 0 is enabled, press the button SW6 to trigger it.
Stored 256 bit key data written to RTC:
  Key[0]: 0x12345678
  Key[1]: 0x9ABCDEF0
  Key[2]: 0x11223344
  Key[3]: 0x55667788
  Key[4]: 0x99AABBCC
  Key[5]: 0xDDFF2211
  Key[6]: 0x22334455
  Key[7]: 0x66778899
Relevant 128 bit key data read from RTC:
  Key[0]: 0x5678
  Key[1]: 0xDEF0
  Key[2]: 0x3344
  Key[3]: 0x7788
  Key[4]: 0xBBCC
  Key[5]: 0x2211
  Key[6]: 0x4455
  Key[7]: 0x8899
Alarm 0 will ring at 21:40:04.
Alarm 1 will ring every second at 80.
Alarm 2 will ring every minute at 2nd second 60 hundredth.
Current Date/Time: 2025-5-31 21:40:0:80

*** ALARM 1 occured! ***

*** WATCHDOG fed! ***
Current Date/Time: 2025-5-31 21:40:1:80

*** ALARM 1 occured! ***

*** WATCHDOG fed! ***
Current Date/Time: 2025-5-31 21:40:2:60

*** ALARM 2 occured! ***
Current Date/Time: 2025-5-31 21:40:2:80

*** ALARM 1 occured! ***

*** WATCHDOG fed! ***
Current Date/Time: 2025-5-31 21:40:3:80

*** ALARM 1 occured! ***

*** WATCHDOG fed! ***
Current Date/Time: 2025-5-31 21:40:4:0

*** ALARM 0 occured! ***
Current Date/Time: 2025-5-31 21:40:4:80

*** ALARM 1 occured! ***

*** WATCHDOG fed! ***
Current Date/Time: 2025-5-31 21:40:5:80

```

If you press the tamper input button SW6 to trigger the tamper, the program will be reset, the time stamp of tamper occurs will be captured and log out. The security key will be erased and log out. 

```
==================================================
RTC Time/Date Mode Example
==================================================
CPU is reset from tamper.
Since the tamper occurs, the 256 bit security key data has been erased. Here is the 128 bit key data read from RTC:
  Key[0]: 0x0
  Key[1]: 0x0
  Key[2]: 0x0
  Key[3]: 0x0
  Key[4]: 0x0
  Key[5]: 0x0
  Key[6]: 0x0
  Key[7]: 0x0
*** TAMPER 0 timestamp: 2025-5-31 21:40:18:40 ***
```

