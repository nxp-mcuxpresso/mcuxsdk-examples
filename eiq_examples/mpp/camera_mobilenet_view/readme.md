# camera_mobilenet_view

## Overview
This example shows how to use the library to create a use-case for
image classification using camera as source.

The machine learning framework used is TensorFlow Lite Micro
The image classification model used is quantized Mobilenet
convolutional neural network model [1] that classifies the input image into
one of 1000 output classes.

[1] https://www.tensorflow.org/lite/models

## Toolchains supported
Refer to build instructions:
- [Build](../../../../middleware/eiq/mpp/Build.md)

## Hardware requirements
Refer to board.readme for hardware setup requirements.
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/eiq_examples/mpp/board_readme.md)
- [FRDM-MCXN947T](../../../_boards/frdmmcxn947t/eiq_examples/mpp/board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/eiq_examples/mpp/board_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/eiq_examples/mpp/board_readme.md)

## Use-cases Description

HOW TO USE THE APPLICATION:
To classify an image, place an image in front of camera so that it fits in the blue rectangle in the middle of the display.
Some images will work better than others.

### High-level description
```
                                                                           +--------------------------------------------------------------+
                                                                           |                                                              |
                                                                           |                                                              |
                                                                          \ /                                                             |
             +-------------+     +-----------+     +-------------+      +-------------+      +------------------+       +-------------+   |
             |             |     |   JPEG    |     |             |      |             |      |                  |       |             |   |
Pipeline 0   |    camera   | --> |  decoder  | --> |  2D convert | -->  |   labeled   | -->  |    2D convert    |  -->  |    Display  |   |
             |             |     | (usb cam) |  |  |(color+flip) |      |  rectangle  |      | (rotation+scale) |       |             |   |
             +-------------+     +-----------+  |  +-------------+      +-------------+      +------------------+       +-------------+   |
                                                |                                                                                         |
                                                |    +-------------+      +--------------+      +-------------+                           |
                                                |    |             |      |              |      |             |                           |
Pipeline 1                                      +--> |  2D convert | -->  | ML Inference | -->  |  NULL sink  |                           |
                                                     |             |      |              |      |             |                           |
                                                     +-------------+      +--------------+      +-------------+                           |
                                                                                   |                                                      |
                                                                                   |                                                      |
    +-----------------+                                                            |                                                      |
    |  Main app:      |                                                            |                                                      |
    | ML output       |   <----- ML Inference output callback ---------------------+                                                      |
    | post processing |                                                                                                                   |
    |                 |   ------ Adding detected labeled rectangles ----------------------------------------------------------------------+
    +-----------------+
```
### Detailed description

Application creates two pipelines:

- One pipeline that runs the camera preview.
- Another pipeline that runs the ML inference on the image coming from the camera.
- Pipeline 1 is split from pipeline 0
- Pipeline 0 executes the processing of each element sequentially and CANNOT be preempted by another pipeline.
- Pipeline 1 is composed of two parts: one that CANNOT be preempted (2D convert element) and one that CAN be preempted (ML inference).

### Pipelines elements description

* Camera element is configured for a specific pixel format and resolution (board dependent)
* Display element is configured for a specific pixel format and resolution (board dependent)
* JPEG decoder element decodes the camera input stream from JPEG format to raw image data (YUV format)
  This element is present in the pipeline only when camera output is in JPEG format (eg. USB camera)
* First 2D convert element on pipeline 0 is configured to perform:
  - color space conversion from camera pixel format to the display pixel format
  - image flip if required by the display orientation
* Second 2D convert element on pipeline 0 is configured to perform:
  - rotation depending on the display orientation compared to camera orientation
  - scaling to match the display resolution
* 2D convert element on pipeline 1 is configured to perform:
  - color space conversion from camera pixel format to RGB888
  - cropping to maintain image aspect ratio
  - scaling to 128x128 as mandated by the image classification model
  - image flip if required by the display orientation

* The labeled rectangle element draws a crop window from which the camera image is sent to
  the ML inference element. The labeled rectangle element also displays the label of the object detected.
* The ML inference element runs an inference on the image pre-processed by the 2D convert element.
* The NULL sink element closes pipeline 1 (in MPP concept, only sink elements can close a pipeline).

* At every inference, the ML inference element invokes a callback containing the inference outputs.
These outputs are post-processed by the callback client component (in this case, the main task of the application)


## Running the demo

EXPECTED OUTPUTS:
The expected outputs of the example are:
- Detected label should be displayed on the screen
- Pipeline tasks statistics are displayed on the console
- Logs below should be displayed on the debug console

Logs for camera_mobilenet_view example using TensorFlow Lite Micro model should look like this:
```
[MPP_VERSION_1.0.0]
                   Inference Engine: TensorFlow-Lite Micro
API stats ------------------------------
rc_cycle = 43 ms rc_cycle_max 99 ms
pr_slot  = 56 ms pr_rounds 1 app_slot 4 ms
MPP stats ------------------------------
mpp 80082C40 exec_time 43 ms
mpp 80082F20 exec_time 52 ms
Element stats --------------------------
mobilenet : exec_time 48 ms
mobilenet : No label detected (0%)
API stats ------------------------------
rc_cycle = 43 ms rc_cycle_max 99 ms
pr_slot  = 56 ms pr_rounds 1 app_slot 4 ms
MPP stats ------------------------------
mpp 80082C40 exec_time 43 ms
mpp 80082F20 exec_time 52 ms
Element stats --------------------------
mobilenet : exec_time 49 ms
mobilenet : mosquito net (32%)
API stats ------------------------------
rc_cycle = 43 ms rc_cycle_max 99 ms
pr_slot  = 56 ms pr_rounds 1 app_slot 4 ms
MPP stats ------------------------------
mpp 80082C40 exec_time 43 ms
mpp 80082F20 exec_time 52 ms
```
## Important notes

TensorFLow Lite Micro is an optional engine for the ML Inference component of MPP.
This project embeds NXP's custom TensorFlow Lite Micro code by default.
TensorFLow Lite allows short-listing the "Operations" used by a specific model in order to reduce the binary image footprint.
This is done by implementing the function:

tflite::MicroOpResolver &MODEL_GetOpsResolver()

If this example uses TensorFLow Lite Micro, it implements its own function MODEL_GetOpsResolver dedicated to Mobilenet.
User may provide its own implementation of MODEL_GetOpsResolver when using a different model.

## Supported Boards
- MIMXRT700-EVK
- MIMXRT1170-EVKB
- FRDM-MCXN947
- FRDM-MCXN947T