This file contains the general information which can be shared by the examples on this board.
The example specific information can be found in *readme.md* or *example_board_readme.md*.

General Example Settings
========================
Hardware requirements
---------------------
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Prepare the Demo
---------------
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Note on POR Boot
=====================
RT1180 application image needs a container header for POR boot, it provides rich features like ELE FW integration,
multiple image support, etc. NXP provide an versatile GUI tool - Secure Provisioning Tool to achieve these purposes.

On the other hand, for most of the demos from SDK, when compiled with ARMGCC/IAR/Keil, the generated outputs
are raw application images without container header. Such raw images are easy to be used with Secure Provisioning Tool. The
limitation is, even you select a flash target, after downloading it to the board, it can only run with debugger, but
can't support POR boot.

To support POR boot via Secure Provisioning Tool, please refer to the getting started document `Bootable image generation`
for detail guide.

For convenience of development, we also provide an option to convert the RAW application image to bootable
image via IDE build. An macro `XIP_BOOT_HEADER_ENABLE` is used to achieve this. The demo apps `hello_world_demo_cm33` and
`multicore_hello_world_cm33` are setting this by default. For other demos, the option need to be manually enabled.
Please refer to the getting started document `RAW/POR image switch` on how to enable this feature in different toolchains.
