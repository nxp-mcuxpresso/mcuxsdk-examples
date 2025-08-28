# Linker File Selection for KW47 Variants

This document explains how to select and configure the correct linker file for your specific KW47 variant when building wireless examples.

## Identifying Your KW47 Variant

The KW47EVK board default configuration:
- **KW47B42ZB7** - 2MB Flash, 256KB RAM

Other than that the KW47EVK board supports two main variants:
- **KW47B42Z83** - 1MB Flash, 128KB RAM
- **KW47B42Z97** - 1MB Flash, 256KB RAM

Check your device marking or part number to identify which variant you have.

## Selecting the Correct Linker File

### For GCC Toolchain

#### KW47B42Z83
Use the linker file: `connectivity_ble_KW47B42Z83.ld`

**Building with west**
If using west you can update `gcc_wireless_linker_file` and `gcc_wireless_linker_file_ble` in `examples\_boards\kw47evk\project_segments\wireless\prjseg.cmake` with the value `connectivity_ble_KW47B42Z83.ld` and building your project.

#### KW47B42Z97
Use the linker file: `connectivity_ble_KW47B42Z97.ld`

**Building with west**
If using west you can update `gcc_wireless_linker_file` and `gcc_wireless_linker_file_ble` in `examples\_boards\kw47evk\project_segments\wireless\prjseg.cmake` with the value `connectivity_ble_KW47B42Z97.ld` and building your project.

### For IAR Toolchain

#### KW47B42Z83
Use the linker file: `connectivity_KW47B42Z83.icf`

**Building with west**
If using west you can update `iar_wireless_linker_file` in `examples\_boards\kw47evk\project_segments\wireless\prjseg.cmake` with the value `connectivity_KW47B42Z83.icf` and building your project.

**In IDE:**
1. Right-click project → Options → Linker → Config
2. Override default → Use linker command file
3. Browse and select: `connectivity_KW47B42Z83.icf`

#### KW47B42Z97
Use the linker file: `connectivity_KW47B42Z97.icf`

**Building with west**
If using west you can update `iar_wireless_linker_file` in `examples\_boards\kw47evk\project_segments\wireless\prjseg.cmake` with the value `connectivity_KW47B42Z97.icf`.

**In IDE:**
1. Right-click project → Options → Linker → Config
2. Override default → Use linker command file
3. Browse and select: `connectivity_KW47B42Z97.icf`

## Quick Reference

| Your Variant | GCC Linker File                  | IAR Linker File               |
|--------------|----------------------------------|-------------------------------|
| KW47B42Z83   | `connectivity_ble_KW47B42Z83.ld` | `connectivity_KW47B42Z83.icf` |
| KW47B42Z97   | `connectivity_ble_KW47B42Z97.ld` | `connectivity_KW47B42Z97.icf` |
| others       | `connectivity_ble.ld`            | `connectivity.icf`            |

**Important:** Always ensure your linker file matches your physical device variant to avoid runtime issues or build failures.