Hardware requirements
=====================
- FRDM-IMXRT1186 A2
- FRDM-MC-LVPMSM
- Power supply 24V
- Teknic2311P Motor
- RJ45 Network cable
- Type-C USB cable
- Personal Computer on which the TwinCat3 has been installed

Board settings
==============
- Jumper setting:
  | Jumper | Setting |
	|--------|--------|
	|   J12  |   2-3  |
	|   J13  |   2-3  |
	|   J17  |   2-3  |
	|   J18  |   2-3  |
  |   J27  |   2-3  |
  |   J29  |   2-3  |
  |   J30  |   2-3  |
  |   J33  |   2-3  |

- For detailed motor connection and motor control user guide, please refer:
  -   http://www.nxp.com/sdkmotorcontrol 

EEPROM LPI2C Emulator
================
1. FRDM-IMXRT1186 supports EEPROM LPI2C emulator, please skip this chapter if you used EEPROM hardware.
2. Hardware Setting

	- Remove Jumper:

	  J31: pin1 - pin2

	  J35: pin1 - pin2

	- Remove Resistors:

	  R297: pin1 - pin2

	  R299: pin1 - pin2

	- Add Resistors:

	  R30

	  R31

	  R297: pin1 - pin3

	  R299: pin1 - pin3

	- Connection:
  
      J31: pin1  <-->  J2: pin18

      J35: pin1  <-->  J2: pin20  

3. Add EEPROM_I2C_EMULATOR macro 
	- Add EEPROM_I2C_EMULATOR macro to enable eeprom i2c emulator funtion
4. Now you can refer "Prepare the Demo" to setup ecat_examples

Prepare the Demo
================
1. Connect Teknic2311P motor to the FRDM-LVPMSM-FA shield
2. Connect the FRDM-LVPMSM-FA shield on Motor Control interface of the FRDM-IMXRT1186 board.
3. Connect the EtherCAT Port0 on the FRDM-IMXRT1186 board with TwinCAT3
	The label of the EtherCAT Port0 is J57A.
4. Connect a USB cable between the host PC and the OpenSDA USB port on the target board

5. Open a serial terminal with the following settings
	- 115200 baud rate
	- 8 data bits
 	- No parity
	- One stop bit
	- No flow control

6. Download the program to the target board

7. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo
