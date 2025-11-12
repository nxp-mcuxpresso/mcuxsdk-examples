# Wireless Ranging

This section presents the user interactions and testing methods for the Wireless Ranging application.

## Overview
The Wireless Ranging application is used to perform secure and highly accurate distance estimation (ranging) between two BLE devices.
Wireless ranging application can be used either by using a console or by using a python script environment.

Wireless Ranging application allows to:
- Configure most of the parameters required for a measurement
- Select what type of measurement to be performed
- Trigger CS measurements using "range" or "test" command
- Retrieve CS measurement results for RTP, RTT, and other debug information
- Compute distance estimations from RTT and RTP measurements
- Log system debug information but also raw IQ data information in Matlab (.mat), Excel (.xls) or numpy (.npz) format that can be used for further analysis or development of distance computation algorithms

The application is made of two parts:
- The embedded firmware, that can be controlled manually via a serial connection
- The host application (python) running on a PC and controlling the firmware using serial link

For more information please refer to the _"CS wireless ranging demo application.pdf"_ document.

## Supported Boards
The Wireless Ranging application is supported on the following platform:
- KW47-EVK
- KW47-LOC
- MCX-W72-EVK
- FRDM-MCXW72
