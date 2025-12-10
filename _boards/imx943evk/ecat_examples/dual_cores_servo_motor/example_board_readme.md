Hardware requirements
=====================
- IMX943EVK
- FRDM-LVPMSM-FA RevB boards
  Each FRDM-LVPMSM-FA board only could drive one motor. Two boards are needed for two motors demo.
- Servo Motors
  The TGT3-0065-60-48 series motor is used for this demo. There are two modeles:
  - T5PUXS4-H09: integrated ECN 1325 ENDAT2.2 Encoder
  - T5PUXS4-H18: integrated ECN 1325 ENDAT3 Encoder
  One Motor demo:
	- T5PUXS4-H09
    - T5PUXS4-H18
  Two Motors demo:
	- Two T5PUXS4-H09 motors
    - One T5PUXS4-H09 and one T5PUXS4-H18 motor
- RJ45 Network cable
- 24V 5A DC power supply
- Personal Computer on which the TwinCat3 has been installed

Board settings
==============
FRDM-LVPMSM-FA board with T5PUXS4-H09 motor connection:
  - SW90[1:4]: OFF-ON-ON-ON
  - SW30[1:4]: OFF-OFF-ON-OFF
  - J300(1-2)
  - Encoder connection:  
  |Pin name (FRDM-LVPMSM-FA) | Pin location (FRDM-LVPMSM-FA) | Sensor signal name|Pin location (T5PUXS4-H09)|
  |--------------------------|-------------------------------|-------------------|--------------------------|
  | ENC_CLK_P                | J70-2                         | CLOCK             |5                         |
  | ENC_CLK_N                | J70-7                         | CLOCK-            |6                         |
  | ENC_DATA_IO_P            | J70-4                         | DATA              |3                         |
  | ENC_DATA_IO_N            | J70-9                         | DATA-             |4                         |
  | VENC                     | J70-1                         | Vcc(+5v)          |1                         |
  | GND                      | J70-6                         | GND               |2                         |

  - Motor connection:

     J140: pin3  <-->  Cable A: 1=U
     J140: pin2  <-->  Cable A: 3=W
     J140: pin1  <-->  Cable A: 4=V

FRDM-LVPMSM-FA board with T5PUXS4-H18 motor connection:
  - SW90[1:4]: OFF-ON-ON-ON
  - SW30[1:4]: OFF-OFF-ON-OFF
  - J300(1-2)
  - Encoder connection:
  |Pin name (FRDM-LVPMSM-FA) | Pin location (FRDM-LVPMSM-FA) | Sensor signal name|Pin location (T5PUXS4-H18)|
  |--------------------------|-------------------------------|-------------------|--------------------------|
  | ENC_DATA_IO_VENC_P       | J70-5                         | P_SD+             |1                         |
  | ENC_DATA_IO_VENC_N       | J70-10                        | P_SD-             |2                         |

  - Motor connection:

     J140: pin3  <-->  Cable A: 1=U  
     J140: pin2  <-->  Cable A: 3=W  
     J140: pin1  <-->  Cable A: 4=V  

i.MX943-EVK board:
  - SW4: based on used BOOT_MODE
  - SW7[1:4] : ON:OFF:ON:ON
  - SW8: ALL to OFF  
  
Prepare the Demo  
===================
  - Connect the FRDM-LVPMSM-FA shield with the motor on M1 connector on the front of i.MX943-EVK board
  - For two motors demo, connect the second FRDM-LVPMSM-FA shield on M2 connector on the back of i.MX943-EVK board.
  - Select which encoder is used for FRDM-LVPMSM-FA card by the macro definition M1_ENCODER and M2_ENCODER in the mc_periph_init.h file:
    - For M1, "#define M1_ENCODER ENCODER_ENDAT2P2_2" for Endat2.2 encoder or "#define M1_ENCODER ENCODER_ENDAT3" for Endat3 encoder.
    - For M2, "#define M2_ENCODER ENCODER_ENDAT2P2_1" for Endat2.2 encoder or "#define M2_ENCODER ENCODER_ENDAT3" for Endat3 encoder.
    - The default value of these two macros:
      #define M1_ENCODER ENCODER_ENDAT2P2_2
      #define M2_ENCODER ENCODER_ENDAT3

    Note: Only one Endat3 motor is supported on i.MX943-EVK board.

  - Plug the USB cable from the USB host to the FTDI_DEBUG USB connector J15 on the i.MX943-EVK board.
    - Open a serial terminal with the following settings
	  - 115200 baud rate
	  - 8 data bits
 	  - No parity
	  - One stop bit
	  - No flow control
  - Plug the 12-V DC power supply to the P1 connector on the i.MX943-EVK board.
  - Plug the 24-V DC power supply to the J10 connector on the FRDM-LVPMSM-FA board(s). 
  - Connect EtherCAT0 labeled as J24B with TwinCAT3 using RJ45 Network cable.
