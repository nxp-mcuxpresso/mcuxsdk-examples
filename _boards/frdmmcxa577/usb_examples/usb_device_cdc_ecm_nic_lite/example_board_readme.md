# Getting Started

## Hardware requirements

- Type-C USB cable (MCU-Link port, for download)
- Type-C USB cable (USB device port, for the CDC-ECM network adapter)
- RJ45 network cable
- External USB-to-UART module (for the debug console, see the note below)
- FRDM-MCXA577 board
- Personal Computer with a CDC-ECM capable host (Linux-like OS or Mac OS)

## Hardware Settings

FRDM-MCXA577 A0 silicon has an ENET RMII receive issue (ERR053383), so this example
drives the on-board LAN8741 PHY over MII instead of RMII. MII needs more pins than
RMII, so an SCH rev B board requires the following ENET solder-jumper rework before
Ethernet works:

| Solder jumper | Signal       | Setting                      |
|---------------|--------------|------------------------------|
| SJ13          | ENET0_TXD2   | disconnect 1-2, short 2-3    |
| SJ14          | ENET0_TXD3   | disconnect 1-2, short 2-3    |
| SJ16          | ENET0_RXD2   | disconnect 1-2, short 2-3    |
| SJ17          | ENET0_RXD3   | disconnect 1-2, short 2-3    |
| SJ18          | ENET0_COL    | disconnect 1-2, short 2-3    |
| SJ19          | ENET0_CRS    | disconnect 1-2, short 2-3    |
| SJ15          | ENET0_RX_CLK | disconnect both 1-2 and 2-3  |
| SJ44          | ENET0_RXER   | disconnect both 1-2 and 2-3  |

The rework applies to the external 100BASE-TX PHY path only. It is not needed for the
internal 10BASE-T1S digital PHY.

> Note: In this MII configuration the ENET TX bus uses P1_8/P1_9, which are the same
pins as the MCU-Link VCOM debug console (LPUART1). This example therefore routes its
debug console to LPUART2 on the Arduino header (D1/TX = P2_10, D0/RX = P2_11) instead
of the VCOM port. Connect an external USB-to-UART module to those Arduino-header pins
(module RX to D1/P2_10, module TX to D0/P2_11, GND to GND) and open the serial terminal
on that module's COM port. The "USB CDC-ECM NIC Device" banner is printed there, not on
the VCOM port.

## Prepare the example

1.  Rework the board as described above.
2.  Connect an external USB-to-UART module to the Arduino header and open a serial
    terminal on its COM port with 115200 baud rate, 8 data bits, no parity, one stop
    bit and no flow control.
3.  Connect a Type-C USB cable between the host PC and the MCU-Link USB port on the
    target board, then download the program.
4.  Insert an RJ45 network cable into the Ethernet port before running the example.
5.  Connect a Type-C USB cable between the CDC-ECM capable host and the USB device port
    on the target board.
6.  Either press the reset button on your board or launch the debugger in your IDE to
    begin running the demo.
