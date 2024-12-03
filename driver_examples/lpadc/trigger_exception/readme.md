# lpadc_trigger_exception

## Overview
The lpadc_trigger_exception example demonstrates the usage of trigger exception feature.

This example involves two conversion sequences. The first works in continuous mode, triggered by software to sample
external input. The second works in one-shot mode, triggered by an onboard button to sample VDD/4. The second sequence
has a higher priority than the first, meaning that if it is triggered, the first sequence will be aborted and the
second will begin executing at a specified breakpoint.

## Supported Boards
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/lpadc/trigger_exception/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/lpadc/trigger_exception/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/lpadc/trigger_exception/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/lpadc/trigger_exception/example_board_readme.md)
