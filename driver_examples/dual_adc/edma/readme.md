# dual_adc_edma

## Overview

The dual_adc_edma example shows how to use eDMA to transfer DualADC conversion results.

This chip integrates one 16-bit Dual ADC with both A-side and B-side conversion channels. The two sides can operate independently or in cooperation mode, and multi-channel conversion is performed through a scan sequence built from command buffers (CMD). When the DualADC DMA request is enabled, each end-of-scan event raises an eDMA request that moves the conversion results out of the RSLT registers into a memory buffer without CPU intervention.

This example demonstrates the following features:

- Initializes the DualADC with the default configuration, enabling Cooperation Mode and analog pre-enable to minimize trigger latency, and enables the ADCA DMA request (end-of-scan source; in cooperation mode ADCB follows ADCA).
- Configures four command buffers to form a scan sequence, each occupying one channel in 12-bit standard resolution with Hardware Average Single (no hardware averaging):
  - CMD1: A-side single-ended conversion.
  - CMD2: B-side single-ended conversion.
  - CMD3: A-side single-ended conversion.
  - CMD4: B-side single-ended conversion.
  - CMD5: marked as the sequence end command so the scan terminates after CMD4.
- Runs a full calibration (offset, optional high-speed, and gain calibration).
- Configures one eDMA channel to copy the four 32-bit RSLT registers into a memory buffer on each end-of-scan DMA request, and enables the major-loop-complete interrupt.
- On each key press, issues a software trigger (Trigger0) to start the conversion sequence. After the eDMA signals the transfer is complete, the 12-bit results (located at bit[14:3]) are right-shifted by 3 and printed over the serial console.


## Supported Boards
- [FRDM-MCXC353](../../../_boards/frdmmcxc353/driver_examples/dual_adc/edma/example_board_readme.md)
