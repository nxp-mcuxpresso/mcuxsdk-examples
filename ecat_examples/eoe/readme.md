# ecat_eoe

## 1. Overview

This example shows how to use EtherCAT EoE (Ethernet over EtherCAT). It extends the
[digital_io](../digital_io/readme.md) example with an lwIP based TCP/IP stack and an HTTP
server that are reachable through the EtherCAT network.

| Item | Value |
|------|-------|
| Base example | ecat_digital_io |
| Mailbox protocol | EoE |
| TCP/IP stack | lwIP + FreeRTOS |
| Application | HTTP server + ping |
| MainDevice used for the demo | TwinCAT 3 |

Default network configuration:

| Node | IP address | Subnet mask | Gateway |
|------|------------|-------------|---------|
| Board (SubDevice) | 192.168.1.66 | 255.255.255.0 | 192.168.1.102 |
| Host PC (MainDevice) | 192.168.1.102 | 255.255.255.0 | 192.168.1.0 |

> **Important:** the project cannot be built as shipped. The SSC source code must be
> generated first - see [2. Build preparation](#2-build-preparation).

## 2. Build preparation

1. Generate the SSC source code: [Generate SSC Code for EoE](../topics/generate_ssc_for_eoe.md)
2. Follow the board specific steps in the board readme listed in [5. Supported boards](#5-supported-boards).
3. Build and flash the example.

## 3. Expected output

```
Start the SSC EoE example...

Hardware init success...

EoE interface init success...

***********************************************************
HTTP Server example
***********************************************************
IPv4 Address     : 192.168.1.66
IPv4 Subnet mask : 255.255.255.0
IPv4 Gateway     : 192.168.1.102
mDNS hostname    : lwip-eoe
***********************************************************
```

Verification from the host PC:

* `ping 192.168.1.66` must succeed.
* `http://192.168.1.66` must open the demo web page (disable the browser proxy first).

> **Note:** to change the HTML content served by the HTTP server, refer to the
> `lwip_examples/lwip_httpsrv` example.

## 4. TwinCAT setup

TwinCAT 3 is used as the EtherCAT MainDevice. Perform the steps in this order:

| Step | Document |
|------|----------|
| 1. Generate SSC code | [Generate SSC Code for EoE](../topics/generate_ssc_for_eoe.md) |
| 2. Install TwinCAT | [TwinCAT Installation](../topics/download_and_install_the_twincat_software.md) |
| 3. Write the ESI file to EEPROM | [TwinCAT EEPROM Update](../topics/twincat_eeprom_update_eoe.md) |
| 4. Create the project and set the EoE addresses | [TwinCAT Project Setup](../topics/twincat_project_setup_eoe.md) |

Additional reading: [Note about the source code in the document](../topics/note_about_the_source_code_in_the_document.md)

## 5. Supported boards

| Board | Board specific readme |
|-------|-----------------------|
| MIMXRT1180-EVK | [example_board_readme.md](../../_boards/evkmimxrt1180/ecat_examples/eoe/example_board_readme.md) |
| FRDM-MIMXRT1186 | [example_board_readme.md](../../_boards/frdmimxrt1186/ecat_examples/eoe/example_board_readme.md) |
| i.MX943-EVK | [example_board_readme.md](../../_boards/imx943evk/ecat_examples/eoe/example_board_readme.md) |
