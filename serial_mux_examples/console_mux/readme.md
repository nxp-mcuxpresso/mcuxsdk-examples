# serial_mux_console_mux

Single-UART console multiplexer for imx943evk.

All three cores share ONE physical UART (LPUART1, owned by the cm33_core1 hub)
to carry their consoles as separate COBS logical channels:

| Channel | Core       | Role                              |
|---------|------------|-----------------------------------|
| 0       | cm33_core1 | Hub own console (local encoder)   |
| 1       | cm7_core0  | Client, muxuart HAL UART adapter   |
| 2       | cm7_core1  | Client, muxuart HAL UART adapter   |

The two cm7 clients route `PRINTF` through an ordinary `debug_console_lite` on
the serial_mux HAL UART adapter (`components/uart/fsl_adapter_muxuart.c`),
which pushes console bytes into a serial_mux client uplink ring in shared
memory. The HAL UART "instance" of a client is its mux channel index. The hub polls every uplink ring and its
own channel-0 output, COBS-encodes each frame, and drains them onto LPUART1.

Because the hub concurrently drains the client rings, the clients' blocking
adapter send never deadlocks. The hub itself does not go through a client
(that would deadlock on its own banner); it formats its output locally and
encodes a channel-0 frame straight into the TX ring.

Build one image per core (the role is selected by `MUX_CORE_INDEX`):

```
west build -p always --toolchain armgcc -b imx943evk \
  examples/serial_mux_examples/console_mux -Dcore_id=cm33_core1
```

Use the PC-side tools under `../host_tools/` to split the LPUART1 stream back
into per-channel virtual serial ports. For example, to expose 3 OS-level
virtual serial ports (one per channel):

```
python3 ../host_tools/serial_mux_vports.py --port /dev/ttyUSB0 --channels 3
```


## Host-side split examples

Linux (creates PTY symlinks /tmp/vcom0, /tmp/vcom1, ... you can open):

```
python3 ~/serial_mux/serial_mux_vports.py --port /dev/ttyUSB2 --channels 2 --link-prefix /tmp/vcom
```

Windows (uses the HHD Virtual Serial Port tool to bridge each channel to a
paired COM port, e.g. channel 0 -> COM2, channel 1 -> COM4):

```
python .\serial_mux_vports.py --port COM14 --channels 2 --vsp-tool hhd --win-bridge-ports COM2,COM4
```

## Host-side split examples

The split tool turns the single physical UART carrying the COBS-framed channels
into per-channel OS-level serial ports. Your PC-side application then opens the
per-channel port directly and reads/writes only that core's data.

Linux:

```
python3 ~/serial_mux/serial_mux_vports.py --port /dev/ttyUSB2 --channels 2 --link-prefix /tmp/vcom
```

This creates two virtual serial ports and exposes them as the symlinks
`/tmp/vcom0` (channel 0) and `/tmp/vcom1` (channel 1). Your PC-side program just
opens `/tmp/vcom0` to talk to channel 0 and `/tmp/vcom1` to talk to channel 1.

Windows:

```
python .\serial_mux_vports.py --port COM14 --channels 2 --vsp-tool hhd --win-bridge-ports COM2,COM4
```

On Windows the HHD Virtual Serial Port tool provides linked COM-port pairs.
Here it creates the pairs COM1<->COM2 and COM3<->COM4, and the tool uses COM2
and COM4 (the values passed to `--win-bridge-ports`) as its own bridge/backend
ends. Your PC-side program therefore opens the other end of each pair: open
COM1 to get channel 0 data, and COM3 to get channel 1 data.
