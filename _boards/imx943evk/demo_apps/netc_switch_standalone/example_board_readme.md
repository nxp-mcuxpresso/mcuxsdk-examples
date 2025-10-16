## Hardware requirements

- Micro USB cable
- IMX943-EVK board
- 12V~20V power supply
- Personal Computer

## Board settings

NETC switch performs L2 switching on SGMII5/J30 and SGMII6/J31 ports.
Default mode is 1000M speed and full-duplex, which can be changed in app.h.

#define EXAMPLE_SWT_PORT_SPEED kNETC_MiiSpeed1000M
#define EXAMPLE_SWT_PORT_DUPLEX kNETC_MiiFullDuplex

## Prepare the Demo

1.  Connect 12V~20V power supply to the board.
2.  Connect a micro USB cable between the host PC and the J15(FTDI_DEBUG) USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Program the Linux BSP to SD/eMMC. Then program flash.bin of this demo.
5.  Switch SW1 to power on the board.

Note: the flash.bin is flash_all target with mx94evknetc.cfg System Manager image.
For A core Linux BSP, it should be booted with imx943-evk-netc-rpmsg.dtb.

## Running the demo

When the demo runs, the log would be seen on the terminal like below.

```
NETC Switch standalone demo.
Please suspend others cores for lowest power...
NETC switch will keep L2 switching working.
```

Suspend other cores for lowest power.
For A core, execute `echo mem > /sys/power/state`.
For M7 cores power mode switch demo, enter `d` and `s`.
For M33 SM, execute `idle`.

Waking up other cores can be via PWR_ON/OFF button.
