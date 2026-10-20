# dual_adc_interrupt

## Overview

The dual_adc_interrupt example shows how to use interrupt with DualADC driver.

This chip integrates one 16-bit Dual ADC with both A-side and B-side conversion channels. The two sides can operate independently or in cooperation mode, and multi-channel conversion is performed through a scan sequence built from command buffers (CMD).

This example demonstrates the following features:

- Initializes the DualADC with the default configuration, enabling Cooperation Mode and analog pre-enable to minimize trigger latency.
- Configures multiple command buffers to form a scan sequence:
  - CMD1: A-side single-ended conversion, 12-bit standard resolution.
  - CMD2: B-side single-ended conversion, 12-bit standard resolution.
  - CMD3: A/B dual single-ended conversion with alternate B channel select enabled.
  - CMD4: marked as the sequence end command so the scan terminates after CMD3.
- Enables the End-of-Scan interrupt for ADCA.
- Runs a full calibration (offset, optional high-speed, and gain calibration).
- On each key press, issues a software trigger (Trigger0) to start a conversion sequence. In the End-of-Scan interrupt, the conversion results of each command are read and the flags cleared, then the 12-bit results (located at bit[14:3]) are right-shifted by 3 and printed over the serial console.


## Supported Boards
- [FRDM-MCXC353](../../../_boards/frdmmcxc353/driver_examples/dual_adc/interrupt/example_board_readme.md)

