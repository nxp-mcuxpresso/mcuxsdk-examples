# fsci_bridge

## Overview
This application imeplements an uart_fsci bridge which conveys the FSCI packets between the serial interface and RPMSG.
It takes the FSCI packets received over UART and sends them to the radio coprocessor over RPMSG. It translates the messages received over RMSG from the radio coprocessor to FSCI events.

It can be used together with the ncp_ble_test_app or the ncp_fsci_black_box examples running on the radio coprocessor.

## Supported Boards
- MCX-W72-EVK
- FRDM-MCXW-72