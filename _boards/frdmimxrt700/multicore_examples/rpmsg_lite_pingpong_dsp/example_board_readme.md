Hardware requirements
=====================

- Mini/micro USB cable
- FRDM-IMXRT700 board
- Personal Computer

Board settings
==============

Unshort jumper JP27 to enable also Serial Console from Secondary Core.

Prepare the Demo
================

1. Connect the PC host and the board
2. Open a two serial terminals with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Demo Diagram
============

```
                  [ SHMEM 0 @ 0x20200000 | 4KB ]
                    [ RPMSG Inst 0 | SHMEM 0 ]

+------------------------+       MU4_A / MU4_B       +------------------+
| CM33_core0             |<=========================>| HIFI4            |
| rpmsg-master0,master1  |                           | rpmsg-slave0     |
+------------------------+                           +------------------+
 ॥    \                                                    |
 ॥     \  MU0_A / MU0_B                     /==============/
 ॥      \                                   ^
 ॥       \==================================================/
 ॥            /============================/^               |
 ॥ MU1_A /   /  MU2_A / MU2_B                               |
 ॥ MU1_B    /                                               |
+------------------------+       MU3_A / MU3_B       +------------------+
| CM33_core1             |<=========================>| HIFI1            |
| rpmsg-slave1,master2   |                           | rpmsg-slave2     |
+------------------------+                           +------------------+
[ SHMEM 1 @ 0x20201000 | 4KB ]
  [ RPMSG Inst 1 | SHMEM 1 ]
                                    [ SHMEM 2 @ 0x20588000 | 4KB ]
                                      [ RPMSG Inst 2 | SHMEM 2 ]

Communication (Default):
CM33_core0 -> MU1_A/MU1_B -> CM33_core1
CM33_core0 -> MU4_A/MU4_B -> HIFI4
CM33_core1 -> MU3_A/MU3_B -> HIFI1

Communication Cross-links (CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM=y):
CM33_core0 -> MU1_A/MU1_B -> CM33_core1
CM33_core0 -> MU0_A/MU0_B -> HIFI1
CM33_core1 -> MU2_A/MU2_B -> HIFI4
```

Running the demo
================

The log below shows the output of the RPMsg-Lite pingpong demo in the terminal window on primary core:

```
RPMsg demo starts Primary core.
Copy CORE1 image to address: 0x20600000, size: 102860
RPMsg demo starts DSP core 'rt700_hifi4_RI23_11_nlib'.

RPMsg demo starts
Sending data to Secondary core...
Primary core sending  a cm_msg. Message: Size=4, DATA = 0
Primary core received a cm_msg. Message: Size=4, DATA = 1
...
...
Primary core sending  a cm_msg. Message: Size=4, DATA = 98
Primary core received a cm_msg. Message: Size=4, DATA = 99
Sending data to HIFI4 core...
Primary core sending  a dsp_msg. Message: Size=4, DATA = 100
Primary core received a dsp_msg. Message: Size=4, DATA = 101
...
...
Primary core sending  a dsp_msg. Message: Size=4, DATA = 198
Primary core received a dsp_msg. Message: Size=4, DATA = 199

RPMsg demo ends
```

The log below shows the output of the RPMsg-Lite pingpong demo in the terminal window on secondary core:

```
RPMsg demo starts Secondary core.
RPMsg demo starts DSP core 'rt700_hifi1_RI23_11_nlib'.
Sending data to HIFI1 core...
Secondary core sending  a dsp_msg. Message: Size=4, DATA = 200
Secondary core received a dsp_msg. Message: Size=4, DATA = 201
...
...
Secondary core sending  a dsp_msg. Message: Size=4, DATA = 298
Secondary core received a dsp_msg. Message: Size=4, DATA = 299

RPMsg demo ends

```
