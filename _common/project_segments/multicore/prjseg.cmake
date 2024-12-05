# core1 support
if(CONFIG_MCUX_PRJSEG_module.core1_support)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/
        SOURCES core1_support.c
                core1_support.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/
        INCLUDES ./
    )
endif()
