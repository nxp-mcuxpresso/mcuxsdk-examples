# v2x_fce_hmac

The v2x_fce_hmac example demonstrates the EdgeLock V2X/FCE (Fast Crypto Engine)
HMAC service. It opens the FCE service, loads a plain HMAC key, computes an
HMAC-SHA-256 over a message, checks it against the RFC 4231 Test Case 2 known
answer, then verifies the same MAC through the FCE HMAC-verify request.

## Supported Boards
- imx943evk

## Running the demo
When the example runs successfully, the following is printed to the terminal:

```
*************** EdgeLock V2X/FCE HMAC demo ***************

****************** Open V2X/FCE service ******************
Open V2X/FCE service successfully.

****************** Get V2X/FCE info **********************
V2X/FCE info:
...

****************** Load HMAC plain key *******************
Loaded HMAC key into slot 0.

****************** HMAC-SHA-256 generate *****************
HMAC:
5bdcc146bf60754e6a0424260895 75c7
5a003f089d2739839dec58b964ec3843
HMAC matches the RFC 4231 KAT vector.

****************** HMAC-SHA-256 verify *******************
HMAC verify successful.

****************** Close V2X/FCE service *****************
Close V2X/FCE service successfully.

*************** EdgeLock V2X/FCE demo END ****************
```
