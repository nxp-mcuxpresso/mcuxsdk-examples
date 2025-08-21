# ele_derive_key

## Overview
The ELE derive key application provides example how to use SSSAPI to derive keys by
utilising the HKDF functionality, as well as how to derive keys for an Elliptic Curve
Diffie-Hellman key exchange operation.

## Warning
Please note that HKDF flow depends on the revision of your chip. Expected inputs
for various steps of HKDF have changed with chip revision A2.1. Below is a brief
overview of the two variants.

The change relates only to the HKDF Extract step.

### For revisions A0 through A2:
When initializing a key derivation context for any variant of
`kAlgorithm_SSS_HKDF_*_EXTRACT` via `sss_sscp_derive_key_context_init()`,
salt is to be passed as a key object.
Next, when calling `sss_sscp_derive_key()` for the Extract step,
the Input Keying Material (IKM) is passed as an input buffer.

### For revision A2.1:
The order and approach to passing salt and IKM are swapped compared to
previous revisions.

When initializing a key derivation context for any variant of
`kAlgorithm_SSS_HKDF_*_EXTRACT` via `sss_sscp_derive_key_context_init()`,
IKM is to be passed as a key object.
Next, when calling `sss_sscp_derive_key()` for the Extract step,
the salt is passed as an input buffer.

This approach allows for keeping the IKM opaque (e.g. taking
IKM from the result of an ECDH or SPAKE2+ operation).

## Note
By default, IAR's "Static clustering" optimization option is enabled in the IAR `release` targets.
Compile flag `--no_clustering` has been added to the build to disable static clustering due to it causing erroneous execution of this example.

## Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
The output shown is the output of the example with chip revision A2.1.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE Derive Key via SSSAPI Example

**** Elliptic Curve Diffie-Hellman ****
Init Alice key object...OK
Init Bob key object...OK
Init shared key object...OK
Allocate Alice key object handle...OK
Allocate Bob key object handle...OK
Allocate shared key object handle...OK
Initialize RNG before key generation...OK
Import Alice key pair...OK
Import Bob public key...OK
Init Derive Key Context...OK
ECDH key derivation...OK
Get derived shared secret...OK
Derived shared secret matches expected shared secret...OK
Cleanup...OK

**** HDKF key derivation ****
Init IKM key object...OK
Init PRK key object...OK
Init OKM key object...OK
Allocate IKM key handle...OK
Allocate PRK key handle...OK
Allocate OKM key handle...OK
Set IKM...OK
Initialize HKDF SHA256 extract step context...OK
HKDF SHA256 extract...OK
Free HKDF SHA256 extract step context...OK
Initialize HKDF SHA256 expand step context...OK
HKDF SHA256 expand...OK
Get OKM...OK
Derived OKM matches expected OKM...OK
Cleanup...OK

End of Example with SUCCESS!!

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [KW47-EVK](../../_boards/kw47evk/secure-subsystem_examples/ele_derive_key/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/secure-subsystem_examples/ele_derive_key/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/secure-subsystem_examples/ele_derive_key/example_board_readme.md)
- [MCXW72-LOC](../../_boards/mcxw72loc/secure-subsystem_examples/ele_derive_key/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/secure-subsystem_examples/ele_derive_key/example_board_readme.md)
