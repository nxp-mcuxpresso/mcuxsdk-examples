Hardware requirements
===================
- Type-C USB cable
- FRDM-MCXC242 board
- Personal Computer

Board settings
==============
MCU Link firmware version 2.263 and above needs to be used
Use libusbsio Python module version 2.1.11 (https://pypi.org/project/libusbsio/2.1.11)

Prepare the Demo
===============
1.  Connect a type-c USB cable between the PC host and the MCULink USB port (J9) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

For detailed instructions, see the appropriate board User's Guide.

Running the demo
================
When the target device application is running execute Python run_uart.py from boards\frdmmcxc242\multiprocessor_examples\erpc_common\erpc_matrix_multiply folder:
python run_uart.py --client --port <COMxx port of the MCU-LINK>

When the demo runs successfully, the log below shows the output of the Client eRPC Matrix Multiply demo
in the Python Shell window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Matrix #1
=========
  21   33   37   37    9
  23   45   43    0   32
  38   44    8   15   36
  18   18   38   44   16
  22   23    0   38    7

Matrix #2
=========
  11   23   27   45   11
   7   19   23   24    6
  32   26   49   43   16
  22   48   36   34   41
  27   20   32   31   11

eRPC request is sent to the server

Result matrix
=============
2703 4028 4759 4865 2637
2808 3142 4787 4956 1563
2284 3358 4122 4736 1821
2940 4176 4858 4868 2894
1428 2907 2715 3051 2015

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
