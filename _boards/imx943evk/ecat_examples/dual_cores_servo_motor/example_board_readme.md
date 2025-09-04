Hardware requirements
=====================
- IMX943EVK
- FRDM-LVPMSM-FA RevB
- TGT3-0065-60-48 Motor
- RJ45 Network cable
- 24V 5V DC power supply 
- Personal Computer on which the TwinCat3 has been installed

Board settings
==============
FRDM-LVPMSM-FA board:
  - SW90[1:4]: OFF-ON-OFF-ON
  - SW30[1:4]: OFF-ON-OFF-OFF
  - J300(1-2)
  - Encoder connection:  
	|Pin name (FRDM-LVPMSM-FA) | Pin location (FRDM-LVPMSM-FA) | Sensor signal name|
	|--------------------------|-------------------------------|-------------------|
	| ENC_CLK_P                | J70-2                         | CLOCK_P           |
    | ENC_CLK_N                | J70-7                         | CLOCK_N           |
    | ENC_DATA_IO_P            | J70-4                         | DATA_P            |
    | ENC_DATA_IO_N            | J70-9                         |            DATA_N |
    | VENC                     | J70-1                         | UP - Power supply voltage|
    | GND                      | J70-6                         | GND               |
  - Motor connection:
     (J140) <todo>

i.MX943-EVK board:
  - SW4: based on used BOOT_MODE
  - SW7[1:4] : ON:OFF:ON:ON
  - SW8: ALL to OFF  
  
Prepare the Demo  
===================
  - Connect the FRDM-LVPMSM-FA shield with the motor on Motor Control 1 of the i.MX943-EVK board 
  - If demostart two motors, connect the second FRDM-LVPMSM-FA shield on Motor Control 2 connector of the i.MX943-EVK board.
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
