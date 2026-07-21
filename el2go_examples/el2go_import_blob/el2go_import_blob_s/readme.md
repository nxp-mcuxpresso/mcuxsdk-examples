# EdgeLock 2GO Import Blob (S)

This sample application shows how to import encrypted EdgeLock 2GO secure object blobs from flash to the ITS storage. The imported objects can then be validated by executing crypto operations.

Workspace structure:
- *tfm_s_crypto_client*: Project creating the static library required by the secure processing environment (S)
- *el2go_import_blob_s*: Project running in the secure processing environment (S)
- *el2go_import_blob_ns*: Project running in the non-secure processing environment (NS)

Details on building and running the application can be found in the
[el2go_import_blob_ns](../el2go_import_blob_ns/readme.md) project.

## Supported Boards
- FRDM-RW612
- MCX-N5XX-EVK
- RD-RW612-BGA
- FRDM-MCXN947
- MCX-N9XX-EVK
