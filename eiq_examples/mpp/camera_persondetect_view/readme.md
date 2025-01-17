# camera_persondetect_view

## Overview

This example shows how to use the library to create a use-case for
person detection using a camera or a static image as source.

The machine learning framework used for this example is TensorFlow Lite Micro.
The person detection model used is quantized persondetect model that detects multiple persons in an input image.

## Toolchains supported
- MCUXpresso, version 11.10.0
- GCC Arm Embedded, version 13.2.Rel1

## Hardware requirements
Refer to board.readme for hardware setup requirements.
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/eiq_examples/mpp/board_readme.md)

## Use-cases Description

HOW TO USE THE APPLICATION:

### High-level description
```
                                                                   +--------------------------------------------------------+
                                                                   |                                                        |
                                                                   |                                                        |
                                                                  \ /                                                       |
                  +-------------+      +-------------+      +-------------+      +-------------+      +-------------+       |
                  |             |      |             |      |             |      |             |      |             |       |
Pipeline 0        |    camera   | -->  |  2D convert | -->  |   labeled   | -->  |  2D convert | -->  |    Display  |       |
                  |             |  |   |(color+flip) |      |  rectangle  |      | (rotation)  |      |             |       |
                  +-------------+  |   +-------------+      +-------------+      +-------------+      +-------------+       |
                                   |                                                                                        |
                                   |     +-------------+      +--------------+      +-------------+                         |
                                   |     |             |      |              |      |             |                         |
Pipeline 1                         +---> |  2D convert | -->  | ML Inference | -->  |  NULL sink  |                         |
                                         |             |      |              |      |             |                         |
                                         +-------------+      +--------------+      +-------------+                         |
                                                                       |                                                    |
                                                                       |                                                    |
        +-----------------+                                            |                                                    |
	|  Main app:      |                                            |                                                    |
	| ML output       |   <----- ML Inference output callback -----+                                                    |
        | post processing |                                                                                                 |
	|                 |   ------   labeled rectangle update   ----------------------------------------------------------+
	+-----------------+
```
### Detailed description

Application creates two pipelines:

- One pipeline that runs the camera or static image preview.
- Another pipeline that runs the ML inference on the image coming from the camera or the static image.
- Pipeline 1 is split from pipeline 0
- Pipeline 0 executes the processing of each element sequentially and CANNOT be preempted by another pipeline.
- Pipeline 1 executes the processing of each element sequentially but CAN be preempted.

### Pipelines elements description

* Camera element is configured for a specific pixel format and resolution (board dependent)
* Static image element is configured for a specific pixel format and resolution (board dependent)
* Display element is configured for a specific pixel format and resolution (board dependent)
* 2D convert element on pipeline 0 is configured to perform:
  - color space conversion from the camera pixel format to the display pixel format
  - rotation depending on the display orientation compared to landscape mode (NB: Rotation should be performed 
  after the labeled-rectangle to get labels in the right orientation).

* 2D convert element on pipeline 1 is configured to perform:
  - color space conversion from the camera pixel format to RGB888
  - cropping to maintain image aspect ratio
  - scaling to 160x128 as mandated by the persondetection model

* The labeled rectangle element draws a crop window from which the camera image is sent to
  the ML inference element. 
* The ML inference element runs an inference on the image pre-processed by the 2D convert element.
* The NULL sink element closes pipeline 1 (in MPP concept, only sink elements can close a pipeline).

* At every inference, the ML inference element invokes a callback containing the inference outputs.
These outputs are post-processed by the callback client component (in this case, the main task of the application)

## Running the demo

EXPECTED OUTPUTS:
The expected outputs of the example are:
- For each detected person, a rectangle should be displayed on the screen.
- Logs below should be displayed on the debug console.

Logs for camera_persondetect_view example using TensorFlow Lite Micro model should look like this:
```
[MPP_VERSION_2.1.e1dc37e]
Inference Engine: TensorFlow-Lite Micro 

Element stats --------------------------
Persondetect : exec_time 405 ms
Number of detections : 1
box 0 --> score 37 %
 Left=64, Top=49, Right=115, Bottom=128
-------------------------------------------

Element stats --------------------------
Persondetect : exec_time 405 ms
No person detected

Element stats --------------------------
Persondetect : exec_time 405 ms
Number of detections : 1
box 0 --> score 43 %
 Left=63, Top=55, Right=113, Bottom=128
-------------------------------------------

Element stats --------------------------
Persondetect : exec_time 403 ms
Number of detections : 1
box 0 --> score 37 %
 Left=67, Top=55, Right=112, Bottom=128
-------------------------------------------

Element stats --------------------------
Persondetect : exec_time 405 ms
No person detected

Element stats --------------------------
Persondetect : exec_time 405 ms
No person detected
```
## Important notes

TensorFLow Lite Micro is an optional engine for the ML Inference component of MPP.
This project embeds NXP's custom TensorFlow Lite Micro code by default.
TensorFLow Lite allows short-listing the "Operations" used by a specific model in order to reduce the binary image footprint.
This is done by implementing the function:

tflite::MicroOpResolver &MODEL_GetOpsResolver()

This example implements its own function MODEL_GetOpsResolver dedicated to persondetect.
User may provide its own implementation of MODEL_GetOpsResolver when using a different model.
