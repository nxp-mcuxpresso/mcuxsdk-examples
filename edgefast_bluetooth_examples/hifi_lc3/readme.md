# lc3

## Overview

The hifi_lc3 application is used for LC3 encoding/decoding by LE Audio applications.

The application is relayed via RPMsg-Lite IPC to the hifi DSP where it is processed and a response is returned.

This application must work with LE Audio applications:
    call_gateway/terminal
    broadcast_media_sender/receiver
    unicast_media_sender/receiver

The release configurations of the demo need combine LE Audio applications into one ARM
image. With this, the ARM core will load and start the DSP application on
startup. Pre-compiled hifi DSP binary images are provided under hifi4/binary/ directory.
If you make changes to the hifi DSP application in release configuration, rebuild
ARM application after building then hifi DSP application.

This application can be built by the following tools:
Xtensa Xplorer or Xtensa C Compiler. Required tool versions can be found
in MCUXpresso SDK Release Notes for the board. 

The ARM application will power and clock the DSP, so it must be loaded prior to
loading the hifi DSP application.

## Supported Boards
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/edgefast_bluetooth_examples/hifi_lc3/example_board_readme.md)
