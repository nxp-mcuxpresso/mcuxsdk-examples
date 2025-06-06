# memfault

## Overview

This example demonstrates how to use Memfault SDK to collect fault data.
The dumped data is saved to flash, and can be sent to Memfault cloud for analysis.
For more dtails, please refer to: https://memfault.com/

**NOTE** Only flash targets are supported!

This project only dumps some necessary memory regions, if you want to dump more,
please refer to the memfault document: https://docs.memfault.com/docs/mcu/coredumps#choosing-what-to-store-in-a-coredump,
and create your own function `memfault_platform_coredump_get_regions` to override
the default one which is a weak function in file memfault_platform_port.c.

## Configuration

A flash region is reserved for memfault dumped data, this flash region position
and size can be configured.

When use Kconfig, go to the "Component Configuration -> Debug -> MemFault SDK",
configure the "Backend flash offset" and "Backend flash size".
When use the package without Kconfig, modify the macros `CONFIG_MEMFAULT_FLASH_BASE_OFFSET`
and `CONFIG_MEMFAULT_FLASH_SIZE` in file "mcux\_config.h".

**NOTE**
When the flash region is reconfigured, the linker script should be updated too, to
make sure the flash region match.

## Running the project

Follow the instructions in the terminal, press 'A' to trigger a fault. When the fault
occurs, MCU will dump the data to flash reset. After reset, press 'B' to show the
dumped data and send the data to Memfault cloud to check.

After 'B' is pressed, the dumped data is cleared from flash, so if 'B' is pressed
again, no data will be shown.

Option 'C' is self test, if run pass, `Test successed` will be printed.
**Note** If run 'A' then 'C', the test will fail, because there is valid dump data
in flash, selftest can't erase the dump data.

## Analyze the dumped result

There are two methods to analyze the dumped result: Upload dumped data chunk to Memfault
server, or debug use GDB.

### Upload data chunk to Memfault server

When 'B' is pressed, the data chunks are shown in the terminal, post the data following
such guide: https://docs.memfault.com/docs/mcu/export-chunks-over-console

### Debug with GDB

1. Install the python tool using command `pip install memfault-core-convert`. Details
   of this tool is: https://pypi.org/project/memfault-core-convert/
2. Export the dumped data in flash, for example, using gdb command like,
   `dump binary memory memfault-core.bin 0x00010000 0x00018000`, fill the right flash
   address according to your settings. An easy way is dump all memfault flash region,
   the region base address is: `CONFIG_FLASH_BASE_ADDRESS`+`CONFIG_MEMFAULT_FLASH_BASE_OFFSET`,
   the regioni size is: `CONFIG_MEMFAULT_FLASH_SIZE`. These values can be got
   from the file "mcux\_config.h".
3. Generate the core elf file use `memfault-core-convert --file memfault-core.bin --out_elf core.elf`
4. Run GDB to debug: `arm-none-eabi-gdb -se memfault.elf --core core.elf`, here `memfault.elf`
   is current project image.

Detailed steps can be found here: https://docs.memfault.com/docs/mcu/coredump-elf-with-gdb.

**NOTE** Don't run step 2 after pressing 'B', because the flash will be cleaned when press 'B'.

## Supported Boards
- [FRDM-RW612](../../_boards/frdmrw612/component_examples/memfault/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/component_examples/memfault/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/component_examples/memfault/example_board_readme.md)
