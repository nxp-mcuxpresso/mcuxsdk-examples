if (CONFIG_MCUX_PRJSEG_module.board.lvgl)
    mcux_add_source(
        SOURCES lvgl_support.h
                lvgl_support.c
    )

    mcux_add_include(
        INCLUDES .
    )

    mcux_add_macro(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
    )
endif()
