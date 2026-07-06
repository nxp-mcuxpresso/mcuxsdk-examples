# eSPI Promira Test Guide

## Overview

This guide explains how to set up and run eSPI tests using Total Phase Promira platform as the eSPI host/analyzer against the `espi_device` example.

## Prerequisites

### Hardware

- Supported MCU board with eSPI interface
- Promira Serial Platform x1 (Host)
- Promira Serial Platform x1 (Analyzer)
- eSPI cable connecting Promira to MCU (CS#, CLK, IO0-IO3, ALERT#, RESET#). A splitter/adapter is needed to tap the same eSPI signals to both Host and Analyzer Promira devices simultaneously.
- USB cable for MCU debug/UART

### Software Downloads

1. **Promira Software API I2C/SPI Active**
   Download from https://www.totalphase.com/products/promira-software-api-i2cspi/
   Select the package for your platform (Windows/Linux/macOS).

2. **Promira Software API eSPI Analysis**
   Download from https://www.totalphase.com/products/promira-software-api-espi/
   Select the package for your platform.

3. **Python 3.9+** with `pyserial` (`pip install pyserial`)

## Environment Setup

### Step 1: Create working directory

```
mkdir espi_promira_test
cd espi_promira_test
```

### Step 2: Copy Promira SDK files

From the **I2C/SPI Active API** package (`python/` subdirectory), copy:

- `promact_is.dll`
- `promact_is_py.py`
- `promira.dll`
- `promira_py.py`
- `espi_simulator.py`
- `detect.py`

From the **eSPI Analysis API** package (`python/` subdirectory), copy:

- `promana.dll`
- `promana_py.py`

> Note: Both SDK packages ship `promira.dll` and `promira_py.py`. These files are functionally identical (same code, only PE build timestamp differs). Keep the copy from either package.

### Step 3: Copy test scripts

From the example directory, copy:

- `espi_host.py`
- `espi_autotest.py`
- `espi_capture.py`

### Final directory structure

```
espi_promira_test/
├── espi_host.py           // NXP test host script
├── espi_autotest.py       // NXP automated test
├── espi_capture.py        // NXP bus capture/debug tool
├── promira.dll            // Promira platform communication
├── promira_py.py          // Python wrapper for promira.dll
├── promact_is.dll         // I2C/SPI Active application layer
├── promact_is_py.py       // Python wrapper for promact_is.dll
├── espi_simulator.py      // eSPI protocol layer
├── detect.py              // Promira device discovery
├── promana.dll            // eSPI Analysis application layer
└── promana_py.py          // Python wrapper for promana.dll
```

## Getting Started

### Build and flash firmware

Build the `espi_device` example for your board and flash it to the MCU.

### Discover Promira IP addresses

```bash
cd espi_promira_test
python detect.py
```

### Initialize Promira

After connecting, Promira's RESET# pin may hold the MCU in reset. Release it:

```bash
python espi_host.py <HOST_IP> 0 reset
```

Then press the physical reset button on the MCU board.

## Usage

### Automated test

```bash
python espi_autotest.py --host <HOST_IP> --serial <COM_PORT> --analyzer <ANALYZER_IP>
python espi_autotest.py --host <HOST_IP> --serial <COM_PORT>
python espi_autotest.py ... [--io-mode single|dual|quad] [--freq 20|25|33|50|66] [--tests A02,S01,C06]
```

### Bus capture

```bash
python espi_capture.py <ANALYZER_IP> [--io-mode quad] [--filter vw,oob] [--csv FILE] [--raw]
```

### Manual host commands

```bash
python espi_host.py <HOST_IP> <SIM_MODE> <COMMAND> [COMMAND...]
```

- `SIM_MODE`:
  - `0` (transaction): Promira generates the complete eSPI transaction including both command and response phases. The response is pre-programmed by the script, not read from the slave. Useful for testing without a real slave or for analyzer-only scenarios.
  - `1` (master-only): Promira generates the command phase and provides clock for the response phase, but lets the slave drive the actual response. This is the normal mode when testing with a real MCU board.

## Host Command Reference

### Bus Configuration
| Command | Description |
|---------|-------------|
| `setup_channels` | Enable all eSPI channels (Peripheral, VWire, OOB, Flash) |
| `set_bus_config <io_mode> <freq>` | Configure IO mode and frequency (e.g. `set_bus_config single 25`) |

### Virtual Wire
| Command | Description |
|---------|-------------|
| `mcux_sts_get` | Get device status |
| `mcux_vw_put` | Write 8 VWire groups (SLP_Sx, PLTRST#, HOST_RST_WARN, etc.) |
| `mcux_vw_put_p1` | Write VWire batch 1/3: indices 0x02, 0x03, 0x07 |
| `mcux_vw_put_p2` | Write VWire batch 2/3: indices 0x41, 0x42, 0x43 |
| `mcux_vw_put_p3` | Write VWire batch 3/3: indices 0x44, 0x47 |
| `mcux_gpio_vw_put <index> <data>` | Send one GPIO-expander VWire (index 128-255, data = {Valid[7:4], Level[3:0]}) |

### OOB
| Command | Description |
|---------|-------------|
| `mcux_oob_put` | Send 128-byte OOB message |

### Flash (SAF)
| Command | Description |
|---------|-------------|
| `mcux_saf_erase` | Erase 128 bytes at address 0x0500 |
| `mcux_saf_write` | Write 128B at 0x0500 + 128B at 0x0580 |
| `mcux_saf_read` | Read 256 bytes from 0x0500 |
| `mcux_saf_wrd` | Erase + Write + Read |

### Peripheral I/O — Endpoint
| Command | Description |
|---------|-------------|
| `mcux_ep_write_p80` | Write 0x88 to Port 80 |
| `mcux_ep_write_data` | Write 0x56 to endpoint data register |
| `mcux_ep_write_cmd` | Write 0x57 to endpoint command register |
| `mcux_ep_read_data` | Read from endpoint data register |

### Peripheral I/O — Index-Data
| Command | Description |
|---------|-------------|
| `mcux_idx_write_data` | Write data register |
| `mcux_idx_write_cmd` | Write index register (triggers interrupt) |

### Mailbox (Memory)
| Command | Description |
|---------|-------------|
| `mcux_mb_write` | Write 8 bytes to mailbox |
| `mcux_mb_read` | Read from mailbox |
| `mcux_mb_wrd` | Write 256 bytes then read back |

## MCU Console Commands

The firmware provides an interactive console on the UART port:

| Command | Description |
|---------|-------------|
| `status` | Show eSPI status register |
| `send_vw_flag <name> <val>` | Set VWire flag (e.g. `send_vw_flag pmen 1`) |
| `send_vw_mask <hex>` | Set VWire by 32-bit mask |
| `vw_flags` | List available VWire flag names |
| `send_oob <hex bytes>` | Send OOB data (e.g. `send_oob AA BB CC`) |
| `push_irq <num>` | Push IRQ 0-255 to host |
| `reset_p80` | Reset Port 80 counter |
| `help` | Show help |

## Troubleshooting

| Issue | Solution |
|-------|----------|
| No serial output after board reset | Promira pins may not be in working state yet. Send any host command (e.g. `python espi_host.py <IP> 0 reset`) to activate Promira pins, then reset the board again |
| Read operations unstable (result in bus reset) | Connect Analyzer in parallel for improved signal integrity |

## References

- [Promira Serial Platform eSPI Analyzer User Manual](https://www.totalphase.com/support/articles/213706108-promira-serial-platform-espi-analyzer-user-manual/)
- [Monitoring eSPI Messages Using Promira Platform and Data Center](https://www.totalphase.com/support/articles/215954487-monitoring-espi-messages-using-promira-platform-and-data-center/)
