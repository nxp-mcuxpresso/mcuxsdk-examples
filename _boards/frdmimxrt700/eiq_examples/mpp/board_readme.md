# FRDM-IMXRT700 MPP Board Support

## Hardware

- **Board:** FRDM-IMXRT700
- **Device:** MIMXRT798SGVKB (i.MX RT700, dual CM33 + HiFi1 + HiFi4 + eZHV)
- **Debug UART (core0):** LP_FLEXCOMM0 — PIO0_31 (RX), PIO1_0 (TX) — 115200 baud
- **Debug UART (core1):** LP_FLEXCOMM19 — PIO8_14 (RX), PIO8_15 (TX) — 115200 baud

## Display

Connect the RK055MHD091 MIPI DSI panel to the MIPI connector (J18).

Alternatively, connect the TFT Proto 5" (SSD1963) panel to the FlexIO/LCD-DBI connector.

## Camera

Connect the OV7670 camera module to the FlexIO camera connector (J17).

For USB camera support, connect a UVC-compatible USB camera to the USB-A connector (J15).

## PSRAM

The board uses 16-bit PSRAM on XSPI2 port4. No additional hardware setup required.

## Jumper Settings

- J1: Default position (VDDIO)
- J2: Default position (PMIC enabled)

## Building

```bash
west build -b frdmimxrt700/mimxrt798s/cm33_core0 -- -DEXTRA_CONF_FILE=mpp_config.conf
```

For multicore examples (--sysbuild):
```bash
west build --sysbuild -b frdmimxrt700/mimxrt798s/cm33_core0 -- -DEXTRA_CONF_FILE=mpp_config.conf
```
