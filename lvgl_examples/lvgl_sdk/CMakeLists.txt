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
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES middleware/lvgl/src/*.c
                middleware/lvgl/src/core/*.c
                middleware/lvgl/src/display/*.c
                middleware/lvgl/src/draw/*.c
                middleware/lvgl/src/draw/nxp/pxp/*.c
                middleware/lvgl/src/draw/nxp/vglite/*.c
                middleware/lvgl/src/draw/sdl/*.c
                middleware/lvgl/src/draw/sw/arm2d/*.c
                middleware/lvgl/src/draw/sw/blend/arm2d/*.c
                middleware/lvgl/src/draw/sw/blend/*.c
                middleware/lvgl/src/draw/sw/blend/neon/*.c
                middleware/lvgl/src/draw/sw/*.c
                middleware/lvgl/src/drivers/*.c
                middleware/lvgl/src/drivers/display/drm/*.c
                middleware/lvgl/src/drivers/display/fb/*.c
                middleware/lvgl/src/drivers/display/ili9341/*.c
                middleware/lvgl/src/drivers/display/lcd/*.c
                middleware/lvgl/src/drivers/display/st7735/*.c
                middleware/lvgl/src/drivers/display/st7789/*.c
                middleware/lvgl/src/drivers/display/st7796/*.c
                middleware/lvgl/src/drivers/display/tft_espi/*.c
                middleware/lvgl/src/drivers/evdev/*.c
                middleware/lvgl/src/drivers/libinput/*.c
                middleware/lvgl/src/drivers/nuttx/*.c
                middleware/lvgl/src/drivers/sdl/*.c
                middleware/lvgl/src/drivers/windows/*.c
                middleware/lvgl/src/drivers/x11/*.c
                middleware/lvgl/src/drivers/glfw/*.c
                middleware/lvgl/src/drivers/qnx/*.c
                middleware/lvgl/src/drivers/wayland/*.c
                middleware/lvgl/src/font/*.c
                middleware/lvgl/src/indev/*.c
                middleware/lvgl/src/layouts/flex/*.c
                middleware/lvgl/src/layouts/grid/*.c
                middleware/lvgl/src/layouts/*.c
                middleware/lvgl/src/libs/barcode/*.c
                middleware/lvgl/src/libs/bin_decoder/*.c
                middleware/lvgl/src/libs/bmp/*.c
                middleware/lvgl/src/libs/ffmpeg/*.c
                middleware/lvgl/src/libs/freetype/*.c
                middleware/lvgl/src/libs/fsdrv/*.c
                middleware/lvgl/src/libs/gif/*.c
                middleware/lvgl/src/libs/libjpeg_turbo/*.c
                middleware/lvgl/src/libs/libpng/*.c
                middleware/lvgl/src/libs/lodepng/*.c
                middleware/lvgl/src/libs/lz4/*.c
                middleware/lvgl/src/libs/qrcode/*.c
                middleware/lvgl/src/libs/rle/*.c
                middleware/lvgl/src/libs/rlottie/*.c
                middleware/lvgl/src/libs/thorvg/*.c
                middleware/lvgl/src/libs/tiny_ttf/*.c
                middleware/lvgl/src/libs/tjpgd/*.c
                middleware/lvgl/src/misc/cache/*.c
                middleware/lvgl/src/misc/*.c
                middleware/lvgl/src/osal/*.c
                middleware/lvgl/src/others/file_explorer/*.c
                middleware/lvgl/src/others/fragment/*.c
                middleware/lvgl/src/others/gridnav/*.c
                middleware/lvgl/src/others/ime/*.c
                middleware/lvgl/src/others/imgfont/*.c
                middleware/lvgl/src/others/monkey/*.c
                middleware/lvgl/src/others/observer/*.c
                middleware/lvgl/src/others/snapshot/*.c
                middleware/lvgl/src/others/sysmon/*.c
                middleware/lvgl/src/others/vg_lite_tvg/*.c
                middleware/lvgl/src/stdlib/builtin/*.c
                middleware/lvgl/src/stdlib/clib/*.c
                middleware/lvgl/src/stdlib/*.c
                middleware/lvgl/src/stdlib/micropython/*.c
                middleware/lvgl/src/stdlib/rtthread/*.c
                middleware/lvgl/src/themes/default/*.c
                middleware/lvgl/src/themes/*.c
                middleware/lvgl/src/themes/mono/*.c
                middleware/lvgl/src/themes/simple/*.c
                middleware/lvgl/src/tick/*.c
                middleware/lvgl/src/widgets/animimage/*.c
                middleware/lvgl/src/widgets/arc/*.c
                middleware/lvgl/src/widgets/bar/*.c
                middleware/lvgl/src/widgets/button/*.c
                middleware/lvgl/src/widgets/buttonmatrix/*.c
                middleware/lvgl/src/widgets/calendar/*.c
                middleware/lvgl/src/widgets/carousel/*.c
                middleware/lvgl/src/widgets/canvas/*.c
                middleware/lvgl/src/widgets/chart/*.c
                middleware/lvgl/src/widgets/checkbox/*.c
                middleware/lvgl/src/widgets/dropdown/*.c
                middleware/lvgl/src/widgets/image/*.c
                middleware/lvgl/src/widgets/imagebutton/*.c
                middleware/lvgl/src/widgets/keyboard/*.c
                middleware/lvgl/src/widgets/label/*.c
                middleware/lvgl/src/widgets/led/*.c
                middleware/lvgl/src/widgets/line/*.c
                middleware/lvgl/src/widgets/list/*.c
                middleware/lvgl/src/widgets/lottie/*.c
                middleware/lvgl/src/widgets/menu/*.c
                middleware/lvgl/src/widgets/msgbox/*.c
                middleware/lvgl/src/widgets/objx_templ/*.c
                middleware/lvgl/src/widgets/roller/*.c
                middleware/lvgl/src/widgets/scale/*.c
                middleware/lvgl/src/widgets/slider/*.c
                middleware/lvgl/src/widgets/span/*.c
                middleware/lvgl/src/widgets/spinbox/*.c
                middleware/lvgl/src/widgets/spinner/*.c
                middleware/lvgl/src/widgets/switch/*.c
                middleware/lvgl/src/widgets/table/*.c
                middleware/lvgl/src/widgets/tabview/*.c
                middleware/lvgl/src/widgets/textarea/*.c
                middleware/lvgl/src/widgets/tileview/*.c
                middleware/lvgl/src/widgets/win/*.c
                middleware/lvgl/src/*.h
                middleware/lvgl/src/core/*.h
                middleware/lvgl/src/display/*.h
                middleware/lvgl/src/draw/*.h
                middleware/lvgl/src/draw/nxp/pxp/*.h
                middleware/lvgl/src/draw/nxp/vglite/*.h
                middleware/lvgl/src/draw/sdl/*.h
                middleware/lvgl/src/draw/sw/arm2d/*.h
                middleware/lvgl/src/draw/sw/blend/arm2d/*.h
                middleware/lvgl/src/draw/sw/blend/*.h
                middleware/lvgl/src/draw/sw/blend/neon/*.h
                middleware/lvgl/src/draw/sw/*.h
                middleware/lvgl/src/drivers/*.h
                middleware/lvgl/src/drivers/display/drm/*.h
                middleware/lvgl/src/drivers/display/fb/*.h
                middleware/lvgl/src/drivers/display/ili9341/*.h
                middleware/lvgl/src/drivers/display/lcd/*.h
                middleware/lvgl/src/drivers/display/st7735/*.h
                middleware/lvgl/src/drivers/display/st7789/*.h
                middleware/lvgl/src/drivers/display/st7796/*.h
                middleware/lvgl/src/drivers/display/tft_espi/*.h
                middleware/lvgl/src/drivers/evdev/*.h
                middleware/lvgl/src/drivers/libinput/*.h
                middleware/lvgl/src/drivers/nuttx/*.h
                middleware/lvgl/src/drivers/sdl/*.h
                middleware/lvgl/src/drivers/windows/*.h
                middleware/lvgl/src/drivers/x11/*.h
                middleware/lvgl/src/drivers/glfw/*.h
                middleware/lvgl/src/drivers/qnx/*.h
                middleware/lvgl/src/drivers/wayland/*.h
                middleware/lvgl/src/font/*.h
                middleware/lvgl/src/indev/*.h
                middleware/lvgl/src/layouts/flex/*.h
                middleware/lvgl/src/layouts/grid/*.h
                middleware/lvgl/src/layouts/*.h
                middleware/lvgl/src/libs/barcode/*.h
                middleware/lvgl/src/libs/bin_decoder/*.h
                middleware/lvgl/src/libs/bmp/*.h
                middleware/lvgl/src/libs/ffmpeg/*.h
                middleware/lvgl/src/libs/freetype/*.h
                middleware/lvgl/src/libs/fsdrv/*.h
                middleware/lvgl/src/libs/gif/*.h
                middleware/lvgl/src/libs/libjpeg_turbo/*.h
                middleware/lvgl/src/libs/libpng/*.h
                middleware/lvgl/src/libs/lodepng/*.h
                middleware/lvgl/src/libs/lz4/*.h
                middleware/lvgl/src/libs/qrcode/*.h
                middleware/lvgl/src/libs/rle/*.h
                middleware/lvgl/src/libs/rlottie/*.h
                middleware/lvgl/src/libs/thorvg/*.h
                middleware/lvgl/src/libs/tiny_ttf/*.h
                middleware/lvgl/src/libs/tjpgd/*.h
                middleware/lvgl/src/misc/cache/*.h
                middleware/lvgl/src/misc/*.h
                middleware/lvgl/src/osal/*.h
                middleware/lvgl/src/others/file_explorer/*.h
                middleware/lvgl/src/others/fragment/*.h
                middleware/lvgl/src/others/gridnav/*.h
                middleware/lvgl/src/others/ime/*.h
                middleware/lvgl/src/others/imgfont/*.h
                middleware/lvgl/src/others/monkey/*.h
                middleware/lvgl/src/others/observer/*.h
                middleware/lvgl/src/others/snapshot/*.h
                middleware/lvgl/src/others/sysmon/*.h
                middleware/lvgl/src/others/vg_lite_tvg/*.h
                middleware/lvgl/src/stdlib/builtin/*.h
                middleware/lvgl/src/stdlib/clib/*.h
                middleware/lvgl/src/stdlib/*.h
                middleware/lvgl/src/stdlib/micropython/*.h
                middleware/lvgl/src/stdlib/rtthread/*.h
                middleware/lvgl/src/themes/default/*.h
                middleware/lvgl/src/themes/*.h
                middleware/lvgl/src/themes/mono/*.h
                middleware/lvgl/src/themes/simple/*.h
                middleware/lvgl/src/tick/*.h
                middleware/lvgl/src/widgets/animimage/*.h
                middleware/lvgl/src/widgets/arc/*.h
                middleware/lvgl/src/widgets/bar/*.h
                middleware/lvgl/src/widgets/button/*.h
                middleware/lvgl/src/widgets/buttonmatrix/*.h
                middleware/lvgl/src/widgets/calendar/*.h
                middleware/lvgl/src/widgets/canvas/*.h
                middleware/lvgl/src/widgets/carousel/*.h
                middleware/lvgl/src/widgets/chart/*.h
                middleware/lvgl/src/widgets/checkbox/*.h
                middleware/lvgl/src/widgets/dropdown/*.h
                middleware/lvgl/src/widgets/image/*.h
                middleware/lvgl/src/widgets/imagebutton/*.h
                middleware/lvgl/src/widgets/keyboard/*.h
                middleware/lvgl/src/widgets/label/*.h
                middleware/lvgl/src/widgets/led/*.h
                middleware/lvgl/src/widgets/line/*.h
                middleware/lvgl/src/widgets/list/*.h
                middleware/lvgl/src/widgets/lottie/*.h
                middleware/lvgl/src/widgets/menu/*.h
                middleware/lvgl/src/widgets/msgbox/*.h
                middleware/lvgl/src/widgets/objx_templ/*.h
                middleware/lvgl/src/widgets/roller/*.h
                middleware/lvgl/src/widgets/scale/*.h
                middleware/lvgl/src/widgets/slider/*.h
                middleware/lvgl/src/widgets/span/*.h
                middleware/lvgl/src/widgets/spinbox/*.h
                middleware/lvgl/src/widgets/spinner/*.h
                middleware/lvgl/src/widgets/switch/*.h
                middleware/lvgl/src/widgets/table/*.h
                middleware/lvgl/src/widgets/tabview/*.h
                middleware/lvgl/src/widgets/textarea/*.h
                middleware/lvgl/src/widgets/tileview/*.h
                middleware/lvgl/src/widgets/win/*.h
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
