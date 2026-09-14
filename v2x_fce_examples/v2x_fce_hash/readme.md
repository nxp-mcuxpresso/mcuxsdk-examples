# v2x_fce_hash

The v2x_fce_hash example demonstrates the EdgeLock V2X/FCE (Fast Crypto Engine)
keyless HASH service. It opens the FCE service, computes a SHA-256 digest over a
message, and checks it against the NIST FIPS 180-4 "abc" known-answer vector.

## Supported Boards
- imx943evk

## Running the demo
When the example runs successfully, the following is printed to the terminal:

```
*************** EdgeLock V2X/FCE HASH demo ***************

****************** Open V2X/FCE service ******************
Open V2X/FCE service successfully.

****************** Get V2X/FCE info **********************
V2X/FCE info:
...

****************** SHA-256 hash generate *****************
Digest:
ba7816bf8f01cfea414140de5dae2223
b00361a396177a9cb410ff61f20015ad
Digest matches the FIPS 180-4 KAT vector.

****************** Close V2X/FCE service *****************
Close V2X/FCE service successfully.

*************** EdgeLock V2X/FCE demo END ****************
```
