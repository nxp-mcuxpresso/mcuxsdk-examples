
mcux_add_iar_configuration(
    TARGETS sdram_txt_debug sdram_txt_release
    CC "-Om"
)
mcux_add_mdk_configuration(
    TARGETS sdram_txt_debug
    CC "-g"
)
mcux_add_mdk_configuration(
    TARGETS sdram_txt_release
    CC "-O3"
)
mcux_add_armgcc_configuration(
    TARGETS sdram_txt_debug
    CC "-g\
       -O0"
    CX "-g\
       -O0"
    LD "-g"
)
mcux_add_armgcc_configuration(
    TARGETS sdram_txt_release
    CC "-Os"
    CX "-Os"
)

mcux_add_macro(
    TOOLCHAINS iar mdk
    TARGETS sdram_txt_debug sdram_txt_release
    CC "-DUSE_SDRAM"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS sdram_txt_release
    CC "-DUSE_SDRAM\
       -DNDEBUG"
    CX "-DNDEBUG"
    AS "-DNDEBUG"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS sdram_txt_debug sdram_txt_release
    CC "-DUSE_SDRAM"
    AS "-D__STARTUP_CLEAR_BSS"
)

mcux_remove_iar_configuration(
    TARGETS sdram_txt_debug
    CC "-On"
)
mcux_remove_iar_configuration(
    TARGETS sdram_txt_release
    CC "-Oh"
)

mcux_remove_macro(
    TOOLCHAINS armgcc
    TARGETS sdram_txt_debug sdram_txt_release
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_txt_debug sdram_txt_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/iar/MIMXRT1152xxxxx_hyperram_txt.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_txt_debug sdram_txt_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/arm/MIMXRT1152xxxxx_hyperram_txt.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_txt_debug sdram_txt_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/gcc/MIMXRT1152xxxxx_hyperram_txt.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_txt_debug sdram_txt_release
    LINKER ${board_root}/${board}/driver_examples/flexram/ram_allocate/linker/frdmimxrt1152_flexram_ram_allocate.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_txt_debug sdram_txt_release
    LINKER ${board_root}/${board}/driver_examples/flexram/ram_allocate/linker/frdmimxrt1152_flexram_ram_allocate.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_txt_debug sdram_txt_release
    LINKER ${board_root}/${board}/driver_examples/flexram/ram_allocate/linker/frdmimxrt1152_flexram_ram_allocate.ld
)
