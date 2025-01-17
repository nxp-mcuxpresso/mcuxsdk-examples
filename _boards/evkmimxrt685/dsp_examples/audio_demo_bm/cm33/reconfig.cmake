#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/audio_demo_bm/hifi4/binary/
    SOURCES dsp_literal_release.bin
            dsp_text_release.bin
            dsp_data_release.bin
	    dsp_ncache_release.bin
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/audio_demo_bm/hifi4/binary/
    INCLUDES ./
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/audio_demo_bm/
    SOURCES incbin.S
    TOOLCHAINS mdk armgcc mcux
)

# Add additional configuration
mcux_add_macro(
    AS "-DDSP_NCACHE"
    CC "-DDSP_NCACHE"
)
mcux_add_iar_configuration(
    LD "--image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/audio_demo_bm/hifi4/binary/dsp_literal_release.bin,__dsp_literal_bin,__dsp_literal_section,4\
        --keep=__dsp_literal_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/audio_demo_bm/hifi4/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/audio_demo_bm/hifi4/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/audio_demo_bm/hifi4/binary/dsp_ncache_release.bin,__dsp_ncache_bin,__dsp_ncache_section,4\
        --keep=__dsp_ncache_bin"

)
