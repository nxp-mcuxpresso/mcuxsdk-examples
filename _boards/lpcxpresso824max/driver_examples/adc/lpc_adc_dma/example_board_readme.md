# Functional Description
1.This example demonstrates how to configure the A sequences with interrupt, assigning one channel with software
  trigger, you can configure channel via "DEMO_ADC_SAMPLE_CHANNEL_NUMBER".
  
2.Before configuration of the ADC begins, the ADC is put through a self-calibration cycle.  

3.Configure the DMA and DMAMUX to work with ADC sequences.

4.Enable the Conversion-Complete or Sequence-Complete DMA for sequences A.
  
5.After ADC channels are assigned to each of the sequences, if the user enters any key via terminal, software trigger will start.  
  
6.When the conversion completes, the DMA would be requested.

7.When the DMA transfer completes, DMA will trigger a interrupt. ISR would set the "bDmaTransferDone" to 'true'. Then main function will 
  print conversion result to terminal.

# Hardware Requirements
- Micro USB cable
- LPCXpresso824MAX board
- Personal Computer

# Board Settings
- ADC CH1 input signal J5-1(PIO0-6).

# Run the Project
Run this example by performing the following steps:

1. Connect a micro USB cable between the PC host and the CMSIS DAP port(J3 on the 
   LPCXpresso824MAX board) for receiving debug information.

2. Open a serial terminal in PC(for example PUTTY) with the following settings:
   - 9600 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control

3. Compile and download the program to the target board.
   More information about how to compile and program the project can refer to 

   [Getting Started with MCUXpresso SDK](../../../../../docs/Getting Started with MCUXpresso SDK.pdf).

4. Monitor the information on the debug console.

# Expected Result
Press any key and print user channel's result in serial terminal.
