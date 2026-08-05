# FRDM-MCXN947 MPP Board Support

## Hardware requirements

- FRDM-MCXN947 board
- Mini/micro USB cable
- Personal computer
- OV7670 camera module
- NXP LCD-PAR-S035 display 320x480 (default) or MikroElektronika TFT Proto 5" display 800x480

## Board settings

In order to route the camera data signals to the chip, some Solder Jumpers (SJ) on the back of
the board must be changed: move SJ16, SJ26, SJ27 from A side to B side, otherwise the captured
image will appear reddish.

- Connect the OV7670 module to the camera pins (match the 3V3 pins).
- Connect the display to the FlexIO LCD pins (panel pins from GND up to D15).

## Prepare the Demo

1. Connect a USB cable between the host PC and the FRDM-MCXN947 board.
2. Debug Console is available with a TTY device over USB MCU-Link.
3. For console over UART: open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
4. Build the project using the MCUX IDE (axf image) or armgcc (elf image).
5. Download the program to the target board using the MCUXpresso IDE.
6. Either press the reset button on your board or launch the debugger to begin running the demo.

## Supported examples

| Example | Notes |
|---------|-------|
| camera_view | Basic camera preview |
| camera_mobilenet_view | MobileNet image classification (TFLite) |
| camera_ultraface_view | UltraFace face detection (TFLite) |
| camera_persondetect_view | FastestDet person detection (TFLite) |

## Build example

```bash
python3 ./build_mpp.py -b frdmmcxn947 -e camera_view
```

## Device support

| Category | Devices |
|----------|---------|
| Camera | OV7670 (FlexIO) |
| Display | NXP LCD-PAR-S035 (ST7796S, 8080 parallel), Mikroe TFT Proto 5" (SSD1963) |
| Graphics | CPU (software) |
| Decoder | JPEG SW |
