# lpadc_edma

## Overview

The lpadc_edma example shows how to use ADC to trigger a DMA transfer. 

In this example, user should indicate a channel to provide a voltage signal(can be controlled by user) as the LPADC's sample input
and active the low power timer to trigger the ADC conversion upon the set Time reached(can be changed by user). A programmable watermark
threshold supports configurable notification of data availability. When FCTRLn[FCOUNT] is greater than FCTRLn[FWMARK], the associated RDY
flag is asserted which would generate a DMA request and trigger the eDMA to move the ADC conversion result from ADC result FIFO to user
indicated memory. Then the major loop waits for the transfer to be done. The results would be printed on terminal when any key pressed.

## Supported Boards
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/lpadc/edma/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/lpadc/edma/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/lpadc/edma/example_board_readme.md)
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/lpadc/edma/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/lpadc/edma/example_board_readme.md)
- [FRDM-MCXA346](../../../_boards/frdmmcxa346/driver_examples/lpadc/edma/example_board_readme.md)
- [FRDM-MCXA174](../../../_boards/frdmmcxa174/driver_examples/lpadc/edma/example_board_readme.md)
- [FRDM-MCXA344](../../../_boards/frdmmcxa344/driver_examples/lpadc/edma/example_board_readme.md)
- [FRDM-MCXA266](../../../_boards/frdmmcxa266/driver_examples/lpadc/edma/example_board_readme.md)
- [FRDM-MCXA366](../../../_boards/frdmmcxa366/driver_examples/lpadc/edma/example_board_readme.md)
