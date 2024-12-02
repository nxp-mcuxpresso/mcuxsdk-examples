# sai_low_power_audio_rtos_imx95

## Overview
In this demo, A core decoded music data and put it to DDR buffer and informs M core with the related information.
Then M core will take the ownership of consuming the buffer, it will copy buffer from DDR to TCM, manipulating EDMA4 to transfer the data to SAI and codec for playback.
It gives DDR and A core opportunity to do power saving for rather long time frame. M core will also take ownership of codec initialization.
SRTM(Simplified Real Time Messaging) protocol is used to communicate between A core and M core.
The protocol provides various commands for A core and M core to communicate with each other.
If there is no audio palyback, M core will enter the STOP mode, and the whole SOC system would enter deep sleep mode(DSM) once A core enter low power status.

## Supported Boards
- [IMX95LP4XEVK-15](../../_boards/imx95lp4xevk15/demo_apps/sai_low_power_audio/example_board_readme.md)
- [IMX95LPD5EVK-19](../../_boards/imx95lpd5evk19/demo_apps/sai_low_power_audio/example_board_readme.md)
- [imx95verdinevk](../../_boards/imx95verdinevk/demo_apps/sai_low_power_audio/example_board_readme.md)
