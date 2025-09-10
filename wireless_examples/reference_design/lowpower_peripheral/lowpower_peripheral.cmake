# Copyright 2024-2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    SOURCES
    app_config.c
    dynamic_gatt_database.c
    dynamic_gatt_database.h
    lowpower_peripheral.c
    lowpower_peripheral.h
    gatt_db.h
    gatt_uuid128.h
    readme.md
    pics/fig3.PNG
)

mcux_add_linker_symbol(SYMBOLS "gUseNVMLink_d=1" )
# Erase NVM partition (corrupt NVM meta data) during binary flashing
mcux_add_linker_symbol(SYMBOLS "gEraseNVMLink_d=1" )

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/reference_design/lowpower_peripheral/reconfig.cmake)

mcux_add_include(
    INCLUDES
    .
)
