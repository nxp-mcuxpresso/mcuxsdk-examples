# Hardware requirements

- imx943evk board
- USB-C cable
- Personal computer
- The mux-bridge LPUART signals must be routed to a serial-to-USB adapter (or the
  on-board debug UART header) so the PC can observe the muxed stream. By default
  the bridge is LPUART8; when the hub keeps a dedicated console it moves to
  LPUART11 (and LPUART8 stays the hub's plain console).

# Board settings

All three cores share one physical UART: the mux bridge (LPUART8 by default,
LPUART11 when the hub dedicated console is enabled), owned by the cm33_core1 hub.
No jumper changes are required beyond exposing the mux-bridge TX/RX to the host.

# Running the demo

Flash one image per core (cm33_core1 hub, cm7_core0, cm7_core1). On the host,
run the PC-side de-mux tool under `docs/serial_mux/` against the mux-bridge port to
recover three virtual serial ports:

- virtual port 0 <- channel 2 (cm33_core1 hub console)
- virtual port 1 <- channel 0 (cm7_core0 console)
- virtual port 2 <- channel 1 (cm7_core1 console)
