/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if defined(__GNUC__) || defined(__ARMCC_VERSION)
        .section .elfdata , "ax" @progbits @preinit_array
        .align 16
        .global _binary_NeutronFirmware_elf_start
        .global _binary_NeutronFirmware_elf_end

_binary_NeutronFirmware_elf_start:
#if (defined(CPU_MIMXRT2663AHP8A) || defined(CPU_MIMXRT2663CHPAA) || defined(CPU_MIMXRT2663CVVAA) || defined(CPU_MIMXRT2663DHPAA) || defined(CPU_MIMXRT2663DVVAA) || defined(CPU_MIMXRT2663XHP8A) || defined(CPU_MIMXRT2663XVV8A))
        .incbin "../../../middleware/eiq_int/neutron/rt2660/NeutronFirmware.elf"
#endif
_binary_NeutronFirmware_elf_end:
#endif
