# rtc_jdp

## Overview

The RTC example is a simple demonstration program of the SDK RTC driver. The RTC interrupt is enabled in period 1000ms, the API(Autonomous Periodic Interrupt) interrupt is enabled in 500ms. A callback function sample is provided which will log it is a RTC or API interrupt and toggle the corresponding LED if existed in the board.

## Running the demo

The log below shows the output of the terminal window:

```
RTC example starts.
It prints callback function running for the RTC or API interrupt.
Run in API call back.
Run in RTC call back.
Run in API call back.
Run in API call back.
Run in RTC call back.
Run in API call back.
Run in API call back.
Run in RTC call back.
Run in API call back.
Run in API call back.
Run in RTC call back.
......
```

## Supported Boards
- FRDM-MCXE31B