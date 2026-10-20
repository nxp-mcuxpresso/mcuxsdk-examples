# dual_adc_average_timer

## Overview

The dual_adc_average_timer example shows how to use the average-with-timer feature of the DualADC driver.

This chip integrates one 16-bit Dual ADC with both A-side and B-side conversion channels. The two sides can operate independently or in cooperation mode, and multi-channel conversion is performed through a scan sequence built from command buffers (CMD). The average-with-timer feature automatically repeats the whole scan sequence at a configurable interval and accumulates the raw results into the RSLT register, so software can average them.

This example demonstrates the following features:

- Initializes the DualADC with the default configuration, enabling Cooperation Mode and analog pre-enable to minimize trigger latency.
- Configures four command buffers to form a scan sequence, each occupying one channel in 12-bit standard resolution with Hardware Average Single (no per-command hardware averaging):
  - CMD1: A-side single-ended conversion.
  - CMD2: B-side single-ended conversion.
  - CMD3: A-side single-ended conversion.
  - CMD4: B-side single-ended conversion.
  - CMD5: marked as the sequence end command so the scan terminates after CMD4.
- Keeps each command's sample time short so the whole scan sequence completes inside the average-with-timer interval.
- Enables the average-with-timer for ADCA (in cooperation mode ADCB follows ADCA), repeating the sequence 8 times at the configured interval.
- Enables the End-of-Scan interrupt for ADCA.
- Runs a full calibration (offset, optional high-speed, and gain calibration).
- On each key press, issues a software trigger (Trigger0) to start the repeated conversion. In the End-of-Scan interrupt, the accumulated sum of each command is read and the flags cleared. Because the 12-bit code sits at bit[14:3], the summed value is right-shifted by 3 first and then divided by the repeat count (8) to recover the averaged 12-bit value, which is printed over the serial console.


## Supported Boards
- [FRDM-MCXC353](../../../_boards/frdmmcxc353/driver_examples/dual_adc/average_timer/example_board_readme.md)
