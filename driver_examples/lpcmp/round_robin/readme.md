# lpcmp_round_robin

## Overview
The LPCMP round robin example is a simple demonstration program that uses the LPCMP driver to
help users get started quickly. In this example, users need to specify which port and channel
to fix, and which channels to select as checker channels. Before doing the comparison, it is
necessary to preset the comparison result of each checker channel with the fixed channel. 
If any results of the comparator are different from the preset value, an interrupt will be 
triggered, and the information of the triggered channel will be printed on the terminal after
changing the input voltage of checker channels.

## Supported Boards
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/lpcmp/round_robin/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/lpcmp/round_robin/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/lpcmp/round_robin/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/lpcmp/round_robin/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/lpcmp/round_robin/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/lpcmp/round_robin/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/lpcmp/round_robin/example_board_readme.md)
