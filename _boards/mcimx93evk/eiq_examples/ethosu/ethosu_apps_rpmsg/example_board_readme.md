Hardware requirements
=====================
- Micro USB cable
- MIMX93-EVK board
- J-Link Debug Probe
- 5V power supply
- Personal Computer

Board settings
============
No special settings are required.

#### Please note this application can't support running with Linux BSP! ####

Prepare the Demo
===============
1.  Connect 5V power supply and J-Link Debug Probe to the board, switch SW1 to power on the board.
2.  Connect a micro USB cable between the host PC and the J6 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
This example is built as `.out` or `.elf` and installed in `rootfs`. In the Linux command line, users can run the following command to perform inference for the model:

```
# ./inference_runner -d /dev/ethosu0 -n mobilenet_v1_1.0_224_pb_int8_vela.tflite -i stopwatch.bmp -l labels.txt -o output_data_detection -t 1080000000
```

When the example runs, the log and inference result appear on the terminal as shown below:

```
Send Ping
Send version request
Send capabilities request
Capabilities:
        version_status:1
        version:{ major=0, minor=0, patch=0 }
        product:{ major=6, minor=0, patch=0 }
        architecture:{ major=1, minor=0, patch=6 }
        driver:{ major=0, minor=16, patch=0 }
        macs_per_cc:8
        cmd_stream_version:0
        custom_dma:false
Create network
Create inference
Wait for inferences
Inference status: success

Detected: jinrikisha, confidence:70
Detected: horse cart, confidence:70
Detected: academic gown, confidence:70
Detected: howler monkey, confidence:70
```
