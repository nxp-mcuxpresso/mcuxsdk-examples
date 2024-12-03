# cmt

## Overview

The cmt example shows the simplest way to use CMT driver.

In this example, the CMT is work as Time mode and used to modulation 11 bit numbers of data.
The CMT is configured to generate a 40000hz carrier generator signal through a modulator gate
configured with different mark/space time period to represent bit 1 and bit 0. 
The modulated data rate is 9600.

Note, The end of cycle interrupt provides a means for the user to reload new mark/space values
into the modulator data registers. Modulator data register updates will take effect at the end 
of the current modulation cycle. The CMT internal down-counter and space period register are
updated at the end of every modulation cycle, irrespective of interrupt handling and the state
of the interrupt. so please make sure the IRQ handler process is shorter than the modulation
cycle to ensure the new mark/space counter is prepared before each end of modulation cycle.
