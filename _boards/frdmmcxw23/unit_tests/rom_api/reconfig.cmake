# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_project_remove_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES devices_int/MCX/MCXW/periph4
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples_int/_devices/MCXW236B/periph4
)

mcux_project_remove_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES devices_int/MCX/MCXW/MCXW236B
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples_int/_devices/MCXW236B
)

mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/MCXW236B_flash.ld
)

mcux_add_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/unit_tests/rom_api/linker/MCXW236B_rom_api.ld
)

mcux_add_macro(
  CC "-DENABLE_RAM_VECTOR_TABLE=1"
)
