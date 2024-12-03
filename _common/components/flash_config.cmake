if(CONFIG_MCUX_COMPONENT_driver.flash_config)

    mcux_add_source(BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/flash_config SOURCES flash_config.c
                    flash_config.h)

    mcux_add_include(BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/flash_config INCLUDES .)

    mcux_add_configuration(
        CC "-DBOOT_HEADER_ENABLE=1"
        CX "-DBOOT_HEADER_ENABLE=1"
    )

endif()
