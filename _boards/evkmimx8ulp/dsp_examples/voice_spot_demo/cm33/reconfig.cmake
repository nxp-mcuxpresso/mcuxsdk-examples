
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/voice_spot_demo/
    SOURCES fusionf1/binary/dsp_reset_release.bin
            fusionf1/binary/dsp_text_release.bin
            fusionf1/binary/dsp_data_release.bin
            dsp_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/voice_spot_demo/
    INCLUDES fusionf1/binary/
             ./
)

mcux_add_macro(
    CC "-DENABLE_IDLE_CALLBACK=1\
        -DFUSION_F1_TRDC=1"
)

mcux_add_iar_configuration(
    LD "--image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/voice_spot_demo/fusionf1/binary/dsp_reset_release.bin,__dsp_reset_bin,__dsp_reset_section,4\
        --keep=__dsp_reset_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/voice_spot_demo/fusionf1/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/voice_spot_demo/fusionf1/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)
