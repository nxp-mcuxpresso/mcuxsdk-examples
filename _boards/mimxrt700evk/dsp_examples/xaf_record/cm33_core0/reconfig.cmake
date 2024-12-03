
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/hifi4/binary/
    TARGETS flash_debug flash_release
    SOURCES dsp_literal_release.bin
            dsp_text_release.bin
            dsp_data_release.bin
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/hifi4/binary/
    INCLUDES ./
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/
    SOURCES incbin.S
    TOOLCHAINS mdk armgcc mcux
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/audio_voice/components/vit/HIFI4/Lib/Inc/*.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/audio_voice/components/vit/HIFI4/Lib/Inc
)

mcux_add_macro(
    CC "-DBOARD_DMIC_NUM=1 -DBOARD_PCM_WIDTH=32"
)

mcux_add_iar_configuration(
    TARGETS flash_debug flash_release
    LD "--image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/hifi4/binary/dsp_literal_release.bin,__dsp_literal_bin,__dsp_literal_section,4\
        --keep=__dsp_literal_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/hifi4/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_record/hifi4/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)

mcux_remove_iar_linker_script(
       TARGETS flash_debug flash_release
       BASE_PATH ${SdkRootDirPath}
       LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
)
mcux_remove_mdk_linker_script(
       TARGETS flash_debug flash_release
       BASE_PATH ${SdkRootDirPath}
       LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
)
mcux_remove_armgcc_linker_script(
       TARGETS flash_debug flash_release
       BASE_PATH ${SdkRootDirPath}
       LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/dsp_examples/linker/cm33_core0/MIMXRT798Sxxxx_cm33_core0_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/dsp_examples/linker/cm33_core0/MIMXRT798Sxxxx_cm33_core0_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/dsp_examples/linker/cm33_core0/MIMXRT798Sxxxx_cm33_core0_flash.ld
)
