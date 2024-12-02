# core1 support
if(CONFIG_MCUX_PRJSEG_module.core1_support)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/
        SOURCES core1_support.c
                core1_support.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/
        INCLUDES ./
    )
endif()
