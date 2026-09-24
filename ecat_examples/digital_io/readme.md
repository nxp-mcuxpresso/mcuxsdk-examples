# ecat_digital_io

## 1. Overview

This example shows how to build an EtherCAT remote I/O SubDevice on top of the Beckhoff
Slave Stack Code (SSC).

| Item | Value |
|------|-------|
| EtherCAT profile | Generic digital I/O |
| Synchronization | SM-Synchron mode only (for DC operation) |
| Process data | 1 output channel + 1 input channel |
| MainDevice used for the demo | TwinCAT 3 |

Channel mapping:

| Channel | Direction | Function |
|---------|-----------|----------|
| Output  | MainDevice -> board | Drives the LED |
| Input   | board -> MainDevice | Reads back the current LED state |

> **Important:** the project cannot be built as shipped. The SSC source code must be
> generated first - see [2. Build preparation](#2-build-preparation).

## 2. Build preparation

1. Generate the SSC source code: [Generate SSC Code for digital_io](../topics/generate_ssc_for_digital_io.md)
2. Follow the board specific steps in the board readme listed in [5. Supported boards](#5-supported-boards).
3. Build and flash the example.

## 3. Expected output

The serial console prints:

```
Start the SSC digital_io example...
```

Once the TwinCAT PLC program is started, the LED blinks at 0.5 Hz.

## 4. TwinCAT setup

TwinCAT 3 is used as the EtherCAT MainDevice. Perform the steps in this order:

| Step | Document |
|------|----------|
| 1. Generate SSC code | [Generate SSC Code for digital_io](../topics/generate_ssc_for_digital_io.md) |
| 2. Install TwinCAT | [TwinCAT Installation](../topics/download_and_install_the_twincat_software.md) |
| 3. Write the ESI file to EEPROM | [TwinCAT EEPROM Update](../topics/twincat_eeprom_update_digital_io.md) |
| 4. Create the PLC project | [TwinCAT Project Setup](../topics/twincat_project_setup_digital_io.md) |

Additional reading: [Note about the source code in the document](../topics/note_about_the_source_code_in_the_document.md)

## 5. Supported boards

| Board | Board specific readme |
|-------|-----------------------|
| MIMXRT1180-EVK | [example_board_readme.md](../../_boards/evkmimxrt1180/ecat_examples/digital_io/example_board_readme.md) |
| FRDM-MIMXRT1186 | [example_board_readme.md](../../_boards/frdmimxrt1186/ecat_examples/digital_io/example_board_readme.md) |
| i.MX943-EVK | [example_board_readme.md](../../_boards/imx943evk/ecat_examples/digital_io/example_board_readme.md) |
