# v2x_fce_aes_cbc

The v2x_fce_aes_cbc example demonstrates the EdgeLock V2X/FCE (Fast Crypto
Engine) AES-CBC service. It brings up the V2X firmware, opens the FCE service,
loads a plain AES-128 key, encrypts a plaintext block in CBC mode, checks the
ciphertext against the NIST SP 800-38A F.2.1 known answer, then decrypts it back
and confirms it matches the original plaintext.

## Supported Boards
- imx943evk

## Running the demo
When the example runs successfully, the following is printed to the terminal:

```
*************** EdgeLock V2X/FCE AES-CBC demo ***************

****************** Bring up V2X firmware *****************
V2X firmware is running.

****************** Get V2X FW version ********************
V2X FW version: ...

****************** Open V2X/FCE service ******************
Open V2X/FCE service successfully.

****************** Get V2X/FCE info **********************
V2X/FCE info:
...

****************** Load AES plain key ********************
Loaded AES key into slot 0.

****************** AES-128-CBC encrypt *******************
Ciphertext:
7649abac8119b246cee98e9b12e9197d
Ciphertext matches the NIST SP 800-38A KAT vector.

****************** AES-128-CBC decrypt *******************
Decrypted plaintext:
6bc1bee22e409f96e93d7e117393172a
Decrypted plaintext matches the original.

****************** Close V2X/FCE service *****************
Close V2X/FCE service successfully.

*************** EdgeLock V2X/FCE demo END ****************
```
