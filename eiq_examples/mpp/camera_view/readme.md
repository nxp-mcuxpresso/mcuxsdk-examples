# camera_view

## Overview

This basic example shows how to use the library to create a
simple camera preview pipeline.
This example also shows how to stop and re-start the pipeline after 3 seconds.

## Toolchains supported
Refer to build instructions:
- [Build](../../../../middleware/eiq/mpp/Build.md)

## Hardware requirements
Refer to board.readme for hardware setup requirements.
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/eiq_examples/mpp/board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/eiq_examples/mpp/board_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/eiq_examples/mpp/board_readme.md)

## Pipelines Description

The pipeline created by the application is:
```
+-------------+     +-----------+       +-------------+      +-------------+
|             |     |   JPEG    |       |             |      |             |
|    camera   | --> |  decoder  |  -->  |  2D convert | -->  |    Display  |
|             |     | (usb cam) |       |             |      |             |
+-------------+     +-----------+       +-------------+      +-------------+
```
Camera element is configured for a specific pixel format and resolution (board dependent)
Display element is configured for a specific pixel format and resolution (board dependent)
JPEG decoder element decodes the camera input stream from JPEG format to raw image data (YUV format)
  This element is present in the pipeline only when camera output is in JPEG format (eg. USB camera)
2D convert is configured to perform:
  - color space conversion from the camera pixel format to the display pixel format
  - rotation depending on the camera vs display orientation
  - scaling to match the display resolution


## Running the demo

EXPECTED OUTPUTS:
The expected outputs of the example are:
- The images captured by the camera should be displayed on the screen


## Supported Boards
- MIMXRT700-EVK
- MIMXRT1170-EVKB
- FRDM-MCXN947