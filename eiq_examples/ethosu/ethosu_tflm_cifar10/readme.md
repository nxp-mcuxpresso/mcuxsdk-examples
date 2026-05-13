# ethosu_tflm_cifar10

## Overview

This example demonstrates a **Convolutional Neural Network (CNN)** for image classification on MCUs using TensorFlow Lite Micro. It showcases the implementation of convolution, ReLU activation, pooling, and fully-connected layers. 

In this example, a static test images ("ship") is evaluated for classification into one of 10 CIFAR-10 classes.

### Key Features

- **Model Architecture**: CifarNet CNN with 3 convolution layers
- **Input**: 32×32 pixel color image
- **Output**: Classification into 10 classes
- **Model Size**: 91 KB
- **Detection Threshold**: 60%

### Network Structure

The neural network consists of:
1. 3 convolutional layers
2. ReLU activation functions (after each convolution)
3. Max pooling layers (interspersed)
4. Fully-connected output layer

### Model Origin

- **Training**: Based on scripts from [TensorFlow Models](https://github.com/tensorflow/models/tree/master/research/slim)
- **Configuration**: Modified to match CMSIS-NN CIFAR-10 example structure
- **Source Code**: Adapted from [TensorFlow Lite Label Image Example](https://github.com/tensorflow/tensorflow/tree/r2.3/tensorflow/lite/examples/label_image)

---

## Project Files

| File | Description |
|------|-------------|
| `main.cpp` | Example main function |
| `ship.bmp` | Static test image (source: [Wikipedia](https://en.wikipedia.org/wiki/File:Christian_Radich_aft_foto_Ulrich_Grun.jpg)) |
| `image_data.h` | Image converted to C array (RGB values) |
| `timer.c` | Timer source code |
| `image/*` | Image capture and pre-processing code |
| `get_top_n.cpp` | Top-N results retrieval |
| `model_data.h` | Model data converted from `.tflite` to C array |
| `model.cpp` | Model initialization and inference |
| `model_cifarnet_ops.cpp` | Model operations registration |
| `output_postproc.cpp` | Output post-processing |
| `video/*` | Camera and display handling |


### Project Structure in MCU SDK

Only primary files are listed here: 

```bash
Path_to_MCUSDK/mcuxsdk/examples/
├── eiq_examples/
│   ├── common/                # Common utilities for eIQ examples
│   │   ├── image/*            # Image capture and pre-processing module
│   │   └── video/*            # Camera and display handling module
│   └── ethosu_tflm_cifar10/          
|       ├── CMakeLists.txt
|       ├── image_data.h
|       ├── labels.h
|       ├── main.cpp
|       ├── readme.md
|       └── ship.bmp
└── _boards/
    └── <board_name>/
        └── eiq_examples/
            └── ethosu_tflm_cifar10/
                ├── example_board_readme.md
                └── npu    # NPU version of the model files
                    ├── cifarnet_quant_int8_vela.tflite
                    ├── model_cifarnet_ops_npu.cpp
                    └── model_data.h
```

### Project Structure in IDE


## Replace the model file


## Running the Demo


## Supported Boards with NPU
