if (CONFIG_MCUX_PRJSEG_module.board.lvgl)
    mcux_add_source(
        SOURCES lvgl_support.h
                lvgl_support.c
    )

    mcux_add_include(
        INCLUDES .
    )
endif()

if (CONFIG_MCUX_PRJSEG_module.board.lvgl.RK043FN02H)
endif()

if (CONFIG_MCUX_PRJSEG_module.board.lvgl.RK043FN66HS)
endif()
