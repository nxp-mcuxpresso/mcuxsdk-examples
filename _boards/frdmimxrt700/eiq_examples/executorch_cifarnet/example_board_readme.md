Hardware requirements
=====================
- Type-C USB cable
- FRDMIMXRT700 board
- Personal computer

Board settings
==============
1. Example running at 325MHz which need PMIC power supply.
2. PMIC is required to drive 1.1V on VDD2, then JP1 and JP3 on the RT700 EVK need to be shorted for proper operation

Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Run the Demo
================
The log below shows the output of the demo in the terminal window (compiled with ARM GCC):

CIFARNET example using a ExecuTorch model
Model buffer loaded, has 11 methods
Running method forward
Setting up planned buffer 0, size 3088.
Method loaded.
Preparing inputs...
Input prepared.
Starting the model execution...
Model executed successfully.
Core/NPU Frequency: 324 MHz
method_allocator Addr: 0x20000000 - 0x20080000
method_allocator_used: Total 0x80000 (524288 B); Used 0xdb9 (3513 B); Used/Total 0 %
temp_allocator Addr: 0x20080000 - 0x20100000
temp_allocator_used: Total 0x80000 (524288 B); Used 0x3f27e4 (4138980 B); Used/Total 789 %
Model Addr: 0x20430000 - 0x20448d60
Model Size: 0x18d60 (101728 B)
Total Size Used: 4244221 B (Model (101728 B) + method_allocator (3513 B) + temp_allocator (4138980 B))
1 outputs:
----------------------------------------
     Inference time: 1072 us
     Detected: cat (99%)
----------------------------------------
Output[0][0]: -128
Output[0][1]: -128
Output[0][2]: -128
Output[0][3]: 127
Output[0][4]: -128
Output[0][5]: -128
Output[0][6]: -128
Output[0][7]: -128
Output[0][8]: -128
Output[0][9]: -128
Program complete, exiting.