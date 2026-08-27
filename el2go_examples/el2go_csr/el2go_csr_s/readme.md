# EdgeLock 2GO Certificate Signing Request (CSR) - S

This sample application demonstrates how to generate Certificate Signing Requests (CSR) and verify X.509 certificates on an MCU device using PSA Crypto APIs in a Trusted Firmware-M (TF-M) enabled environment.

Workspace structure:
- *tfm_s_crypto_clients*: Project creating the static library required by the secure processing environment (S)
- *el2go_csr_s*: Project running in the secure processing environment (S)
- *el2go_csr_ns*: Project running in the non-secure processing environment (NS)

Details on building and running the application can be found in the
[el2go_csr_ns](../el2go_csr_ns/readme.md) project.

## Supported Boards
- [FRDM-MCXL255](../../../_boards/frdmmcxl255/el2go_examples/el2go_csr/el2go_csr_ns/example_board_readme.md)
- [FRDM-MCXA577](../../../_boards/frdmmcxa577/el2go_examples/el2go_csr/el2go_csr_ns/example_board_readme.md)
- [FRDM-MCXA287](../../../_boards/frdmmcxa287/el2go_examples/el2go_csr/el2go_csr_ns/example_board_readme.md)
