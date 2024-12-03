# mcx_secure_gpio_s

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
The Secure GPIO demo application demonstrates using of GPIO peripheral and GPIO Access protection.
By configuring access protection registers (PCNS and PCNP) for each pin you can choose which pin can be used from non-secure world

This Secure GPIO demo uses one user button (let's call it ALLOW button), which is read from secure world and
one user button (let's call it LED button) which is read from non-secure world, and two LED diodes,
one controlled from secure and second one from normal world. Once ALLOW button is pressed, secure world detect this user action (logical zero is read)
and BLUE LED are light up and configure  PCNS register for LED button and red LED so it can be used in non-secure world. 

When LED button is pressed alone, nothing will happen as access to GPIO pins from non-secure world is not allowed.
So to be able light up non-secure led, firstly you need to press ALLOW button and then LED button.

For user buttons and LEDs assignment on particular board, see chapter "Running the demo".

| Input        |            | OUTPUT       |              |
|--------------|------------|--------------|--------------|
| ALLOW button | LED button | "secure" LED | "normal" LED |
| PRESSED      | RELEASED   | ON           | OFF          |
| RELEASED     | RELEASED   | OFF          | OFF          |
| PRESSED      | PRESSED    | ON           | ON           |
| RELEASED     | PRESSED    | OFF          | OFF          |

## Supported Boards
- [FRDM-MCXN236](../../_boards/frdmmcxn236/trustzone_examples/secure_gpio/secure_gpio_s/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/trustzone_examples/secure_gpio/secure_gpio_s/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/trustzone_examples/secure_gpio/secure_gpio_s/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/trustzone_examples/secure_gpio/secure_gpio_s/example_board_readme.md)
