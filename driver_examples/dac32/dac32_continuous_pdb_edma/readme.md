# dac32_continuous_pdb_edma

## Overview

The demo shows how to use the PDB to generate a DAC32 trigger and use the DMA to transfer data into DAC32 buffer.

In this example, DAC32 is first set to normal buffer mode. PDB is as DAC32 hardware trigger source and DMA would work 
when DAC32 read pointer is zero. When run the example, the DAC32 is triggered by PDB and the read pointer increases by one,
every time the trigger occurs. When the read pointer reaches the upper limit, it goes to zero directly in the next trigger event.
While read pointer goes to zero, DMA request will be triggered and transfer data into DAC32 buffer. The user should probe
the DAC output with a oscilloscope to see the Half-sine signal.