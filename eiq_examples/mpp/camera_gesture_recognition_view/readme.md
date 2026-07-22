# camera_gesture_recognition_view

## Overview

This example shows how to use the library to create a use-case for
hand detection, hand landmark detection, and gesture recognition using camera as source.

The machine learning framework used for this example is TensorFlow Lite Micro.
The application uses a cascade of four models:
1. **Blaze Hand Detector** - Detects hands in the input image and provides bounding boxes with rotation angles
2. **Hand Landmark Detector** - Provides 21 3D hand keypoints (landmarks) for detected hands
3. **Gesture Embedder** - Converts hand landmarks into feature embeddings
4. **Gesture Classifier** - Classifies hand gestures based on embeddings (e.g., thumbs up, peace sign, pointing, etc.)

## Toolchains supported
Refer to build instructions:
- [Build](../../../../middleware/eiq/mpp/Build.md)

## Hardware requirements
Refer to board.readme for hardware setup requirements.
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/eiq_examples/mpp/board_readme.md)

## Use-cases Description

HOW TO USE THE APPLICATION:

### High-level description
```
                                                                              +------------------------------------------------------------------+
                                                                              |                                                                  |
                                                                              |                                                                  |
                                                                             \ /                                                                 |
                +-------------+     +-----------+     +-------------+      +-------------+      +------------------+       +-------------+       |
                |             |     |   JPEG    |     |             |      |             |      |                  |       |             |       |
Pipeline 0      |    camera   | --> |  decoder  | --> |  2D convert | -->  |   labeled   | -->  |    2D convert    |  -->  |    Display  |       |
                |             |     | (usb cam) | |   |(color+flip) |      |  rectangle  |      | (rotation+scale) |       |             |       |
                +-------------+     +-----------+ |   +-------------+      +-------------+      +------------------+       +-------------+       |
                                                  |                                                                                              |
                                                  |                                                                                              |
                                                  |     +-------------+      +--------------+      +-------------+                               |
                                                  |     |             |      |  Blaze Hand  |      |             |                               |
Pipeline 1                                        +---> |  2D convert | -->  |   Detector   | -->  |  NULL sink  |                               |
(Hand Detection)                                  |     | (crop+scale)|      | (TFLite BG)  |      |             |                               |
                                                  |     +-------------+      +--------------+      +-------------+                               |
                                                  |                                   |                                                          |
                                                  |                                   | (hand detected)                                          |
                                                  |                                   +-------------------------------------------------------+  |
                                                  |     +-------------+      +--------------+      +------------------+      +-------------+  |  |
                                                  |     |             |      |     Hand     |      |     Gesture      |      |   Gesture   |  |  |
Pipeline 2                                        +---> |  2D convert | -->  |   Landmark   | ---> |    Embedder      | ---> | Classifier  |  |  |
(Landmark & Gesture)                                    |(crop+rotate)|      |  (TFLite BG) |      |   (TFLite BG)    |      | (TFLite BG) |  |  |
                                                        +-------------+      +--------------+      +------------------+      +-------------+  |  |
                                                                                      |                                             |         |  |
                                                                                      |                                             |         |  |
                                                                                      |                                             |         |  |
       +-----------------+                                                            |                                             |         |  |
       |  Main app:      |                                                            |                                             |         |  |
       | ML output       |   <----- ML Inference output callbacks --------------------+---------------------------------------------+---------+  |
       | post processing |                                                                                                                       |
       |                 |   ------ Update labeled rectangles and landmarks ---------------------------------------------------------------------+
       +-----------------+
```
### Detailed description

Application creates multiple pipelines with dynamic switching:

- **Pipeline 0**: Camera preview pipeline that displays the camera feed with overlaid detection results
- **Pipeline 1**: Hand detection pipeline using Blaze Hand Detector (runs initially)
- **Pipeline 2**: Hand landmark and gesture recognition pipeline (activated when hand is detected)
- Pipeline 1 and 2 are split from pipeline 0
- Pipeline 0 executes the processing of each element sequentially and CANNOT be preempted by another pipeline
- Pipeline 1 and 2 are composed of two parts each:
     - One that CANNOT be preempted (2D convert element)
     - One that CAN be preempted (ML inference elements running in background tasks)

### Pipeline Switching Logic

