# secure_gpio_s

SECURE VULNERABILITY NOTE:
There is a vulnerability that can affect TrustZone applications that also use floating point unit (CVE-2021-35465). 
The TrustZone examples delivered here do not use floating point, and so they aren't affected. However, 
if TrustZone and FPU are used together, the used compiler should be checked for inbuilt workaround. 
If workaround is not available yet, then the actions described in CVE-2021-35465 should be applied manually.

Actual compilers status for this vulnerability:
MCUX IDE v11.5.0 – fix not available yet
GNU Arm Embedded Toolchain 10.3-2021.07 - fix not available yet
EWARM v9.20.1 – already fixed
ARM MDK v5.35 – check for new Arm compiler v6.17

## Overview
The Secure GPIO demo application demonstrates using of secure GPIO peripheral and GPIO mask feature in
AHB secure controller. The secure GPIO is standard GPIO peripheral supporting 32 pins on port 0 only.
Having two GPIO peripherals allows user to configure one GPIO peripheral into secure world and another
one into normal world. Thus for all pins on port 0, user can select whether the pin is controlled
from secure or non-secure domain (using IOCON).
Since the every GPIO pin allows to read its state regardless of its configuration, the AHB secure
controller has build in feature GPIO mask for all GPIO pins. It is recommended to mask GPIO pins
for all peripherals assigned to secure domain in order to eliminate information leak from GPIO pins.
For example UART is assigned to secure world. But GPIO peripheral still allows to read state of
TxD and RxD pins. This information can be use to decode UART communication from normal world, which
can lead to unwanted information leak. 

This Secure GPIO demo uses one user button (let's call it LED button), which is read from both secure
(using "secure" GPIO) and normal world (using "standard" GPIO), and two LED diodes, one controlled from
secure and second one from normal world. Once LED button is pressed, both secure and non-secure
worlds detect this user action (logical zero is read) and both LEDs are switched ON (first LED is controlled
from secure world and second one from non-secure world). So both LEDs are controlled by LED button.
The second part of the demo demonstrates AHB secure controller GPIO mask feature. This feature is
controlled by second user button (let's call it MASK button). If the MASK button is not pressed, the GPIO
mask feature is disabled and normal world can read the state of LED button. Therefore LED button still
controls both LEDs as described in previous paragraph.
If the MASK button is pressed, the GPIO mask feature is enabled. In this case normal world cannot read
state of state of first button and logical zero is read only. Since logical zero corresponds to pressed LED
button, the LED, controlled from normal world, is immediately switched ON regardless of the state of LED
button. The LED, controlled from secure world, can be still controlled by LED button, because the state of
this button is still available to the secure world via GPIO assigned to secure world. Below is a table
illustrating the individual button and LED states.
For user buttons and LEDs assignment on particular board, see chapter "Running the demo".

| Input        |            | OUTPUT       |              |
|--------------|------------|--------------|--------------|
| ALLOW button | LED button | "secure" LED | "normal" LED |
| PRESSED      | RELEASED   | ON           | ON           |
| RELEASED     | RELEASED   | OFF          | OFF          |
| PRESSED      | PRESSED    | ON           | ON           |
| RELEASED     | PRESSED    | OFF          | ON           |

## Supported Boards
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/trustzone_examples/secure_gpio/secure_gpio_s/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/trustzone_examples/secure_gpio/secure_gpio_s/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/trustzone_examples/secure_gpio/secure_gpio_s/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/trustzone_examples/secure_gpio/secure_gpio_s/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/trustzone_examples/secure_gpio/secure_gpio_s/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/trustzone_examples/secure_gpio/secure_gpio_s/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/trustzone_examples/secure_gpio/secure_gpio_s/example_board_readme.md)
