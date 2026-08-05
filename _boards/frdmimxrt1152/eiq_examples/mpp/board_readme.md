# FRDM-IMXRT1152 MPP Board Support

## Hardware requirements

- FRDM-IMXRT1152 board
- Mini/micro USB cable
- Personal computer
- OV5640 camera module
- RK055AHD091 or RK055MHD091 MIPI DSI display (RK055MHD091 is default)
- External 5V power supply

## Board settings

- Connect the MIPI DSI display to J48.
- Connect the OV5640 camera to J2.
- Connect external 5V power to J43, set J38 to 1-2 (select external power).

## Prepare the Demo

1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Build the project. The project expects the RK055MHD091 panel by default. To use the RK055AHD091 panel,
   change `#define DEMO_PANEL DEMO_PANEL_RK055MHD091` to `#define DEMO_PANEL DEMO_PANEL_RK055AHD091`
   in `display_support.h`.
4. Download the program to the target board using the MCUXpresso IDE or a flash utility.
5. Either press the reset button on your board or launch the debugger to begin running the demo.

## Supported examples

| Example | Notes |
|---------|-------|
| camera_view | Basic camera preview |
| static_image_nanodet_view | NanoDet-m object detection from static image (TFLite) |

## Build example

```bash
python3 ./build_mpp.py -b frdmimxrt1152 -e camera_view
```

## Device support

| Category | Devices |
|----------|---------|
| Camera | OV5640 (MIPI CSI) |
| Display | RK055AHD091, RK055MHD091 (MIPI DSI) |
| Graphics | PXP (hardware), CPU (software) |
| Decoder | JPEG SW, JPEG HW |
