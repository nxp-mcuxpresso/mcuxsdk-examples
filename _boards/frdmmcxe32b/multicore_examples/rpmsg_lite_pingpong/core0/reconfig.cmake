#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_mdk_configuration(
        LD "--keep=*(*core1_code)"
)

# Opt in to the rpmsg-lite shared-memory region defined in the device linker
# files. Without this the rpmsg_sh_mem region has zero length and consumes no
# RAM; defining __use_shmem__ reserves it (0x1800) from the m_stdby_ram window.
mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1"
)
