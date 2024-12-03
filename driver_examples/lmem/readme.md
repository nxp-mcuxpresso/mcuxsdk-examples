# lmemc

## Overview

The local memory controller cache example shows how to use local memory cache driver.

In this example, the sdram memory will be used to show the example.
The sdram memory is both accessible for cpu and EDMA. For the memory data alignment, cache driver should be 
used rightly. The code cache driver and system cache driver can be used when the relevant cache mode is enabled.
In code cache enabled case, the code cache enabled memory mapped in sdram is write though/non-cacheable mode.
so the sdram memory invalidate should be done before cpu read the memory. In system cache enabled case, when the
sdram memory is write-back mode, the memory invalidate should be done before cpu read the memory and the
memory push should be done before EDMA access the memory.
