
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/sai/sai/recordtoSdcard.c
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/sdmmc_config.c
            examples/demo_apps/sai/sai/sai.h
            examples/demo_apps/sai/sai/recordPlayback.c
            examples/demo_apps/sai/sai/playbackSineWave.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/sai
             examples/demo_apps/sai/sai
)

mcux_add_armgcc_configuration(
    CC "-O1"
    LD "-lm"
)
mcux_add_iar_configuration(
    TARGETS debug
    CC "-Ol"
)

mcux_add_macro(
    TOOLCHAINS armgcc mdk
    CC "-DARM_MATH_CM7\
       -DARM_MATH_CM4"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS sdram_debug sdram_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DARM_MATH_CM7"
)
mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1"
)

mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
)
