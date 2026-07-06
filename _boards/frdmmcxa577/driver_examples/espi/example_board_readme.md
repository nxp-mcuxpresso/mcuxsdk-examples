Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA577 board
- Personal Computer

Board settings
============
EVB rework:
SJ23 2-3, SJ48 1-2, SJ45 1-2, SJ46 1-2, SJ49 1-2, SJ50 1-2.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Interactive eSPI device example.

CLOCK_GetEspiClkFreq = 192000000.
eSPI bus reset.

--- Current eSPI Configuration (0x00289003) ---
  IO Mode: Single SPI
  SPI Speed: <=20 MHz
  CRC Checking: Disabled
  Alert Pin: MISO
  Channels:
    Ch0 (Memory): Enabled
    Ch1 (VWire):  Disabled
    Ch2 (OOB):    Disabled
    Ch3 (Flash):  Disabled
  Max Payload Sizes:
    Memory: 128 bytes
  SAF Support: Yes
  Bus Master: Disabled

Interactive commands:
 show_config               -- Show eSPI configuration
 status                    -- Show eSPI status flags
 send_vw_mask <hexmask>    -- Apply VW by mask (32-bit hex)
 send_vw_flag <name> <val> -- Set VW flag by name (val may be multi-bit)
 vw_flags                  -- List available VW flag names
 send_vw_gpio <i> <v> <l>  -- Send VWire GPIO message(index, value, level)
 send_oob <hexbytes>       -- Send OOB payload (hex, e.g. AA55 or 0xAA 0x55)
 push_irq <num>            -- Push IRQ (0-255) to host
 reset_p80                 -- Reset Port 80 counter
 help                      -- Help

>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
