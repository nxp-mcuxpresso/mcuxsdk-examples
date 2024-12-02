# EdgeLock 2GO Import Blob

This demo demonstrates how to import the encrypted EdgeLock 2GO secure object blobs from Flash to the
ITS storage. The imported objects are used for executing crypto operations.

The workspace structure (when building this order must be respected):
- el2go_import_blob_s: project running in Secure processing environment (SPE)
- el2go_import_blob_ns: project running in Non-secure processing environment (NSPE)

Details on building and running the application are included in the
[el2go_import_blob_ns readme](../el2go_import_blob_ns/readme.md)

## Hardware requirements

- FRDM-RW612, RD-RW61X-BGA, MCX-N5XX-EVK or MCX-N95XX-EVK board
- USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA, MCX-N5XX-EVK or MCX-N95XX-EVK) cable
- Personal Computer

## Board settings

No special settings are required.

## Supported Boards
