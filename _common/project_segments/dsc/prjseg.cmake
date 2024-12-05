# dsp support
if(CONFIG_MCUX_PRJSEG_module.use_board_peripheral)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/peripherals
        SOURCES peripherals.c
                 peripherals.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/peripherals
        INCLUDES ./
    )
endif()