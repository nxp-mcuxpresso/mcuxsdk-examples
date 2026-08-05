# FRDM-IMXRT700 MPP Board Support

## Hardware

- **Board:** FRDM-IMXRT700
- **Device:** MIMXRT798SGVKB (i.MX RT700, dual CM33 + HiFi1 + HiFi4 + eZHV)
- **Debug UART (core0):** LP_FLEXCOMM0 - PIO0_31 (RX), PIO1_0 (TX) - 115200 baud
- **Debug UART (core1):** LP_FLEXCOMM19 - PIO8_14 (RX), PIO8_15 (TX) - 115200 baud

## Hardware requirements

- FRDM-IMXRT700 board
- Mini/micro USB cable
- Personal computer
- OV7670 camera module or USB UVC camera
- RK055MHD091 MIPI DSI display or Raspberry Pi 7" display

## Power

Move jumper **J2** to position **1-2** (5V barrel jack) to supply sufficient power for
both the display and camera. Using USB power alone may cause instability.

## Display

Connect the **Raspberry Pi 7-inch MIPI DSI panel** to connector **J8**.

- **J58:** short (default) — enables DSI signal routing
- **J43:** panel power — pin 1 = 5V, pin 2 = GND

> **Note:** The previously supported RK055 panel is no longer used for this board.

## Camera

### OV7670 (EZH-V DVP)

Connect the **OV7670 camera sensor** to connector **J53** (pins 1–18).

- **JP7:** short pins **2-3**

### USB Camera

A UVC-compatible USB camera can be used via the **J67 USB Type-C** connector.

## PSRAM

The board uses 16-bit PSRAM on XSPI2 port4. No additional hardware setup is required.

## Jumper settings

| Jumper | Position | Description                                      |
|--------|----------|--------------------------------------------------|
| J2     | 1-2      | 5V barrel jack power (required for display/camera) |
| J58    | shorted  | DSI signal routing (default)                     |
| J43    | 1=5V, 2=GND | Raspberry Pi panel power supply              |
| JP7    | 2-3      | OV7670 camera sensor enable                      |

## Prepare the Demo

1. Connect a USB cable between the host PC and the FRDM-IMXRT700 board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Build and flash the project (see Building section below).
4. Press the reset button to begin running the demo.

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
| camera_gesture_recognition_view | Hand gesture recognition (TFLite) |
| camera_usb_final_fr_app_view | USB camera face recognition app (multicore) |
| static_image_mobilenet_view | MobileNet classification from static image |
| static_image_ultraface_view | UltraFace face detection from static image |
| static_image_persondetect_view | Person detection from static image |
| static_image_nanodet_view | NanoDet object detection from static image |
| static_image_mobilefacenet_view | Face recognition from static image (TFLite) |
| static_image_nanodet_persondetect_view | Combined NanoDet + person detection |
| static_image_switch_ultraface_antispoofing_mobilefacenet_view | Face detection + anti-spoofing + recognition (TFLite) |

## Building

Single-core build:

```bash
python3 ./build_mpp.py -b frdmimxrt700 -e camera_view
```

Multicore build (--sysbuild):

```bash
python3 ./build_mpp.py -b frdmimxrt700 -e camera_usb_final_fr_app_view -S
```

Or using west directly:

```bash
west build -b frdmimxrt700/mimxrt798s/cm33_core0 examples/eiq_examples/mpp/camera_view \
    --toolchain armgcc --config flash_release
```

## Device support

| Category | Devices |
|----------|---------|
| Camera | OV7670 (FlexIO), USB UVC |
| Display | Raspberry Pi 7" |
| Graphics | VGLite (GPU), CPU (software) |
| Decoder | JPEG SW, JPEG HW |
| Storage | SD card (FatFS) |
