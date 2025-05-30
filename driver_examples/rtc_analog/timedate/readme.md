# rtc_timedate

## Overview

The RTC project is a demonstration program of the SDK RTC driver working on time date mode.  The program does the followings:

1. Set the stored time and date to make RTC work in timedate mode.
2. Set watchdog
3. Set tamper. Write stored security key into RTC. When a tamper occurs triggered by press tamper input button, the system will be reset, the time stamp of tamper will log out and the prestore security key will be erased and log out.
4. 3 alarms are enabled, alarm0 is single shot, alarm1 and alarm2 are repeated. Alarm1 occurs every second at 80th hundredth. Alarm2 occurs every minute at 2nd second 60th hundredth. In the alarm1 interrupt handler, the watchdog is fed.

## Supported Boards

- [FRDM-MCXL255](../../_boards/frdmmcxl255/driver_examples/rtc/timedate/example_board_readme.md)