1. **Initial State**: Pipeline 1 (Blaze Hand Detector) runs continuously
2. **Hand Detected**: When a hand is detected:
      - Pipeline 1 stops
      - Pipeline 2 starts (Hand Landmark → Gesture Embedder → Gesture Classifier)
      - Bounding box is used to crop and rotate the hand region for landmark detection
3. **Hand Lost**: When hand is no longer detected:
      - Pipeline 2 stops
      - Pipeline 1 restarts to search for hands again

### Pipelines elements description

* **Camera element** is configured for a specific pixel format and resolution (board dependent)
* **Display element** is configured for a specific pixel format and resolution (board dependent)
* **JPEG decoder element** decodes the camera input stream from JPEG format to raw image data (YUV format)
     This element is present in the pipeline only when camera output is in JPEG format (eg. USB camera)
* **First 2D convert element on pipeline 0** is configured to perform:
     - color space conversion from camera pixel format to the display pixel format
     - image flip if required by the display orientation
* **Second 2D convert element on pipeline 0** is configured to perform:
     - rotation depending on the display orientation compared to camera orientation
     - scaling to match the display resolution
* **2D convert element on pipeline 1** (Blaze detector input) is configured to perform:
     - color space conversion from camera pixel format to RGB888
     - cropping to maintain image aspect ratio (detection zone: 480x480 centered on 640x480 frame)
     - scaling to match the model input dimensions (192x192 for Blaze detector)
     - image flip if required by the display orientation
* **2D convert element on pipeline 2** (Hand landmark input) is configured to perform:
     - color space conversion from camera pixel format to RGB888
     - dynamic cropping based on detected hand bounding box
     - dynamic rotation based on hand rotation angle from Blaze detector
     - scaling to match the model input dimensions (224x224 for Hand Landmark detector)
     - image flip if required by the display orientation

* **The labeled rectangle element** draws:
     - A green detection zone window (480x480) showing the area monitored for hand detection
     - Blue bounding boxes with "hand" labels for detected hands (from Blaze detector)
     - Red landmark points (7 keypoints) from Blaze detector when DISPLAY_BLAZE_PALM_RECTS_LDMKS is enabled
     - Green landmark points (21 3D keypoints) from Hand Landmark detector representing finger joints and palm positions
  
* **The ML inference elements** run inferences on pre-processed images:
     - **Blaze Hand Detector**: Detects hands and provides bounding boxes, rotation angles, and 7 palm landmarks
     - **Hand Landmark Detector**: Provides 21 3D hand keypoints (x, y, z coordinates) and handedness (left/right)
     - **Gesture Embedder**: Converts 21 3D landmarks into a 128-dimensional feature vector
     - **Gesture Classifier**: Classifies gestures into categories (e.g., "Closed_Fist", "Open_Palm", "Pointing_Up", "Thumb_Up", "Victory", etc.)
  
* **The NULL sink elements** close pipeline 1 and 2 (in MPP concept, only sink elements can close a pipeline)

* At every inference, the ML inference elements invoke callbacks containing the inference outputs.
These outputs are post-processed by the callback client component (in this case, the main task of the application)

## Running the demo

EXPECTED OUTPUTS:
The expected outputs of the example are:
- A green detection zone rectangle displayed on the screen (480x480 centered)
- For each detected hand, a blue labeled rectangle with "hand" label
- For each detected hand, 21 green landmark points representing hand keypoints (fingertips, joints, palm)
- Gesture classification results displayed on the console
- Inference timing information for all four models
- Logs below should be displayed on the debug console

