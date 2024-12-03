# xrdc_security_modes

## Overview
The XRDC security modes demo application demonstrates how the resources (memory
regions and peripherals) can be protected by the XRDC peripheral in different modes
(privileged secure, privileged nonsecure, user secure, user nonsecure).

In this example is configured memory region with this access policy:
    privileged secure mode: read/write
    privileged nonsecure mode: read/write
    user secure mode: none
    user nonsecure mode: none

The cpu switches between different modes (privileged secure, privileged nonsecure,
user secure, user nonsecure) and try writes to memory region. When the cpu is in user
modes, the writing to the memory region causes hard fault.
