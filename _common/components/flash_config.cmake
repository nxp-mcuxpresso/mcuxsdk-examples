# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_COMPONENT_driver.flash_config)

    mcux_add_source(BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/flash_config SOURCES flash_config.c
                    flash_config.h)

    mcux_add_include(BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/flash_config INCLUDES .)

    mcux_add_configuration(
        CC "-DBOOT_HEADER_ENABLE=1"
        CX "-DBOOT_HEADER_ENABLE=1"
    )

endif()

if(CONFIG_MCUX_COMPONENT_driver.flash_config_custom)

    mcux_add_source(BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/flash_config_custom SOURCES flash_config.c
                    flash_config.h)

    mcux_add_include(BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/flash_config_custom INCLUDES .)

    mcux_add_configuration(
        CC "-DBOOT_HEADER_ENABLE=1"
        CX "-DBOOT_HEADER_ENABLE=1"
    )

endif()
