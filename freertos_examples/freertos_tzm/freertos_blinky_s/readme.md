# freertos_blinky_s

## Overview
This document explains the freertos_tzm example. This demo application utilizes TrustZone,
so it demonstrates following techniques for TrustZone applications development:
1. Application separation between secure and non-secure part
2. TrustZone environment configuration
3. Exporting secure function to non-secure world
4. Calling non-secure function from secure world
5. Configuring IAR, MDK, GCC and MCUX environments for TrustZone based projects

## Running the demo
The Trust Zone-M (TZM) demo creates two unprivileged tasks. One of which calls a secure
side function and passes a pointer to a callback function. The secure side
function does three things:
1. It calls the provided callback function. The callback function increments
a counter.
2. It increments a counter and returns the incremented value.
After the secure function call finishes, it verifies that both the counters
are incremented.
3. It calls a secure side function to toggle the green LED
The second task just calls a secure side function to toggle the blue LED

## Supported Boards
