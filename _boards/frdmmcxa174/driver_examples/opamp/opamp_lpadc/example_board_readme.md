  Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA174 board
- Personal Computer

Board settings
Please connect OPAMP1 positive input channel (SJ2-3) to the GND.
Please connect OPAMP1 negative input channel (SJ3-3) to a voltage source.
Please connect OPAMP1 negative input channel (SJ3-3) to J2-9.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for the description of how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One-stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
 Opamp lpadc example.
 Please press any key to get opamp output value.
 Opamp output voltage = 818mv
 Opamp negative input voltage = 80mv
 Opamp theoretical output value = 903mv
 Opamp output voltage = 611mv
 Opamp negative input voltage = 100mv
 Opamp theoretical output value = 699mv
 Opamp output voltage = 416mv
 Opamp negative input voltage = 120mv
 Opamp theoretical output value = 495mv
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Note: the OPAMP1 maximum output voltage is 3.1v, users need to pay attention to the input voltage value of the negative channel.
For FRDM-MCXA174 board:
Inp = Vp + (Vdd - Vp) * 980 / (20000 + 980)
Inn = Vn + (Vout - Vn) * 980 / (10000 + 980)
Inp = Inn, VDD is about 3287mv.

1. Set opamp negative input to 80mv, opamp theoretical output value is about 900.
2. Set opamp negative input to 100mv, opamp theoretical output value is about 700.
3. Set opamp negative input to 120mv, opamp theoretical output value is about 500,

Slight changes in VDD, input voltage, peripheral circuits, the offset inside the OPAMP, and the LPADC measurement error will cause
a difference between the theoretical value and the actual value. A difference within 100mv is considered normal.
