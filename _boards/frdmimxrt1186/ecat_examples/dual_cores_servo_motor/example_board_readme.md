Hardware requirements
=====================
- FRDM-IMXRT1186
- FRDM-LVPMSM-FA RevB
- FRDM-EXP-LVPMSM
- TGT3-0065-60-48 Motor
- RJ45 Network cable
- 24V DC power supply
- Personal Computer on which the TwinCat3 has been installed

Board settings
==============
- J12(2-3)/J13(2-3): eCAT0 Function Via J57A(Default).
- J18(2-3)/J17(2-3): eCAT1 Function Via J57B(Default).

FRDM-LVPMSM-FA board:
  - SW90[1:4]: OFF-ON-ON-ON
  - SW30[1:4]: OFF-ON-OFF-OFF
  - J300(2-3)
  - Encoder connection:
	|Pin name (FRDM-LVPMSM-FA) | Pin location (FRDM-LVPMSM-FA) | Sensor signal name|
	|--------------------------|-------------------------------|-------------------|
	| ENC_CLK_P                | J70-2                         | CLOCK_P           |
	| ENC_CLK_N                | J70-7                         | CLOCK_N           |
	| ENC_DATA_IO_P            | J70-4                         | DATA_P            |
	| ENC_DATA_IO_N            | J70-9                         | DATA_N            |
	| VENC                     | J70-1                         | UP - Power supply voltage|
	| GND                      | J70-6                         | GND               |

  - Motor connection:
  
     J140: pin3  <-->  Cable A: 1=U  
     J140: pin2  <-->  Cable A: 3=W  
     J140: pin1  <-->  Cable A: 4=V  

FRDM-IMXRT1186 board:
  - Jumper setting:

      J29 (2-3)

      J42 (1-2)

      J49 (2-3)

      J30 (2-3)

      J33 (2-3)
      
      J27 (2-3)

  - Add zero resistors:

      R2, R3, R243, R448, R34, R40, R83(1-3)

  - Remove resistors:

      R407
  
Prepare the Demo  
===================
  - Connect the FRDM-EXP-LVPMSM board to the FRDM-IMXRT1186 board
  - Connect the FRDM-LVPMSM-FA with the motor to the Motor1 interface of FRDM-EXP-LVPMSM
  - If demostart two motors, connect the second FRDM-LVPMSM-FA with the motor to the Motor2 interface of FRDM-EXP-LVPMSM board.
  - Plug the USB cable from the USB host to the FTDI_DEBUG USB connector J23 on the FRDM-IMXRT1186 board.
    - Open a serial terminal with the following settings
	  - 115200 baud rate
	  - 8 data bits
 	  - No parity
	  - One stop bit
	  - No flow control
  - Plug the 24-V DC power supply to the J10 connector on the FRDM-LVPMSM-FA board(s). 
  - Connect EtherCAT0 labeled as J57A with TwinCAT3 using RJ45 Network cable.
