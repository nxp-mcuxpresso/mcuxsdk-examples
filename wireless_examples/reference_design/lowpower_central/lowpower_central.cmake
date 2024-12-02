# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    SOURCES
    app_config.c
    dynamic_gatt_database.c
    dynamic_gatt_database.h
    lowpower_central.c
    lowpower_central.h
    gatt_db.h
    gatt_uuid128.h
    ../README.md
    ../pics/fig3.PNG
)

mcux_add_linker_symbol(SYMBOLS "gUseNVMLink_d=1" )
# Erase NVM partition (corrupt NVM meta data) during binary flashing
mcux_add_linker_symbol(SYMBOLS "gEraseNVMLink_d=1" )

mcux_add_iar_configuration(
    CC "--warnings_are_errors"
)

mcux_add_armgcc_configuration(
    CC "-Werror"
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/reference_design/lowpower_central/reconfig.cmake)

mcux_add_include(
    INCLUDES
    .
)
