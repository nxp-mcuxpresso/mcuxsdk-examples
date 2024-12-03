Hardware requirements
=====================
- Mini USB cable
- TWR-MC56F8200 board
- Personal Computer
- Oscilloscope

Board hardware common setting
=============================
1. HW reworkd(workaround):
   > Connect J18(the mini USB port) pin4 and pin5.
     Note: This is due to HW design error, correct setting should be: J18 pin4 left empty and pin5 connected GND.
2. Power setting:
   > Connect J11 pin1 and pin2
   > Connect J6 pin1 and J7 pin2
3. UART setting: 
   > Connect J8 pin3 and pin4
   > Connect J9 pin3 and pin4
4. Onboard OSJTAG debugger setting:
   > Connect J20 pin1 and pin2.
   > Leave J17 open.
     Note: J17 is only used(connected) when firmware update for onboard debugger OSJTAG.
   > Connect J21 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger OSJTAG.
     Note: when use external debugger(connected to J14), leave all J21 pins open

Board settings
==============
- Connect J501-21 to 3.3V with a 10K resistor. Caution: the 10K resistor must be used in series within J501-21 and 3.3V.
- Probe the pwm signal using an oscilloscope
   - At J501-11
   - At J501-9
   - At J501-30
   - At J501-34

  All PWM signals have the same frequency.
  The J501-11/9 PWM signal is a pair of duty cycle constantly changing complementary signals with a fixed dead time.
  The J501-30 duty cycle is half of J501-9, and J501-34 duty cycle is half of J501-30.

- Connect J501-21 to GND with a 10K resistor. Caution: the 10K resistor must be used in series within J501-21 and GND.
  Then J501-9/J501-30/J501-34 PWM signals will be disabled.

Prepare the Demo
================
1.  Connect USB cable between the host PC and the JM60 USB(J18) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~
FlexPWM driver example
~~~~~~~~~~~~~~~~~~~~~~~
