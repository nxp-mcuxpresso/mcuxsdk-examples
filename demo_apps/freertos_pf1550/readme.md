# freertos_pf1550

## Overview
The freertos_pf1550 example demonstrates the usage of pf1550 service driver based on FreeRTOS and RPMSG_lite.

This example should use with Linux Kernel running on Application Processor Core which has a virtual PF1550 driver in it.
The example has 3 tasks running concurrent: the first one is user application on Cortex-M4 Core, here we use a hello world
application as a reference. The second one is a PF1550 Local Service task which will receive PF1550 control request from
other task and operate the PF1550 peripheral accordingly. The third one is the PF1550 Remote Service task, this task is
continuous waiting for PF1550 control request raised by Remote Application Processor Core and convert the remote PF1550
control request to PF1550 Local Service API calling and send response once PF1550 remote request is done or failed.
