# EdgeLock 2GO Blob Test (S)

This is a test suite which imports and validates EL2GO blobs and their usage with PSA.

Workspace structure:
- *tfm_s_crypto_client*: Project creating the static library required by the secure processing environment (S)
- *el2go_blob_test_s*: Project running in the secure processing environment (S)
- *el2go_blob_test_ns*: Project running in the non-secure processing environment (NS)

Details on building and running the application can be found in the
[el2go_blob_test_ns](../el2go_blob_test_ns/readme.md) project.

## Supported Boards
- FRDM-MCXN236
- FRDM-RW612
- MCX-N5XX-EVK
- RD-RW612-BGA
- FRDM-MCXN947
- MCX-N9XX-EVK