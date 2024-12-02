#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_usb_demo/hifi4/binary/
    TARGETS flash_debug flash_release
    SOURCES dsp_literal_release.bin
            dsp_text_release.bin
            dsp_data_release.bin
            dsp_ncache_release.bin
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_usb_demo/hifi4/binary/
    INCLUDES ./
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_usb_demo/
    SOURCES incbin.S
    TOOLCHAINS mdk armgcc mcux
)

# Add additional configuration
mcux_add_macro(
    AS "-DDSP_NCACHE"
    CC "-DDSP_NCACHE\
       -DUSB_STACK_FREERTOS_HEAP_SIZE=16384\
       -DUSB_STACK_USE_DEDICATED_RAM=1"
)

mcux_add_iar_configuration(
    TARGETS flash_debug flash_release
    LD "--image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_usb_demo/hifi4/binary/dsp_literal_release.bin,__dsp_literal_bin,__dsp_literal_section,4\
       --keep=__dsp_literal_bin\
       --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_usb_demo/hifi4/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
       --keep=__dsp_text_bin\
       --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_usb_demo/hifi4/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
       --keep=__dsp_data_bin\
       --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_usb_demo/hifi4/binary/dsp_ncache_release.bin,__dsp_ncache_bin,__dsp_ncache_section,4\
       --keep=__dsp_ncache_bin"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT600/MIMXRT685S/iar/MIMXRT685Sxxxx_cm33_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT600/MIMXRT685S/arm/MIMXRT685Sxxxx_cm33_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT600/MIMXRT685S/gcc/MIMXRT685Sxxxx_cm33_flash.ld
)


# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/dsp_examples/xaf_usb_demo/linker/MIMXRT685Sxxxx_cm33_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/dsp_examples/xaf_usb_demo/linker/MIMXRT685Sxxxx_cm33_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/dsp_examples/xaf_usb_demo/linker/MIMXRT685Sxxxx_cm33_flash.ld
)
