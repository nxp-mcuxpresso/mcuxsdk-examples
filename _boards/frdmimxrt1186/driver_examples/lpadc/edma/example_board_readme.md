Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
============
Change Jumper J27 from 1-2 to 2-3
- ADC CHA1 input signal J51-8(GPIO_AD_14).

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC EDMA Example
Configuring LPADC...
ADC Full Range: 65536Full channel scale (Factor of 1).

Configuring LPADC EDMA...
Press any key to print output buffer:

0 = 21233
1 = 21188
2 = 21208
3 = 22037
4 = 21165
5 = 21278
6 = 21497
7 = 20709
8 = 21032
9 = 20935
10 = 20534
11 = 21283
12 = 21738
13 = 21204
14 = 21080
15 = 21315
16 = 26028
17 = 21379
18 = 21369
19 = 21382
20 = 20647
21 = 21073
22 = 21169
23 = 21088
24 = 21216
25 = 21220
26 = 21112
27 = 21230
28 = 21038
29 = 21065
30 = 21812
31 = 21055
32 = 21106
33 = 21020
34 = 21079
35 = 21124
36 = 20455
37 = 21289
38 = 21153
39 = 21129
40 = 20334
41 = 22759
42 = 22215
43 = 18957
44 = 21060
45 = 21091
46 = 20954
47 = 20765
48 = 21100
49 = 21154


Press any key to print output buffer:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

