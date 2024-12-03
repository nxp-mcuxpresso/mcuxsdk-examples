# low_power_wakeword

## Overview
In this demo, M core receives audio from microphones and provide them to Linux on Cortex A in DDR and notifies it through RPMSG protocol.
If Linux is suspended, the audio is processed locally with teh selected algorithm
(VAD or wakeword or any future extension).
When voice algorithms triggers (VAD detects voice activity, or wakeword engine detects the wakeword), then A core
is resumed and audio streaming from M core to A core restarts.
When Cortex A is running, music playback from A core can also be sent to M core through DDR and RPMSG protocol but no playback is supported when M core only is running.

## Supported Boards
- [EVK-MIMX8MM](../../_boards/evkmimx8mm/demo_apps/low_power_wakeword/example_board_readme.md)
- [EVK-MIMX8MP](../../_boards/evkmimx8mp/demo_apps/low_power_wakeword/example_board_readme.md)
- [IMX95LP4XEVK-15](../../_boards/imx95lp4xevk15/demo_apps/low_power_wakeword/example_board_readme.md)
- [IMX95LPD5EVK-19](../../_boards/imx95lpd5evk19/demo_apps/low_power_wakeword/example_board_readme.md)
- [MCIMX93-EVK](../../_boards/mcimx93evk/demo_apps/low_power_wakeword/example_board_readme.md)
- [MCIMX93-QSB](../../_boards/mcimx93qsb/demo_apps/low_power_wakeword/example_board_readme.md)
