# MIMXRT700-EVK MPP Board Support

## Hardware requirements

- MIMXRT700-EVK board (versions A1, A2, A3, B0)
- Mini/micro USB cable
- Personal computer
- RK055AHD091 or RK055MHD091 MIPI DSI display
- OV7670 camera module (FlexIO) or Logitech C920 PRO HD WEBCAM (USB camera)

## Board settings

- Connect external 5V power to J45.
- Connect the MIPI DSI display to J25.
- If using OV7670 FlexIO camera: connect it to J53 (3.3V on pin 1) and move JP7 to 2-3 to enable 3.3V FlexIO.
- If using USB camera (Logitech C920 PRO HD WEBCAM): use a USB OTG adapter to connect it to J40 (USB_OTGO).
  If nothing appears on the screen after flashing, try unplugging and re-plugging the USB cable.

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
   in `display_support.h`. The default camera is FlexIO OV7670. To use the USB camera,
   define `USE_USB_CAMERA` in `reconfig.cmake`.
4. Download the program to the target board using a flasher or debugger utility.
5. Press the reset button (SW2) on the board to begin running the demo
   (CPU reset via MCU-Link/CMSIS may not work on all versions).

## Supported examples

| Example | Notes |
|---------|-------|
| camera_view | Basic camera preview |
| camera_mobilenet_view | MobileNet image classification (TFLite) |
| camera_ultraface_view | UltraFace face detection (TFLite) |
| camera_persondetect_view | FastestDet person detection (TFLite) |
| camera_nanodet_view | NanoDet-m object detection (TFLite) |
| camera_mobilefacenet_view | MobileFaceNet face recognition (TFLite) |
| camera_ultraface_mobilefacenet_view | Face detection + recognition (TFLite) |
| camera_usb_final_fr_app_view | USB camera face recognition app (multicore) |
| static_image_ultraface_view | UltraFace face detection from static image |
| static_image_mobilenet_view | MobileNet classification from static image |
| static_image_persondetect_view | Person detection from static image |
| static_image_nanodet_view | NanoDet object detection from static image |
| static_image_nanodet_persondetect_view | Combined NanoDet + person detection |

## Build example

```bash
# Single core
python3 ./build_mpp.py -b mimxrt700evk -e camera_view

# Multicore (sysbuild)
python3 ./build_mpp.py -b mimxrt700evk -e camera_usb_final_fr_app_view -S
```

## Device support

| Category | Devices |
|----------|---------|
| Camera | OV7670 (FlexIO), USB UVC (Logitech C920 PRO HD WEBCAM) |
| Display | RK055AHD091, RK055MHD091 (MIPI DSI) |
| Graphics | PXP (hardware), VGLite (GPU), CPU (software) |
| Decoder | JPEG SW, JPEG HW |
| Storage | SD card (FatFS) |
| Streaming | RTSP/RTP H.264 over UDP (multicore: core0 + core1) |
