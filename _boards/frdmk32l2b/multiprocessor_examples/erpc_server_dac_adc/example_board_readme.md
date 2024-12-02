Hardware requirements
=====================
- Mini USB cable
- FRDM-K32L2B board
- Personal Computer

Board settings
==============
Connect J4-2(PTB0/ADC0_SE8) to J4-11(PTE30/DAC0_OUT)


Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Download the program to the target board.
3.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
4.  Run "run.py" on your PC and select COM port.

For detailed instructions, see the appropriate board User's Guide.

Running the demo
================
Selected UART port: COM12, 115200 bd
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
->Press '1' for DAC ADC conversion
->Press '2' for GPIO LED
->Press '3' for Accelerometer
1
Please enter voltage <0V - 3.3V>: 1.5
You entered 1.5
Read value from ADC: 1.498535
---------------------
->Press '1' for DAC ADC conversion
->Press '2' for GPIO LED
->Press '3' for Accelerometer
2
->Press '1' for red LED
->Press '2' for green LED
3
->Press '1' for DAC ADC conversion
->Press '2' for GPIO LED
->Press '3' for Accelerometer
3
Roll the board on all orientations to get max and min values
Read value from Accelerometer:
    Accelerometer X axis: -92.000000
    Accelerometer Y axis: -64.000000
    Accelerometer Z axis: 8144.000000
->Press '1' for DAC ADC conversion
->Press '2' for GPIO LED
->Press '3' for Accelerometer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
