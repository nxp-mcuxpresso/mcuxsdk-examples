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
#if (defined(CPU_MIMXRT2662AVJ7A) || defined(CPU_MIMXRT2662AVM7A) || defined(CPU_MIMXRT2662CVJ8A) || defined(CPU_MIMXRT2662CVL8A) || defined(CPU_MIMXRT2662CVM8A) || defined(CPU_MIMXRT2662DVJAA) || defined(CPU_MIMXRT2662DVLAA) || defined(CPU_MIMXRT2662DVMAA) || defined(CPU_MIMXRT2662XVJ78A) || defined(CPU_MIMXRT2662XVL8A) || defined(CPU_MIMXRT2662XVM8A))
        .incbin "../../../middleware/eiq/tensorflow-lite/third_party/neutron/rt2660/NeutronFirmware.elf"
#endif
_binary_NeutronFirmware_elf_end:
#endif
