# static_image_nanodet_persondetect_view

## Overview

This example shows how to use the library to create two parallel object detection pipelines using a static image file as source.

The machine learning framework used is TensorFlow Lite Micro.
Two models are used in parallel:
- Nanodet M: A quantized object detection model that detects multiple objects across 80 classes
- PersonDetect: A person detection model specialized for detecting people

## Toolchains supported
Refer to build instructions:
- [Build](../../../../middleware/eiq/mpp/Build.md)

## Hardware requirements
Refer to board.readme for hardware setup requirements.
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/eiq_examples/mpp/board_readme.md)

## Use-case Description

### High-level description

```
                                                                   +---------------------------------------------------------+
                                                                   |                                                         |
                                                                   |                                                         |
                                                                  \ /                                                        |
                  +-------------+      +-------------+      +-------------+      +-------------+       +-------------+       |
                  |    static   |      |             |      |             |      |             |       |             |       |
Pipeline 0        |     image   | -->  |  2D convert | -->  |   labeled   | -->  | 2D convert  |  -->  |    Display  |       |
                  |  320x320    |  |   |(color+scale)|      |  rectangle  |      | (rotation)  |       |             |       |
                  |    BGRA     |  |   +-------------+      +-------------+      +-------------+       +-------------+       |
                  +-------------+  |                                                                                         |
                                   |                                                                                         |
                                   |     +-------------+      +--------------+      +-------------+                          |
                                   |     |             |      |              |      |             |                          |
Pipeline 1                         +---> |  2D convert | -->  | ML Inference | -->  |  NULL sink  |                          |
(Nanodet)                          |     |(crop+scale+ |      |   Nanodet    |      |             |                          |
                                   |     | color conv) |      |              |      +-------------+                          |
                                   |     +-------------+      +--------------+                                               |
                                   |                                   |                                                     |
                                   |                                   |                                                     |
                                   |     +-------------+      +--------------+      +-------------+                          |
                                   |     |             |      |              |      |             |                          |
Pipeline 2                         +---> |  2D convert | -->  | ML Inference | -->  |  NULL sink  |                          |
(PersonDetect)                           |(crop+scale+ |      | PersonDetect |      |             |                          |
                                         | color conv) |      |              |      +-------------+                          |
                                         +-------------+      +--------------+                                               |
                                                                       |                                                     |
                                                                       |                                                     |
    +-----------------+                                                |                                                     |
    |  Main app:      |                                                |                                                     |
    | ML output       |   <----- ML Inference output callbacks --------+                                                     |
    | post processing |                                                                                                      |
    |                 |   ------ Adding detected labeled rectangles ---------------------------------------------------------+
    +-----------------+
```

### Detailed description

Application creates three pipelines:

- Pipeline 0: Runs the static image preview and display
- Pipeline 1: Runs the Nanodet ML inference on the static image
- Pipeline 2: Runs the PersonDetect ML inference on the static image
- Pipelines 1 and 2 are split from pipeline 0
- Pipeline 0 and 2 execute the processing of each element sequentially and CANNOT be preempted by another pipeline
- Pipelines 1 executes the processing of each element sequentially but CAN be preempted

### Pipelines elements description

**Source:**
* Static image 320x320 BGRA format (skigirl_COCO_320_320_bgra)

**Pipeline 0 (Display branch):**
* Display element is configured for a specific pixel format and resolution (board dependent)
* 2D convert element is configured to perform:
  - Color space conversion from BGRA to the display pixel format
  - Scaling from 320x320 to the display resolution
* Labeled rectangle element draws:
  - A detection zone rectangle showing the crop window sent to ML inference
  - Bounding boxes and labels for objects detected by Nanodet (in blue)
  - Bounding boxes and labels for persons detected by PersonDetect (in red)
* Optional 2D convert element for rotation (if display orientation requires it)

**Pipeline 1 (Nanodet branch):**
* 2D convert element is configured to perform:
  - Cropping to maintain image aspect ratio within the detection zone
  - Scaling to the Nanodet model input size
  - Color space conversion from BGRA to RGB888
* ML inference element runs Nanodet model inference
* NULL sink element closes the pipeline

**Pipeline 2 (PersonDetect branch):**
* 2D convert element is configured to perform:
  - Cropping to maintain image aspect ratio within the detection zone
  - Scaling to the PersonDetect model input size
  - Color space conversion from BGRA to RGB888
* ML inference element runs PersonDetect model inference
* NULL sink element closes the pipeline

**Inference callbacks:**
* At every inference, each ML inference element invokes a callback containing the inference outputs
* The callback (mpp_event_listener) processes outputs from both models:
  - Nanodet outputs are post-processed using NANODET_ProcessOutput to determine detected objects and bounding boxes
  - PersonDetect outputs are post-processed using Persondetect_Output_postprocessing
* Non-Maximum Suppression (NMS) is performed to pick the best box for each detected object
* Detected boxes are converted to labeled rectangles and updated on the display
* The main task periodically prints detection statistics to the console

## Running the demo

EXPECTED OUTPUTS:
The expected outputs of the example are:
- A detection zone rectangle (green) displayed on the screen showing the inference region
- For each object detected by Nanodet, a labeled rectangle (blue) with the object class name
- For each person detected by PersonDetect, a labeled rectangle (red) with "person" label
- Logs below should be displayed on the debug console every second

Logs for static_image_nanodet_persondetect_view example should look like this:

```
[MPP_VERSION_4.1.b346f234-dirty]
Inference Engine: TensorFlow-Lite Micro
===========================================================
nanodet : inference time 60 ms (frame num 5)
nanodet : box 0 label person score 77(%)
presondet : inference time 6 ms (frame_num 5)
persondet : box 0 label person score 84(%)
===========================================================
===========================================================
nanodet : inference time 60 ms (frame num 9)
nanodet : box 0 label person score 77(%)
presondet : inference time 6 ms (frame_num 9)
persondet : box 0 label person score 84(%)
===========================================================
```

## Important notes

**TensorFlow Lite Micro:**
TensorFlow Lite Micro is an optional engine for the ML Inference component of MPP.
This project embeds NXP's custom TensorFlow Lite Micro code by default.
TensorFlow Lite allows short-listing the "Operations" used by a specific model in order to reduce the binary image footprint.
This is done by implementing the function:

```c
tflite::MicroOpResolver &MODEL_GetOpsResolver()
```

This example implements its own function MODEL_GetOpsResolver dedicated to both Nanodet and PersonDetect models.
User may provide their own implementation of MODEL_GetOpsResolver when using different models.

**Model Information:**
- Nanodet model info is included via APP_TFLITE_NANODET_INFO
- PersonDetect model info is included via APP_TFLITE_PERSONDETECT_INFO
- Each model has its own tensor arena size and offset configuration
- Models use different input sizes and preprocessing parameters

**Performance:**
Both models run on separate pipelines. The Nanodet model runs on a preemptable pipeline (MPP_EXEC_PREEMPT), while the PersonDetect model runs on a non-preemptable pipeline (MPP_EXEC_RC). They are executed sequentially, not in parallel, but the persondetect might be interrupted by nanodet.
