# dac_continuous_pit_edma

## Overview

The demo shows how to use the PIT to generate a DAC trigger and use the EDMA to transfer data into DAC buffer.

In this example, DAC is first set to normal buffer mode. PIT is as DAC hardware trigger source and EDMA would work 
when DAC read pointer is zero. When run the example, the DAC is triggered by PIT and the read pointer increases by one,
every time the trigger occurs. When the read pointer reaches the upper limit, it goes to zero directly in the next trigger event.
While read pointer goes to zero, EDMA request will be triggered and transfer data into DAC buffer. The user should probe
the DAC output with a oscilloscope to see the Half-sine signal.