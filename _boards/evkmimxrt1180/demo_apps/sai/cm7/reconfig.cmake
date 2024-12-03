
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/sai/sai/recordtoSdcard.c
            examples/demo_apps/sai/sai/sai.h
            examples/demo_apps/sai/sai/recordPlayback.c
            examples/demo_apps/sai/sai/playbackSineWave.c
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/sdmmc_config.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/sai/sai
             examples/_boards/${board}/demo_apps/sai
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
    CC "-DPRINTF_FLOAT_ENABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc mdk
    CC "-DARM_MATH_CM4"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS hyperram_debug hyperram_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)

mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
)

mcux_remove_macro(
    CC "-DARM_MATH_CM4"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x800\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x800"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x800"
)
