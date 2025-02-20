# aon_lpadc_edma

## Overview

The aon_lpadc_edma example shows how to use ADC to trigger a DMA transfer. 

The aon_lpadc_edma example shows how to use ADC to trigger a DMA transfer.
In this example, user should indicate a channel to provide a voltage signal(can
be controlled by user) as the aon_lpadc's sample input. A programmable watermark
threshold supports configurable notification of data availability. When FCTRLn[FCOUNT]
is greater than FCTRLn[FWMARK], the associated RDY flag is asserted which would generate
a eDMA request and trigger the eDMA to move the ADC conversion result from ADC result
FIFO to user indicated memory. Then the major loop waits for the transfer to be done.
The results would be printed on terminal when any key pressed.

## Supported Boards
- [FRDM-MCXL255](../../../_boards/frdmmcxl255/driver_examples/aon_lpadc/edma/example_board_readme.md)
