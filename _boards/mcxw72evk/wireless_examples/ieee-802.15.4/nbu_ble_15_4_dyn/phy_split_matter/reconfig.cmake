# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/wireless_examples/linker/iar/${core_id}/${iar_wireless_linker_file}
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/wireless_examples/linker/iar/nbu_flash_connectivity_matter.icf
)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/mcxw72_nbu_ble_15_4_dyn_matter.bin
)
