# EdgeLock 2GO Certificate Signing Request (CSR)

This sample application demonstrates how to generate Certificate Signing Requests (CSR) and verify X.509 certificates on an MCU device using PSA Crypto APIs. The application is designed to work with NXP's Secure Provisioning SDK (SPSDK) tooling for seamless integration into production workflows.

The application supports two operational modes:
- **CSR Generation Mode**: Generates a new key pair (or uses an existing one) and creates a CSR that can be sent to a Certificate Authority (CA) for signing.
- **Certificate Verification Mode**: Verifies an X.509 certificate received from an external source against the corresponding PSA key stored on the device.

Additional information about EdgeLock 2GO X.509 Certificate Service for MCUs can be found in AN14838 under the following link https://www.nxp.com/webapp/sps/download/license.jsp?colCode=AN14838&appType=file1&DOWNLOAD_ID=null.

## Definitions

- **[GITHUB]**: refers to the case where the MCU SDK is cloned as GitHub Repository and the EdgeLock 2GO application is built using the meta build system.
  More details can be found in https://mcuxpresso.nxp.com/mcuxsdk/latest/html/gsd/repo.html.
- **[ZIP]**: refers to the case where MCU SDK is generated as ZIP package and the EdgeLock 2GO application is imported and built in one of the supported IDEs.
  More details can be found in https://mcuxpresso.nxp.com/mcuxsdk/latest/html/gsd/package.html.

## Prerequisites

