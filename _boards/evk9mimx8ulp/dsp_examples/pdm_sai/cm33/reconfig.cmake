
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/pdm_sai/fusionf1/binary/
    SOURCES dsp_reset_release.bin
            dsp_text_release.bin
            dsp_data_release.bin
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/pdm_sai/fusionf1/binary/
    INCLUDES ./
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/dsp_examples/pdm_sai/cm33/./hardware_init.c
            examples/_boards/evkmimx8ulp/dsp_examples/pdm_sai/cm33/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/dsp_examples/pdm_sai/cm33/.
             examples/_boards/evkmimx8ulp
)

mcux_add_iar_configuration(
    LD "--image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/pdm_sai/fusionf1/binary/dsp_reset_release.bin,__dsp_reset_bin,__dsp_reset_section,4\
        --keep=__dsp_reset_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/pdm_sai/fusionf1/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/pdm_sai/fusionf1/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)
