# Linker File Selection for KW47 Variants

This document explains how to select and configure the correct linker file for your specific KW47 variant when building wireless examples.

## Identifying Your KW47 Variant

The KW47EVK board default configuration:
- **KW47B42ZB7/B6/B3/B2** - 2MB Flash, 264KB RAM

Other than that the KW47EVK board supports two other variants:
- **KW47B42Z83** - 1MB Flash, 136KB RAM
- **KW47B42Z97/96** - 1MB Flash, 264KB RAM

Check your device marking or part number to identify which variant you have.

## Selecting the Correct Linker File

The linker script is generated during the project generation, using the toolchain preprocessor. The generated linker
script will use the appropriate memory layout based on which KW47 variant being used.

Make sure to use the `--device <variant>` in your west command line to select the correct target variant. The variant
used by default is `KW47B42ZB7`.
As an example, if you want to build for `KW47B42Z83`, use:
```bash
west build -b kw47evk --device KW47B42Z83 ...
```

The generated linker script can be found in the build directory:
`<build dir>/linker.ld|icf`

The memory layout is constructed based on a header file definings macros based on which device is selected.
For KW47B42Zxx variants, you can find this file under:
`mcuxsdk/examples/_common/project_segments/wireless/kw47b42z/conn_device_memory.h`
