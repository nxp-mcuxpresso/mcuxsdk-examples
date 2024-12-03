# sai_low_power_audio

## Overview
In this demo, A core decoded music data and put it to DDR buffer and informs M core with the related information. 
Then M core will take the ownership of consuming the buffer, it will copy buffer from DDR to TCM, manipulating SDMA to transfer the data to SAI and codec for playback. 
It gives DDR and A core opportunity to do power saving for rather long time frame. M core will also take ownership of codec initialization.
SRTM(Simplified Real Time Messaging) protocol is used to communicate between A core and M core. 
The protocol provides various commands for A core and M core to communicate with each other. 
If there is no audio palyback, M core will enter the STOP mode, and the whole SOC system would enter deep sleep mode(DSM) once A core enter low power status.

## Supported Boards
- [EVK9-MIMX8ULP](../../_boards/evk9mimx8ulp/demo_apps/sai_low_power_audio/example_board_readme.md)
- [EVK-MIMX8MM](../../_boards/evkmimx8mm/demo_apps/sai_low_power_audio/example_board_readme.md)
- [EVK-MIMX8MN](../../_boards/evkmimx8mn/demo_apps/sai_low_power_audio/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../_boards/evkmimx8mnddr3l/demo_apps/sai_low_power_audio/example_board_readme.md)
- [EVK-MIMX8MP](../../_boards/evkmimx8mp/demo_apps/sai_low_power_audio/example_board_readme.md)
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/demo_apps/sai_low_power_audio/example_board_readme.md)
