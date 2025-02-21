# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/mcxw716_flash.ld
)

mcux_add_armgcc_linker_script(
    TARGETS debug release
    LINKER ./linkers/mcxw716_flash.ld
)
