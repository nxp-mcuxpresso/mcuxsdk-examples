# Introduction

The File Access over EtherCAT \(FoE\) example uses TwinCAT software as EtherCAT Main device, i.MXRT1180 as EtherCAT Subdevice to upgrade firmware.

The EtherCAT Subdevice Controller \(ESC\) integrated on i.MXRT1180 takes care of the EtherCAT communication as an interface between the EtherCAT fieldbus and the subdevice application.

The EtherCAT Subdevice Stack Code\(SSC\) from Beckhoff Automation is a tool to generate the EtherCAT subdevice stack code in ANSI C. The code serves as a development base for the implementation of EtherCAT in devices with its own processor. The Object Dictionary Tool \(OD-Tool\) has been integrated into the SSC OD configuration tools, supporting a consistent definition of the offline and online object dictionary.

FoE example features:

-   Upgrade firmware
-   Download upgraded firmware