- Active [EdgeLock 2GO](https://www.nxp.com/products/security-and-authentication/secure-service-2go-platform/edgelock-2go:EDGELOCK-2GO) account
- NXP Secure Provisioning SDK (SPSDK) installed on the host PC
- Any serial communicator 
  
## Overview

The application flow consists of the following steps:

1. Platform and PSA Crypto initialization
2. Parse the configuration block (TLV format) provided via SPSDK in a defined memory location
3. Verify configuration block integrity
4. Execute the requested operation:
   - **CSR Generation**: Generate/retrieve PSA key, create CSR, write to memory
   - **Certificate Verification**: Read certificate from memory, verify against PSA key
5. Write operation status code to memory for SPSDK retrieval

## Preparing the application

1. **[OPTIONAL]** Configure the target OS:

   The example application supports currently only baremetal execution, which is also per default selected. However, to change the target OS, modify the CMake cache variable:

   ```bash
   -DEL2GO_CSR_OS=baremetal
   ```

2. **[OPTIONAL]** Configure logging level:

   Adjust the logging verbosity by modifying the log level in the application configuration in:
   
   [middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/pal/inc/el2go_csr_console.h](../../../../middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/pal/inc/el2go_csr_console.h)
   
   ```c
   #ifndef CSR_LOG_LEVEL
   #define CSR_LOG_LEVEL LOG_INFO
   #endif
   ```
   or modify the CMake cache variable `CSR_LOG_LEVEL` during build:
   
   ```bash
   -DCSR_LOG_LEVEL=LOG_DEBUG
   ```

   Available levels are:
   - `LOG_ERROR`: Only error messages
   - `LOG_WARNING`: Warning and error messages
   - `LOG_INFO`: Informational messages, warnings, and errors (default)
   - `LOG_DEBUG`: Debug messages, info, warnings, and errors
   - `LOG_TRACE`: All messages including verbose trace information

3. Build the application:

   - **[GITHUB]** Compile the *el2go_csr* project with your desired toolchain using `west build`
   - **[ZIP]** Import and compile the *el2go_csr* project in your IDE.

4. Connect the USB cable to the PC host and the debug USB port on the board.

5. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control

6. Download the application to the target board:

   - **[GITHUB]** Use `west flash` to download the image.
   - **[ZIP]** Use the IDE's download/debug functionality.

## Running the application

The application is designed to be controlled by SPSDK tooling. The configuration data is passed to the application through the memory region (`el2go_csr_conf_data`), and the operation status is returned via the `el2go_spsdk_status` variable, which is mapped to a specific memory address for SPSDK to retrieve.

### CSR Generation Mode

When the configuration block specifies CSR generation, the application will:

1. Parse the key ID and operation type from the configuration
2. Generate a new PSA key or use an existing one (based on the TLV field `device_operation`)
3. Generate a CSR using the PSA key
4. Write the CSR to the specified destination memory address

Example log output (with `CSR_LOG_LEVEL` set to `LOG_TRACE`):

```
[INFO] ########### EdgeLock2GO Certificate Signing Request Application ###########
[DEBUG] CSR has been generated! Writing to memory now...
[DEBUG] Generated CSR:-----BEGIN CERTIFICATE REQUEST-----
MIIBIjCByQIBADBnMQswCQYDVQQGEwJVUzELMAkGA1UECAwCQ0ExEjAQBgNVBAcM
...
-----END CERTIFICATE REQUEST-----
[TRACE] Returning to main function from generate_cert_sign_req subroutine.
[INFO] CSR generation completed successfully!
[DEBUG] Returning status code of operation
[INFO] ########### EdgeLock2GO Certificate Signing Request App. EXIT ###########
```

Example log output (with default `CSR_LOG_LEVEL` set to `LOG_INFO`):

```
[INFO] ########### EdgeLock2GO Certificate Signing Request Application ###########
[INFO] CSR generation completed successfully!
[INFO] ########### EdgeLock2GO Certificate Signing Request App. EXIT ###########
```

### X.509 Certificate Storage

When the configuration block specifies certificate storage/verification, the application will:

1. Parse the key ID and certificate source address from the configuration
2. Read the X.509 certificate from the configuration block specified memory location
3. Verify the certificate's public key matches the PSA key by generating signature using private key and verifying it using public key inserted in certificate
4. Return the verification status

Example log output (with default `CSR_LOG_LEVEL` set to `LOG_INFO`):

```
[INFO] ########### EdgeLock2GO Certificate Signing Request Application ###########
[INFO] Certificate verification and storage completed successfully!
[INFO] ########### EdgeLock2GO Certificate Signing Request App. EXIT ###########
```

### Status Codes

The application returns status codes to SPSDK via the `el2go_spsdk_status` memory location:

| Status Code | Description |
|-------------|-------------|
| `SPSDK_STATUS_CODE_SUCCESS` | Operation completed successfully |
| `PSA_ERROR_INVALID_ARGUMENT` | Invalid configuration or context provided |
| `PSA_ERROR_INSUFFICIENT_MEMORY` | Memory allocation failed |
| `PSA_ERROR_STORAGE_FAILURE` | Memory read/write operation failed |
| `PSA_ERROR_INVALID_SIGNATURE` | Certificate verification failed |
| `PSA_ERROR_ALREADY_EXISTS` | Key already exists (non-fatal for CSR generation) |

## Integration with SPSDK

The application is designed to work with NXP's Secure Provisioning SDK (SPSDK). The typical workflow is:

1. SPSDK prepares the configuration block in TLV format
2. SPSDK loads the configuration to the device memory
3. SPSDK triggers the application execution
4. Application processes the request and writes results to memory
5. SPSDK reads the CSR or status from device memory

For detailed SPSDK integration instructions, refer to the SPSDK documentation

## Configuration Block Format

The configuration block uses a TLV (Type-Length-Value) format with BER-encoded length fields and integrity verification. The application supports two distinct configuration block types, identified by their magic number

### Common Fields

Both configuration blocks share the following fields:

| Field | CSR Generation Tag | Certificate Storage Tag | Description |
|-------|-------------------|------------------------|-------------|
| Magic | `CSR_GEN_TAG_MAGIC` | `CERT_STORAGE_TAG_MAGIC` | Magic value identifying the operation mode |
| Version | `CSR_GEN_TAG_VERSION` | `CERT_STORAGE_TAG_VERSION` | Configuration block version number |
| Device Operation | `CSR_GEN_TAG_DEVICE_OPERATION` | `CERT_STORAGE_TAG_DEVICE_OPERATION` | Operation type (e.g., generate new key or use existing) |
| Key ID | `CSR_GEN_TAG_KEY_ID` | `CERT_STORAGE_TAG_KEY_ID` | PSA key identifier |
| Integrity Algorithm | `CSR_GEN_TAG_INTEGRITY_ALGORITHM` | `CERT_STORAGE_TAG_INTEGRITY_ALGORITHM` | Algorithm for integrity verification |
| Integrity Value | `CSR_GEN_TAG_INTEGRITY_VALUE` | `CERT_STORAGE_TAG_INTEGRITY_VALUE` | Checksum over all preceding fields |

### Mode-Specific Fields

| Mode | Field | Tag | Description |
|------|-------|-----|-------------|
| CSR Generation | Destination Address | `CSR_GEN_TAG_CSR_DEST_ADDR` | Memory address where the generated CSR will be written |
| Certificate Storage | Source Address | `CERT_STORAGE_TAG_CERT_SRC_ADDR` | Memory address where the X.509 certificate is located |
| Certificate Storage | Source Size | `CERT_STORAGE_TAG_CERT_SRC_ADDR_SIZE` | Size of the X.509 certificate in bytes |

### Integrity Verification

The configuration block integrity is verified using the algorithm specified in the `Integrity Algorithm` field. Currently supported algorithms:

| Algorithm ID | Algorithm | Output Size |
|--------------|-----------|-------------|
| `1` | CRC32 | 4 bytes |


## Supported Boards
- [FRDM-MCXE31B](../../_boards/frdmmcxe31b/el2go_examples/el2go_csr/el2go_csr/example_board_readme.md)

## Troubleshooting

1. **"Initialization of crypto HW failed!"**
   - Ensure PSA Crypto is properly configured for your platform
   - Check that the secure boot configuration is correct

2. **"Failed to parse configuration block data!"**
   - Verify the configuration block format matches expected TLV structure
   - Ensure SPSDK is providing valid configuration data

3. **"Configuration data integrity verification failed!"**
   - The CRC32 checksum doesn't match - configuration data may be corrupted
   - Regenerate the configuration block using SPSDK

4. **"Certificate verification failed!"**
   - The certificate's public key doesn't match the PSA key
   - Verify the correct key ID is specified in the configuration

5. **"Writing generated CSR to memory failed!"**
   - Check that the destination address is valid and writable
   - Ensure sufficient memory is available at the target location

6. **"Invalid context for CSR generation provided!"**
   - The configuration block was not properly parsed
   - Verify the TLV structure and magic number are correct

7. **"x.509 certificate size exceeds maximum allowed size!"**
   - The certificate size exceeds `MAX_X509_CERT_SIZE`
   - Check the certificate being provided or increase the maximum size limit

8. **"Memory allocation for x.509 certificate verification failed!"**
   - Insufficient heap memory available
   - Increase heap size in the linker configuration or reduce certificate size