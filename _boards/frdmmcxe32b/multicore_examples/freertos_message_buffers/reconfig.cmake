#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# frdmmcxe32b keeps FreeRTOSConfigBoard.h at the board root (not under
# freertos_config_board/<core_id>/ as some other boards do).
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/board.h
            ${board_root}/${board}/board.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/clock_config.h
            ${board_root}/${board}/clock_config.c
)
