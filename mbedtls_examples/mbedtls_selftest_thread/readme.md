# mbedtls_selftest_thread

## Overview
Purpose of this demo is to test valid multithread support for mbedTLS Hardware implementation. For that, two tasks performs in parallel cryptographic 
algorithm testing.

## Prepare the Demo
Connect a serial cable from the debug UART port of the target board to the PC. Start TeraTerm
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start TeraTerm.

2. New connection -> Serial.

3. Set the appropriate COMx port (where x is port number) in port context menu. The number is provided by the operating
   system, and could be different from computer to computer. Select the COM number related to the virtual
   serial port. Confirm the selected port by clicking the "OK" button.

4. Set following connection parameters in menu Setup -> Serial port.
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: none

5.  Confirm the selected parameters by clicking the "OK" button.

## Running the demo
When the demo runs successfully, the terminal will display similar information like the following:

mbedTLS version 2.28.1
fsys=260000000
Using following implementations:
  SHA: ELS PKC HW accelerated
  AES: ELS PKC HW accelerated
  AES GCM: ELS PKC HW accelerated
  DES: Software implementation
  Asymmetric cryptography: ELS PKC HW accelerated
Create thread successfully
Create thread successfully
  Task 1 is running

  Task 2 is running

  Task 1 is running

  Task 2 is running

  Task 1 is running

  Task 1 is running

  Task 2 is running

  Task 1 is running

  Task 2 is running

  Task 1 is running

  Task 1 is running

  Task 2 is running

  Task 1 is running

  Task 2 is running

  Task 1 is running

  Task 1 is running

  Task 2 is running

  Task 1 is running

  Task 2 is running

  Task 1 is running

  Task 1 is running

  Task 2 is running

  Task 1 is running

## Supported Boards
- [FRDM-RW612](../../_boards/frdmrw612/mbedtls_examples/mbedtls_selftest_thread/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/mbedtls_examples/mbedtls_selftest_thread/example_board_readme.md)
