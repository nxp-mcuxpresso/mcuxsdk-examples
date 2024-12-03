if (CONFIG_MCUX_PRJSEG_module.board.lvgl)
    mcux_add_source(
        SOURCES lvgl_support.h
                lvgl_support.c
    )

    mcux_add_include(
        INCLUDES .
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES examples/_boards/${board}/lvgl_examples/RTE_Device.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES examples/_boards/${board}/lvgl_examples
    )
endif()
