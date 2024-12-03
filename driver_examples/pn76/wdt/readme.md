# wdt

## Overview
The WDT example is to demonstrate usage of the MSDK WDT driver.
In this example, the Watchdog Timer can be used to limit the duration within which a software task must complete.
As soon as the timer started to count down, feed watchdog before the timer reaches zero.Otherwise the system reset will be generated.
The Watchdog timer includes a non-maskable interrupt,and it will be generated when the counter reaches the WDOG_INT_THRESHOLD.