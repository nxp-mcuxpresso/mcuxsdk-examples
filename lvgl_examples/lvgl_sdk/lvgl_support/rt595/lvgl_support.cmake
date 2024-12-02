if (CONFIG_MCUX_PRJSEG_module.board.lvgl)
    mcux_add_source(
        SOURCES lvgl_support.h
                lvgl_support.c
    )

    mcux_add_include(
        INCLUDES .
    )


    mcux_add_macro(
        CC "-DBOARD_ENABLE_PSRAM_CACHE=0\
           -DSSD1963_DATA_WITDH=8\
           -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8\
           -DSDK_I2C_BASED_COMPONENT_USED=1"
    )
endif()
