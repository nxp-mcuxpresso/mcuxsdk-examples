# Introduction

The servo motor example uses TwinCAT software as EtherCAT Main device, FRDM-IMXRT1186 as EtherCAT Subdevice, which supports CiA402 profile to drive the motor.

The EtherCAT Subdevice Controller \(ESC\) integrated on FRDM-IMXRT1186 takes care of the EtherCAT communication as an interface between the EtherCAT fieldbus and subdevice application.

The EtherCAT Subdevice Stack Code \(SSC\) from Beckhoff Automation is a tool to generate the EtherCAT subdevice stack code in ANSI C. Based on this stack, this servo motor example currently supports Cyclic Synchronous Position\( CSP\) and Cyclic Synchronous Velocity \(CSV\) control modes. The Object Dictionary Tool \(OD-Tool\) has been integrated into the SSC OD configuration tool, supporting a consistent definition of the offline and online object dictionary.

