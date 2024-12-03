#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/audio_voice/components/vit/FusionF1/Lib/Inc/PL_platformTypes_HIFI_FUSIONF1.h
            middleware/audio_voice/components/vit/FusionF1/Lib/Inc/PL_memoryRegion.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/audio_voice/components/vit/FusionF1/Lib/Inc
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/fusionf1/binary/
    TARGETS flash_debug flash_release
    SOURCES dsp_reset_release.bin
            dsp_text_release.bin
            dsp_data_release.bin
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/fusionf1/binary/
    INCLUDES ./
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/
    SOURCES incbin.S
    TOOLCHAINS mdk armgcc mcux
)

# Add additional configuration
mcux_add_macro(
    CC "-DBOARD_DMIC_NUM=2 -DBOARD_PCM_WIDTH=16 -DSDK_I3C_BASED_COMPONENT_USED=1 -DXA_VOICE_SEEKER=1"
)

mcux_add_iar_configuration(
    TARGETS flash_debug flash_release
    LD "--image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/fusionf1/binary/dsp_reset_release.bin,__dsp_reset_bin,__dsp_reset_section,4\
       --keep=__dsp_reset_bin\
       --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/fusionf1/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
       --keep=__dsp_text_bin\
       --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/fusionf1/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
       --keep=__dsp_data_bin"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/iar/MIMXRT595Sxxxx_cm33_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/arm/MIMXRT595Sxxxx_cm33_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/gcc/MIMXRT595Sxxxx_cm33_flash.ld
)


# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/dsp_examples/xaf_record/linker/MIMXRT595Sxxxx_cm33_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/dsp_examples/xaf_record/linker/MIMXRT595Sxxxx_cm33_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/dsp_examples/xaf_record/linker/MIMXRT595Sxxxx_cm33_flash.ld
)
