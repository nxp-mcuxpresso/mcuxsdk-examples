Overview
========
The usb_device_cmsis_validation example demonstrates how to use the NXP CMSIS-Driver USB Device
implementation (Driver_USBD) with the ARM CMSIS-Driver Validation framework (DV_USBD).

The example runs four basic validation tests against the USB Device driver:
- GetCapabilities - reads and checks driver capability flags
- Initialization - Initialize/Uninitialize the driver
- PowerControl - exercises power state transitions
- CheckInvalidInit - verifies that invalid initialization sequences are rejected

Test results are printed to the UART console at 115200 baud. The example uses FreeRTOS as the
underlying RTOS and the CMSIS-RTOS2 adapter.

## Supported Boards
- [FRDM-MCXA577](../../_boards/frdmmcxa577/cmsis_driver_examples/usb_dev/example_board_readme.md)
