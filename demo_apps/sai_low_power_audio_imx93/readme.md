# sai_low_power_audio_rtos_imx93

## Overview
In this demo, A core decoded music data and put it to DDR buffer and informs M core with the related information.
Then M core will take the ownership of consuming the buffer, it will copy buffer from DDR to TCM, manipulating EDMA4 to transfer the data to SAI and codec for playback.
It gives DDR and A core opportunity to do power saving for rather long time frame. M core will also take ownership of codec initialization.
SRTM(Simplified Real Time Messaging) protocol is used to communicate between A core and M core.
The protocol provides various commands for A core and M core to communicate with each other.
If there is no audio palyback, M core will enter the STOP mode, and the whole SOC system would enter deep sleep mode(DSM) once A core enter low power status.

## Supported Boards
- [MCIMX93-EVK](../../_boards/mcimx93evk/demo_apps/sai_low_power_audio/example_board_readme.md)
- [MCIMX93-QSB](../../_boards/mcimx93qsb/demo_apps/sai_low_power_audio/example_board_readme.md)
