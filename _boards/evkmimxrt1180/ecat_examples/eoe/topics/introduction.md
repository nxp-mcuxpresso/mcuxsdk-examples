# Introduction

The Ethernet over EtherCAT \(EoE\) example uses TwinCAT software as EtherCAT Main device, i.MXRT1180 as EtherCAT Subdevice to transfer Ethernet frame by EtherCAT frame.

The EtherCAT Subdevice Controller \(ESC\) integrated on i.MXRT1180 takes care of the EtherCAT communication as an interface between the EtherCAT fieldbus and the subdevice application.

The EtherCAT Subdevice Stack Code \(SSC\) from Beckhoff Automation is a tool to generate the EtherCAT subdevice stack code in ANSI C. The code serves as a development base for the implementation of EtherCAT in devices with an own processor. The Object Dictionary Tool \(OD-Tool\) integrated into the SSC OD configuration tooling supports a consistent definition of both the offline and online object dictionary.

The EoE example features:

-   Ping between EtherCAT main device and EtherCAT subdevice
-   HTTP server

