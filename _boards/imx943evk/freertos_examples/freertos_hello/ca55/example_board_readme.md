Hardware requirements
=====================
- Micro USB cable
- IMX943-EVK board
- 12V~20V power supply
- Personal Computer

Board settings
============
No special settings are required.

Run the Demo
===============
1.  Connect 12V~20V power supply, switch SW1(SYS_PWR) to power on the board.
2.  Connect a micro USB cable between the host PC and the J15(FTDI_DEBUG) USB port on the target board.
    There will be 4 serial ports identified when connect USB cable to debug port.
    - The first serial port will be UART8 for FreeRTOS. As there is multiplexing between JTAG and UART8, below bcu configuration is needed to use UART8.
      $ bcu lsftdi
      $ bcu set_gpio fta_jtag_host_en 0 -board=imx943evk19b1 -id=1-1
    - The third serial port will be UART1, which is used by U-Boot and Linux.

3.  Open 2 serial terminals connect to the above 2 ports with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Starting the FreeRTOS Application:
    The Cortex-A Core FreeRTOS application could be booted from the U-Boot command line:
    - For example: using U-Boot "go" command to boot from primary Cortex-A Core:
      u-boot=> load mmc 1:2 0xD0000000 <path-to-image>/freertos_hello_ca55.bin;
      u-boot=> dcache flush;
      u-boot=> go 0xD0000000
    - Or if running the application on secondary Cortex-A Cores, need to use "cpu" command provided by [Real-Time Edge U-Boot](https://github.com/nxp-real-time-edge-sw/real-time-edge-uboot):
      u-boot=> load mmc 1:2 0xD0000000 <path-to-image>/freertos_hello_ca55.bin;
      u-boot=> dcache flush;
      u-boot=> cpu 3 release 0xD0000000

    The Cortex-A Core application could also be booted by Linux remoteproc, more details refer to the section "Unified Life Cycle Management" under "Heterogeneous Multicore Framework" in [Real-time Edge User Guide](https://docs.nxp.com/bundle/REALTIMEEDGEUG/page/topics/Hypervisorless_Virtio/Using_remoteproc_to_boot_acore_from_Linux_command_line.html)
5.  The FreeRTOS console will print out:
    'Hello world.'

#### Note ####
Refer to [Real-Time Edge Software](https://www.nxp.com/rtedge) for getting the Real-Time Edge software.
