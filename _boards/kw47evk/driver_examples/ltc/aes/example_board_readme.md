Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
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
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
............................. LTC (AES)  DRIVER  EXAMPLE .............................

Testing input string:
          Once upon a midnight dreary, while I pondered weak and weary,
          Over many a quaint and curious volume of forgotten lore,
          While I nodded, nearly napping, suddenly there came a tapping,
          As of some one gently rapping, rapping at my chamber door
          Its some visitor, I muttered, tapping at my chamber door
          Only this, and nothing more.


----------------------------------- AES-CBC method --------------------------------------
AES CBC Encryption of 320 bytes.
AES CBC encryption finished.

AES CBC Decryption of 320 bytes.
AES CBC decryption finished.
Decrypted string :
          Once upon a midnight dreary, while I pondered weak and weary,
          Over many a quaint and curious volume of forgotten lore,
          While I nodded, nearly napping, suddenly there came a tapping,
          As of some one gently rapping, rapping at my chamber door
          Its some visitor, I muttered, tapping at my chamber door
          Only this, and nothing more.


----------------------------------- AES-CCM method --------------------------------------
AES CCM Encryption of  320 bytes.
    using iv length  : 12 bytes
    using aad length : 20 bytes
    using key length : 16 bytes
    using tag length : 8 bytes
AES CCM encryption finished.

AES CCM decryption of 320 bytes.
AES CCM decryption finished.
Decrypted string:
          Once upon a midnight dreary, while I pondered weak and weary,
          Over many a quaint and curious volume of forgotten lore,
          While I nodded, nearly napping, suddenly there came a tapping,
          As of some one gently rapping, rapping at my chamber door
          Its some visitor, I muttered, tapping at my chamber door
          Only this, and nothing more.


----------------------------------- AES-CMAC ------------------------------------------
AES CMAC Computing hash of 320 bytes
Computed hash:
ba fd 2c 65 a 62 7c 18 a0 df b4 20 bb fb 68 27

.............. THE  END  OF  THE  LTC (AES)  DRIVER  EXAMPLE ................................
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
