# Introduction

The digital input output \(IO\) example is used to demonstrate the steps to develop an Ethernet for Control Automation Technology \(EtherCAT\) subdevice based on Subdevice Stack Code \(SSC\) and ECAT peripheral integrated on i.MXRT1180.

The EtherCAT SSC from Beckhoff Automation is an example source code in ANSI C. The code serves as a development base for the implementation of EtherCAT in the devices with their own processor. The Object Dictionary Tool \(OD-Tool\) has been integrated into the SSC OD configuration tooling, supporting a consistent definition of the offline and the online object dictionary.

The ECAT controller integrated on i.MXRT1180 takes care of the EtherCAT communication as an interface between the EtherCAT fieldbus and the sub application.

This digital IO example only supports SM-Synchron mode for DC operation, and implements one input channel and one output channel:

-   Output channel: connected with the D6 LED on the MIMXRT1180-EVK board and used to control this LED.
-   Input channel: used to read the D6 LED status.

