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
Board needs to rework, please refer to detailed eCAT_Servo_Motor_Example_User_Guide to rework board.
<Add Switch setting and Motor Connection infomation>

Prepare the Demo
================

1. Connect Teknic2311P motor to the FRDM-LVPMSM-FA shield \<How\>
2. Connect the FRDM-LVPMSM-FA shield on Motor Control interface of the MIMXRT1180-EVK board.
3. Connect the EtherCAT Port0 on the MIMXRT1180-EVK board with TwinCAT3
	The label of the EtherCAT Port0 is J28.
4. Connect a USB cable between the host PC and the OpenSDA USB port on the target board

5. Open a serial terminal with the following settings
	- 115200 baud rate
	- 8 data bits
 	- No parity
	- One stop bit
	- No flow control

6. Download the program to the target board

7. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo