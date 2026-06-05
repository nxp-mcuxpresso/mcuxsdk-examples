Overview
========

This readme explains how to run face recognition application example.
This version of face recognition solution does not support kex package. To build the application, use MCUXpresso SDK from the NXP Bitbucket repository.
The face recognition applications can be run in VSCode with MCUXpresso extension or in MCUXpresso IDE. The application demonstrates real-time face detection and recognition using a USB camera input on RT700 NXP development board.
The application uses a pre-built core1 image that handles the USB camera and sends the frames to core0 for face recognition processing via RPMsg lite IPC mechanism.

⚠️ **AI Components Disclaimer**

The AI components are provided solely for demonstration, evaluation, and benchmarking purposes and are not designed, licensed, or authorized for production use, deployment, or reliance in any operational setting or safety-related environment. Any use beyond NXP's expressly defined intended purpose is undertaken entirely at the Licensee's own risk and responsibility, including all applicable regulatory, compliance, and liability obligations. Nothing herein shall be construed as a representation or warranty that the AI components are production-ready or suitable for use in real-world or regulated environments. The AI components may not be used to train, fine tune, adapt, or create derivative AI models, nor to extract or reuse model artefacts, except as expressly permitted for the stated demonstration purposes.

The pipeline of the application is as follows:

```
                     |                                                                   +--------------------------------------------------------------+
      CORE1          |                                                CORE0              |                                                              |
                     |                                                                   |                                                              |
                     |                                                                  \ /                                                             |
                     |                         +-----------+     +-------------+      +-------------+      +------------------+      +-------------+    |
                     |                         |   JPEG    |     |             |      |             |      |                  |      |             |    |
                     |      Pipeline 0     +-> |  decoder  | --> |  2D convert | -->  |   labeled   | -->  |   IMG compose    | -->  |   Display   |    |
                     |                     |   | (HW/RGB)  |     |(color+flip) |      |  rectangle  |      | (logo+text+inf)  |      |             |    |
   +-------------+   |                     |   +-----------+     +-------------+      +-------------+      +------------------+      +-------------+    |
   |             |   |     +-----------+   |                                                                                                            |
   | USB camera  |   |     |   USB     |   |                                                                                                            |
   | (dual stream| --+-----|  Virtual  | --+                                                                                                            |
   |  RGB + IR)  |   |     | Cam elem  |   |                                                                                                            |
   |             |   |     +-----------+   |   +-------------+      +-------------+      +------------------+      +-------------+     +-------------+  |
   +-------------+   |                     |   |   JPEG      |      |             |      |                  |      |             |     |             |  |
                     |       Pipeline 1    +-> |  decoder    | -->  |  2D convert | -->  | Image Quality    | -->  | ML Inference| --> |  NULL sink  |  |
                     |                         | (HW/IR)     |      |             |      | Check            |      | (SCRFD/     |     |             |  |
                     |                         +-------------+      +-------------+      +------------------+      | Antispoofing|     +-------------+  |
                     |                                                                                             | /MobileFace)|                      |
                     |                                                                                             +-------------+                      |
                     |                                                                                                   |                              |
                     |                                                                                                   |                              |
                     |                     +-----------------+                                                           |                              |
                     |                     |  Main app:      |                                                           |                              |
                     |     User app clbk   | ML output       |   <------- ML Inference output callback ------------------+                              |
                     |                     | post processing |                                                                                          |
                     |                     | State machine   |   -------- Adding detected labeled rectangles + updating text area ----------------------+
                     |                     +-----------------+
```  

### Detailed description

Application creates two pipelines:

- One pipeline that runs the camera preview with labeled rectangles and composed output.
- Another pipeline that runs the ML inference on the IR stream from the USB camera.
- Pipeline 1 is split from pipeline 0
- Pipeline 0 executes the processing of each element sequentially and CANNOT be preempted by another pipeline.
- Pipeline 1 is composed of two parts: one that CANNOT be preempted (JPEG decoder + 2D convert + Image Quality Check) and one that CAN be preempted (ML inference).

### Pipelines elements description

* USB Camera is running on CORE1 and produces dual stream output (RGB + IR) in JPEG format (it is not part from the pipeline)
* Display element is configured for a specific pixel format and resolution (board dependent)
* JPEG decoder elements decode both RGB and IR camera streams from JPEG format to raw image data
* First 2D convert element on pipeline 0 is configured to perform:
  - color space conversion from camera pixel format to the display pixel format
  - image flip if required by the display orientation
* 2D convert element on pipeline 1 is configured to perform:
  - color space conversion from IR camera pixel format to RGB888
  - cropping to maintain image aspect ratio
  - scaling to match the model input size (dynamically updated based on active model: SCRFD_KPS, Antispoofing, or MobileFaceNet)
  - image flip if required by the display orientation

* The Image Quality Check element validates image brightness and contrast before recognition
* The labeled rectangle element draws a detection zone and displays detected face boxes with labels
* The IMG Compose element combines multiple images:
  - Camera preview (main area)
  - NXP logo (top area)
  - Text information (user name, confidence, inference times, state)
  - Inference preview (optional debug feature)
* The ML inference element dynamically switches between three models:
  - SCRFD_KPS: Face detection with keypoints
  - Antispoofing: Liveness detection (real vs fake face)
  - MobileFaceNet: Face recognition/embedding extraction
* The NULL sink element closes pipeline 1 (in MPP concept, only sink elements can close a pipeline).

* At every inference, the ML inference element invokes a callback containing the inference outputs.
These outputs are post-processed by the callback client component (in this case, the main task of the application)
which implements a state machine to manage the face recognition workflow:
  - STATE_DETECTING: Running SCRFD_KPS for face detection
  - STATE_DETECTED: Face detected, preparing for antispoofing check
  - STATE_CHECKING_ANTISPOOFING: Running antispoofing model
  - STATE_SPOOF: Fake face detected
  - STATE_REAL: Real face confirmed, preparing for recognition
  - STATE_RECOGNIZING: Running MobileFaceNet for face recognition
  - STATE_RECOGNIZED: Face successfully recognized
  - STATE_NOT_RECOGNIZED: Face not found in database (triggers registration if in registration mode)
  - STATE_NOTIFYING_USER: Displaying results to user
  - STATE_USER_NOTIFIED: Ready to return to detection state

* The Antispoofing model is disabled by default and can be enabled via configuration (set USE_ANTISPOOFING to 1 in the application configuration header - mpp_config.h file at path ./examples/_boards/mimxrt700evk/eiq_examples/mpp/examples/camera_usb_final_fr_app_view/cm33_core0/mpp_config.h)


Running the application
=======================
Follow the following steps to build the application.

1) This documentation assumes that user have west installed and environment configured. Refer to SDK documentation to install west and setup your environment https://kex-daily.nxp.com/mcuxsdk-internal/latest/html/gsd/installation.html .

3) Install the MCUXpresso SDK and ensure all dependencies are properly configured in your environment. Follow instructions from the MCUXpresso SDK documentation: https://kex-daily.nxp.com/mcuxsdk-internal/latest/html/gsd/install/github.html

4) Under mcuxpresso-sdk/mcuxsdk directory, run the following command to build the camera_usb_final_fr_app_view example:
* On RT700: west build -b mimxrt700evk ./examples/eiq_examples/mpp/camera_usb_final_fr_app_view -p always --config flash_release --toolchain armgcc -Dcore_id=cm33_core0

5) Download the program to the target board then press the reset button (SW2) to begin running the demo (CPU reset via MCU-Link/CMSIS may not work).

Output
======

1) Running  the camera_usb_final_fr_app_view example:
* Please type "start fr" on console to start the face recognition pipeline
* Initially, there will be no face recognized as no face was previously registered. On the right hand side, unde the NXP logo, you will se the user name Unknown.

Note that on console, you won't see anything until a face is registered

* to register a new user, type "add 'your name' x", where x is the timeout in seconds for the registration process.
* Once a face is successfully registered, the application will automatically stop the face recognition and will enter in display only mode
* To restart the face recognition pipeline, type "start fr" on console again. This time, the registered face will be recognized and displayed with the corresponding user name on the right side of the screen. The console output will show the recognized username and similarity score as shown below:

```
*** Recognized username ***
[CTRL] Recognized username, similarity 90
```

* To register additional users, repeat the process by typing "add 'your name' x" with a new name and timeout value
* To see the full list of supported console commands, type "help" on the console
