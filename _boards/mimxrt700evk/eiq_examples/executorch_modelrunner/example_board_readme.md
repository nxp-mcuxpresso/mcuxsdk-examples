Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT700-EVK board
- Personal computer

Board settings
==============
1. Example running at 325MHz which need PMIC power supply.
2. PMIC is required to drive 1.1V on VDD2, then JP1 and JP3 on the RT700 EVK need to be shorted for proper operation

Prepare the Demo
================
1. Connect a USB cable between the host PC and the J-Link USB port on the target board.
2. Open a serial terminal (115200 baud, 8N1, no flow control).
3. Download the program to the target board.
4. Reset the board / launch the debugger.

Run the Demo
================
After reset the board prints:

    ExecuTorch Modelrunner Ready

From then on the firmware speaks a binary request/response protocol on the
same UART (it must be the only traffic — do not type into the terminal).
Use the PC-side tool to load a model, feed an input tensor and run inference:

    python scripts/run.py --port <serial-port> --baud 115200
    (modelrunner|no model)> model <model>.pte
    (modelrunner|model loaded)> input <input>.bin
    (modelrunner|input ready)> run

The `run` command prints the inference time (measured with the DWT cycle
counter, pure inference time) and the raw output tensor values.
