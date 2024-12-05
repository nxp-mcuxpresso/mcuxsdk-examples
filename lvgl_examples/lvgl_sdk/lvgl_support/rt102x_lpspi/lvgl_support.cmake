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
        SOURCES ${board_root}/${board}/lvgl_examples/lvgl_support/lvgl_support_board.h
                ${board_root}/${board}/lvgl_examples/lvgl_support/RTE_Device.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES ${board_root}/${board}/lvgl_examples/lvgl_support
    )

endif()
