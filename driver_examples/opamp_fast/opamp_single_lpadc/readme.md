# opamp_fast_lpadc

## Overview

The OPAMP single lpadc example demonstrates how to use the OPAMP driver.
In this example, the reference voltage (Vref) of the OPAMP non-inverting input
port comes from the VDD, the OPAMP inverting input port needs to be connected
to a voltage source and an LPADC channel, the OPAMP non-inverting input port
needs to be connected to GND, and the OPAMP output port needs to be connected
to an LPADC channel. 

When this example runs successfully, the terminal will print the OPAMP output
port voltage value, the OPAMP inverting input port voltage value, and the
theoretical OPAMP output value.

The theoretical calculated value should be very close to the value
measured by LPADC.

OPAMP input and output relationship formula: 
1. Inn=Vp+(Vout-Vp)*Rfb1/(Rfb1+Rfb2)=Vp+(Vout-Vp)/(gain+1)
2. Inp=Vn+(Vref-Vn)*Rfb1/(Rfb1+Rfb2)=Vn+(Vref-Vn)/(gain+1)
3. inn=inp => Vp*gain+Vout=Vn*gain+Vref, Vout=Vref-(Vp-Vn)*gain

## Supported Boards
- [FRDM-MCXA174](../../../_boards/frdmmcxa174/driver_examples/opamp/opamp_lpadc/example_board_readme.md)
- [FRDM-MCXA344](../../../_boards/frdmmcxa344/driver_examples/opamp/opamp_lpadc/example_board_readme.md)
