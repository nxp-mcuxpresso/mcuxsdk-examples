mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/voice_spot_demo
    INCLUDES ./
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/voice_spot_demo/
    SOURCES dsp_config.h
)

mcux_add_macro(
    CC "-DUSE_PDM_EDMA=1 \
        -DVOICE_SPOT_DEMO_REDUCED_MEMORY_FOOTPRINT=1\
        -DFUSIONF1=1"
)

mcux_add_xtensa_configuration(
	CC "-ffunction-sections -fdata-sections"
    LD "-Wl,-emain -Wl,--gc-sections -Wl,--defsym=__stack=0x840000"
)
