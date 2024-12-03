# tsi_v5_selfmode

## Overview
The tsi_v5_selfmode demo shows how to use TSI_V5 driver in self-cap mode:

In this example , available electrodes on board is used to show how to realize touch key.
1. Firstly, get the non-touch calibration results as baseline electrode counter;
2. Then, start the periodical Software-Trigger scan using polling method to detect finger touch;
3. Wait for the electrodes touched and deal with the event.

## Running the demo
When running successfully, LED will be turn on when the electrode on board touched and will be
turn off when released. The log output in terminal shall be similar as below:
~~~~~~~~~~~~~~~~~~~~~
TSI self mode demo.
TSI key touched.
TSI key released.
TSI key touched.
TSI key released.
~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-KE15Z](../../../../_boards/frdmke15z/demo_apps/tsi_v5/tsi_v5_selfmode/example_board_readme.md)
- [FRDM-KE16Z](../../../../_boards/frdmke16z/demo_apps/tsi_v5/tsi_v5_selfmode/example_board_readme.md)
- [FRDM-KE17Z](../../../../_boards/frdmke17z/demo_apps/tsi_v5/tsi_v5_selfmode/example_board_readme.md)
- [FRDM-KE17Z512](../../../../_boards/frdmke17z512/demo_apps/tsi_v5/tsi_v5_selfmode/example_board_readme.md)
