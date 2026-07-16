# EdgeLock Certificate Signing Request (CSR) (S)

This sample application demonstrates how to generate Certificate Signing Requests (CSR) and verify X.509 certificates on an MCU device using PSA Crypto APIs.

Workspace structure:
- *tfm_s_crypto_client*: Project creating the static library required by the secure processing environment (S)
- *el2go_csr_s*: Project running in the secure processing environment (S)
- *el2go_csr_ns*: Project running in the non-secure processing environment (NS)

Details on building and running the application can be found in the
[el2go_csr_ns](../el2go_csr_ns/readme.md) project.
