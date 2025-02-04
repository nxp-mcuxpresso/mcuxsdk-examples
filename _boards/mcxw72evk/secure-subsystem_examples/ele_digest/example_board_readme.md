Hardware requirements
=====================
- Type-C USB cable
- MCX-W72-EVK Board
- Personal Computer

Board settings
============

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J14.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE Digest via SSSAPI Example

**** SHA256 One-Go ****
Init digest context...OK
Generate digest with one-go operation...OK
Check if digest matches expected value...OK
Free digest context...OK

**** SHA1/256/384/512 Multi-Part with Context Switching ****
Init contexts for all four digests...OK

Init SHA1...OK
Init SHA256...OK
Init SHA384...OK
Init SHA512...OK

Update SHA1   with first 176 bytes...OK
Update SHA256 with first 176 bytes...OK
Update SHA384 with first 176 bytes...OK
Update SHA512 with first 176 bytes...OK

Update SHA512 with remaining 176 bytes...OK
Update SHA384 with remaining 176 bytes...OK
Update SHA256 with remaining 176 bytes...OK
Update SHA1   with remaining 176 bytes...OK

Finish SHA1...OK
Check SHA1 digest...OK
Finish SHA256...OK
Check SHA256 digest...OK
Finish SHA384...OK
Check SHA384 digest...OK
Finish SHA512...OK
Check SHA512 digest...OK

Free contexts of all four digests...OK

**** Export and Import digest context ****
Init context OK
Init SHA256...OK
Update SHA256 with first 176 bytes...OK
Export digest context OK
Import digest context OK
Update SHA256 with remaining 176 bytes...OK
Finish SHA256...OK
Check SHA256 digest...OK

**** Opaque and transparent One-Go CMAC / HMAC  ****
Init CMAC key object...OK
Init HMAC key object...OK
Allocate CMAC key handle...OK
Allocate HMAC key handle...OK
Initialize RNG for opaque key generation...OK

Set transparent CMAC key...OK
Set transparent HMAC key...OK
Init CMAC context...OK
Init HMAC context...OK
Generate CMAC keyed digest...OK
Generate HMAC keyed digest...OK
Check if CMAC correct...OK
Check if HMAC correct...OK

Generate opaque CMAC key...OK
Generate opaque HMAC key...OK
Init CMAC context...OK
Init HMAC context...OK
Generate CMAC keyed digest...OK
Generate HMAC keyed digest...OK

Free contexts and key objects...OK

End of Example with SUCCESS!!

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~