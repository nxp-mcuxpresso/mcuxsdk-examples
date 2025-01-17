# camera_view

## Overview

This basic example shows how to use the library to create a
simple camera preview pipeline.
This example also shows how to stop and re-start the pipeline after 3 seconds.

## Toolchains supported
- MCUXpresso, version 11.10.0
- GCC Arm Embedded, version 13.2.Rel1

## Hardware requirements
Refer to board.readme for hardware setup requirements.
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/eiq_examples/mpp/board_readme.md)

## Pipelines Description

The pipeline created by the application is:
```
+-------------+      +-------------+      +-------------+
|             |      |             |      |             |
|    camera   | -->  |  2D convert | -->  |    Display  |
|             |      |             |      |             |
+-------------+      +-------------+      +-------------+
```
Camera element is configured for a specific pixel format and resolution (board dependent)
Display element is configured for a specific pixel format and resolution (board dependent)
2D convert is configured to perform:
  - color space conversion from the camera pixel format to the display pixel format
  - rotation depending on the camera vs display orientation


## Running the demo

EXPECTED OUTPUTS:
The expected outputs of the example are:
- The images captured by the camera should be displayed on the screen

