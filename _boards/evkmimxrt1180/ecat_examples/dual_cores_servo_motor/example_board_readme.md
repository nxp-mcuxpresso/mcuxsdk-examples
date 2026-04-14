Hardware requirements
=====================
- MIMXRT1180-EVK RevC
- FRDM-MC-LVPMSM
- Teknic2311P Motor
- RJ45 Network cable
- Mini/micro USB cable
- Personal Computer on which the TwinCat3 has been installed

Board settings
==============
- Jumper setting:
  | Jumper | Setting | Jumper | Setting | Jumper | Setting |
	|--------|--------|--------|--------|--------|--------|
	|   JP1  |   1-2  |   J58  |   2-3  |   J78  |   1-2  |
	|   J4   |   1-2  |   J59  |   2-3  |   J79  |   1-2  |
	|   J6   |   1-2  |   J63  |   1-2  |   J90  |   1-2  |
	|   JP6  |   1-2  |   J65  |   1-2  |   J91  |   1-2  |
	|   J9   |   1-2  |   J72  |   1-2  |   J93  |   1-2  |
	|   J11  |   2-3  |   J73  |   1-2  |   J97  |   1-2  |
	|   J12  |   1-2  |   J75  |   2-3  |   J98  |   1-2  |
	|   J14  |   1-2  |   J76  |   1-2  |   J99  |   1-2  |
	|   J57  |   2-3  |   J77  |   1-2  |   J100 |   1-2  |

- Remove and solder zero resistors:  
  - Add zero resistors: R747, R749, R760, R763, R767, R1049, R1051.
  - Remove zero resistors: R104, R105, R107, R126, R142, R146, R148, R1089, R1090.


- For detailed motor connection and motor control user guide, please refer:
  -   http://www.nxp.com/sdkmotorcontrol 

Prepare the Demo
================

1. Connect Teknic2311P motor to the FRDM-LVPMSM-FA shield
2. Connect the FRDM-LVPMSM-FA shield on Motor Control interface of the MIMXRT1180-EVK board.
3. Connect the EtherCAT Port0 on the MIMXRT1180-EVK board with TwinCAT3
	- The label of the EtherCAT Port0 is J28.
4. Connect a USB cable between the host PC and the OpenSDA USB port on the target board

5. Open a serial terminal with the following settings
	- 115200 baud rate
	- 8 data bits
 	- No parity
	- One stop bit
	- No flow control

6. Download the program to the target board

7. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo