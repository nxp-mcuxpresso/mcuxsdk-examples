# SWT

## Overview

The SWT Example project is to demonstrate usage of the KSDK swt driver.
In this example, SWT will be refreshed by fixed service and keyed service sequence in regular window.
After this, it will refresh SWT with fixed service sequence in window mode.

## Running the demo

When the example runs successfully, you can see the similar information from the terminal as below.

=== Software watchdog timer example start ===

---     SWT regular(non window) mode      ---
Refresh SWT with fixed service sequence:
SWT timeout interrupt triggered 0!
SWT timeout interrupt triggered 1!
SWT timeout interrupt triggered 2!

Refresh SWT with keyed service sequence:
SWT timeout interrupt triggered 0!
SWT timeout interrupt triggered 1!
SWT timeout interrupt triggered 2!

---            SWT window mode            ---
SWT refresh with fixed service sequence in window mode done!
===  Software watchdog timer example end  ===

## Supported Boards
- FRDM-MCXE31B
