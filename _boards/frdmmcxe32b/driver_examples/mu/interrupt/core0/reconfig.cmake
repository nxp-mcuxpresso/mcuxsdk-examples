#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# frdmmcxe32b: the secondary (M7_1) image runs in place from flash at
# 0x00600000 and is NOT copied to RAM, so remove the default macro.
mcux_remove_mdk_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_remove_iar_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_remove_armgcc_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)