Logs for camera_gesture_recognition_view example using TensorFlow Lite Micro model should look like this:
```
[MPP_VERSION_3.6.0]
Inference Engine: TensorFlow-Lite Micro

=== Camera Parameters ===
APP_CAMERA_WIDTH: 640
APP_CAMERA_HEIGHT: 480

=== Model Input Dimensions ===
BLAZE_DETECTOR_WIDTH: 192
BLAZE_DETECTOR_HEIGHT: 192

=== Display Dimensions ===
DISPLAY_SMALL_DIM: 480
DISPLAY_LARGE_DIM: 640
APP_DISPLAY_WIDTH: 640
APP_DISPLAY_HEIGHT: 480
SWAP_DIMS: 0

=== Detection Zone Parameters ===
DETECTION_ZONE_RECT_WIDTH: 480
DETECTION_ZONE_RECT_HEIGHT: 480
DETECTION_ZONE_RECT_LEFT: 80
DETECTION_ZONE_RECT_TOP: 0

=== Crop Parameters ===
CROP_SIZE_LEFT: 480
CROP_SIZE_TOP: 480
CROP_LEFT: 80
CROP_TOP: 0

=== Boxes Offset Parameters ===
BOXES_OFFSET_LEFT: 80
BOXES_OFFSET_TOP: 0

=======================================================
inference time blaze palm model: 22 ms 
inference time hand landmark model: 0 ms 
inference time gesture embedder model: 0 ms 
inference time gesture classifier model: 0 ms 
=======================================================

=======================================================
inference time blaze palm model: 21 ms 
inference time hand landmark model: 0 ms 
inference time gesture embedder model: 0 ms 
inference time gesture classifier model: 0 ms 
=======================================================

=======================================================
inference time blaze palm model: 22 ms 
inference time hand landmark model: 35 ms 
inference time gesture embedder model: 8 ms 
inference time gesture classifier model: 2 ms 
Detected hand score: 98%
Left hand: no
Detected gesture: Thumb_Up (95%)
=======================================================

=======================================================
inference time blaze palm model: 0 ms 
inference time hand landmark model: 36 ms 
inference time gesture embedder model: 8 ms 
inference time gesture classifier model: 2 ms 
Detected hand score: 97%
Left hand: no
Detected gesture: Thumb_Up (94%)
=======================================================

=======================================================
inference time blaze palm model: 0 ms 
inference time hand landmark model: 35 ms 
inference time gesture embedder model: 8 ms 
inference time gesture classifier model: 2 ms 
Detected hand score: 99%
Left hand: yes
Detected gesture: Victory (92%)
=======================================================
```

## Important notes

TensorFLow Lite Micro is an optional engine for the ML Inference component of MPP.
This project embeds NXP's custom TensorFlow Lite Micro code by default.
TensorFLow Lite allows short-listing the "Operations" used by a specific model in order to reduce the binary image footprint.
This is done by implementing the function:

tflite::MicroOpResolver &MODEL_GetOpsResolver()

This example implements its own function MODEL_GetOpsResolver dedicated to the four models used:
- Blaze Hand Detector
- Hand Landmark Detector
- Gesture Embedder
- Gesture Classifier

User may provide its own implementation of MODEL_GetOpsResolver when using different models.

## Model Details

### Blaze Hand Detector Model:
- **Input**: 192x192 RGB image
- **Output**: Hand bounding boxes, 7 palm landmarks per detected hand, and rotation angles
- **Purpose**: Initial hand detection and localization
- **Landmarks**: 7 keypoints representing palm region (used for hand orientation)

### Hand Landmark Detector Model:
- **Input**: 224x224 RGB image (cropped and rotated hand region)
- **Output**: 21 3D hand landmarks (x, y, z coordinates), hand presence score, handedness (left/right)
- **Purpose**: Detailed hand keypoint detection for gesture recognition
- **Landmarks**: 21 keypoints representing:
     - Wrist (1 point)
     - Thumb (4 points)
     - Index finger (4 points)
     - Middle finger (4 points)
     - Ring finger (4 points)
     - Pinky finger (4 points)

### Gesture Embedder Model:
- **Input**: 63 values (21 landmarks × 3 coordinates)
- **Output**: 128-dimensional feature embedding vector
- **Purpose**: Convert hand landmarks into a compact representation for gesture classification

### Gesture Classifier Model:
- **Input**: 128-dimensional embedding vector
- **Output**: Gesture classification scores for multiple gesture categories
- **Purpose**: Classify hand gestures into predefined categories
- **Supported Gestures**: Closed_Fist, Open_Palm, Pointing_Up, Thumb_Down, Thumb_Up, Victory, ILoveYou, and more

### Visualization:
- **Detection Zone**: Green rectangle (480x480)
- **Blaze Detector Output**: Blue bounding boxes with "hand" labels
- **Blaze Palm Landmarks**: Red points (7 keypoints) - optional, controlled by DISPLAY_BLAZE_PALM_RECTS_LDMKS
- **Hand Landmarks**: Green points (21 keypoints) - displayed when hand is tracked
- Each landmark is displayed with a 4-pixel width for easy identification
