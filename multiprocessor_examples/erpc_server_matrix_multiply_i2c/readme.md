# erpc_server_matrix_multiply_i2c

## Overview
This example demonstrates usage of eRPC between PC and board using I2C transport layer.
Board acts like a server and the PC as client. The libusbsio Python module is used for 
accessing the NXP libusbsio APIs and thus allowing to exercise SPI, I2C bus and GPIO pins 
from Python application (over USBSIO interface of NXP LPCLink2 and MCUlink Pro devices).
Note, that when more than one board with LPCLink2/MCUlink devices
is connected to the PC the Python application is not able to automatically detect which one to use to establish the communication. Thus, please
ensure only one board with running eRPC server application is connected to the PC when starting the Python application on the PC side 
or use the -d command line argument of the Python application to specify the targetted LPCLink2/MCUlink device index. 

When client starts, it generates two random
matrixes and sends them to server. Server then performs matrix multiplication and sends
result data back to client. Result matrix is then printed on the PC side.

## eRPC documentation
eRPC specific files are stored in: middleware\multicore\erpc
eRPC documentation is stored in: middleware\multicore\erpc\doc
eRPC is open-source project stored on github: https://github.com/EmbeddedRPC/erpc
eRPC documentation can be also found in: http://embeddedrpc.github.io

## PC Side Setup (Python)
1. Make sure you have Python installed on your PC
2. Install serial module by executing following command in command line: "python -m pip install pyserial"
3. Install eRPC module to Python by executing setup.py located in: middleware\multicore\erpc\erpc_python - "python setup.py install"
4. Install libusbsio module to Python by executing following command in command line: "python -m pip install libusbsio"

## Usage of run_i2c.py
usage: run_i2c.py [-h] [-b BD] [-d DEVIDX]

eRPC Matrix Multiply example

optional arguments:
  -h, --help                    show this help message and exit
  -b BD, --bd BD                Baud rate (default value is 100000)
  -d DEVIDX, --devidx DEVIDX    LIBUSBSIO device index (default value is 0 - only one board connected to the host PC)

Example:
To run PC side as a client with a board connected as a server:
"run_i2c.py --bd 100000"

## Supported Boards
- [FRDM-MCXN236](../../_boards/frdmmcxn236/multiprocessor_examples/erpc_server_matrix_multiply_i2c/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/multiprocessor_examples/erpc_server_matrix_multiply_i2c/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/multiprocessor_examples/erpc_server_matrix_multiply_i2c/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/multiprocessor_examples/erpc_server_matrix_multiply_i2c/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/multiprocessor_examples/erpc_server_matrix_multiply_i2c/example_board_readme.md)
