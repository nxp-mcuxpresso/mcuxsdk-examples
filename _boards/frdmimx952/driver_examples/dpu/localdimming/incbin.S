/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Purpose: Embed imx9xm0p_ld_fw.bin into ELF using .incbin
 */

    .syntax unified
    .thumb

    /* Put into a dedicated read-only section */
    .section .ld_firmware.rodata, "a", %progbits
    .balign 4

    /* Export symbols for C code */
    .global __ld_fw_start
    .global __ld_fw_end
    .global __ld_fw_size

    .type   __ld_fw_start, %object
    .type   __ld_fw_end,   %object
    .type   __ld_fw_size,  %object

__ld_fw_start:
    .incbin "imx9xm0p_ld_fw.bin"   /* Ensure this file is in the same directory */
__ld_fw_end:
    .set __ld_fw_size, __ld_fw_end - __ld_fw_start
