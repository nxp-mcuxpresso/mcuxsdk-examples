
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/sema42/hifi1/binary/
    SOURCES dsp_vector_release.bin
            dsp_text_release.bin
            dsp_data_release.bin
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/sema42/hifi1/binary/
    INCLUDES ./
)

mcux_add_iar_configuration(
    LD "--image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/sema42/hifi1/binary/dsp_vector_release.bin,__dsp_vector_bin,__dsp_vector_section,4\
        --keep=__dsp_vector_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/sema42/hifi1/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/sema42/hifi1/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)