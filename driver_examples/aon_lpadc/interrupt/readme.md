# aon_lpadc_interrupt

## Overview

The aon_lpadc_interrupt example shows how to use interrupt with aon_lpadc driver.

In this example, user should indicate a channel to provide a voltage signal (can
be controlled by user) as the aon_lpadc's sample input. When running the project,
typing any key into debug console would trigger the conversion. ADC watermark
interrupt would be asserted once the number of datawords stored in the ADC Result
FIFO is greater than watermark value. In ADC ISR, the watermark flag would be
cleared by reading the conversion result value. Also, result information would
be printed when the execution return to the main function.

## Supported Boards
- [FRDM-MCXL255](../../../_boards/frdmmcxl255/driver_examples/aon_lpadc/interrupt/example_board_readme.md)
