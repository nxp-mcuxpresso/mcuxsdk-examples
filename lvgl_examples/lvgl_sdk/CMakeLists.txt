if (CONFIG_MCUX_COMPONENT_middleware.lvgl)

    mcux_add_macro(
        CC "-DLV_CONF_INCLUDE_SIMPLE=1"
        CX "-DLV_CONF_INCLUDE_SIMPLE=1"
    )

    mcux_add_armgcc_configuration(
        CC "-fms-extensions"
        CX "-fms-extensions"
    )

    mcux_add_mdk_configuration(
        CC "-fms-extensions"
        CX "-fms-extensions"
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
	    INCLUDES
            middleware/lvgl
            middleware/lvgl/src/draw/nxp/vglite
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/src/**.c
                middleware/lvgl/src/**.h
                middleware/lvgl/*.h
    )

endif()

if(CONFIG_MCUX_COMPONENT_middleware.lvgl.demo.widgets)

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/demos/widgets/*.c
                middleware/lvgl/demos/widgets/*.h
                middleware/lvgl/demos/widgets/assets/*.c
                middleware/lvgl/demos/widgets/assets/*.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
            INCLUDES
            middleware/lvgl/demos/widgets
            middleware/lvgl/demos/widgets/assets
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.lvgl.demo.keypad_encoder)

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/demos/keypad_encoder/*.c
                middleware/lvgl/demos/keypad_encoder/*.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
            INCLUDES
            middleware/lvgl/demos/keypad_encoder
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.lvgl.demo.benchmark)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/demos/benchmark/*.c
                middleware/lvgl/demos/benchmark/*.h
                middleware/lvgl/demos/benchmark/assets/*.c
                middleware/lvgl/demos/benchmark/assets/*.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
            INCLUDES
            middleware/lvgl/demos/benchmark
            middleware/lvgl/demos/benchmark/assets
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.lvgl.demo.common)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/demos/*.c
                middleware/lvgl/demos/*.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
            INCLUDES
            middleware/lvgl/demos
    )
endif()


if(CONFIG_MCUX_COMPONENT_middleware.lvgl.demo.stress)

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/demos/stress/*.c
                middleware/lvgl/demos/stress/*.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
            INCLUDES
            middleware/lvgl/demos/stress
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.lvgl.demo.transform)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/demos/transform/*.c
                middleware/lvgl/demos/transform/*.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
            INCLUDES
            middleware/lvgl/demos/transform
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.lvgl.demo.multilang)

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/demos/multilang/*.c
                middleware/lvgl/demos/multilang/*.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
            INCLUDES
            middleware/lvgl/demos/multilang
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.lvgl.demo.flex_layout)

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/demos/flex_layout/*.c
                middleware/lvgl/demos/flex_layout/*.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
            INCLUDES
            middleware/lvgl/demos/flex_layout
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.lvgl.demo.scroll)

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/demos/scroll/*.c
                middleware/lvgl/demos/scroll/*.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
            INCLUDES
            middleware/lvgl/demos/scroll
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.lvgl.demo.music)

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/demos/music/*.c
                middleware/lvgl/demos/music/*.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
            INCLUDES
            middleware/lvgl/demos/music
    )
endif()

if (CONFIG_MCUX_COMPONENT_middleware.lvgl.template)

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES examples/lvgl_examples/lvgl_sdk/template/lv_conf.h
                examples/lvgl_examples/lvgl_sdk/template/lvgl_support.c
        CONFIG true
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
            INCLUDES examples/lvgl_examples/lvgl_sdk/template
    )
endif()
