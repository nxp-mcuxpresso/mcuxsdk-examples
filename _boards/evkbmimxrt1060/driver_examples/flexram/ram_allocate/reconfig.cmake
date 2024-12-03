
mcux_add_iar_configuration(
    TARGETS sdram_txt_debug sdram_txt_release
    CC "-Om"
)
mcux_add_mdk_configuration(
    TARGETS sdram_txt_debug
    CC "-O1\
       -g"
    CX "-O1\
       -g"
)
mcux_add_mdk_configuration(
    TARGETS sdram_txt_release
    CC "-Oz"
    CX "-Oz"
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
    CC "-DSKIP_SYSCLK_INIT"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS sdram_txt_release
    CC "-DSKIP_SYSCLK_INIT\
       -DNDEBUG"
    CX "-DNDEBUG"
    AS "-DNDEBUG"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS sdram_txt_debug sdram_txt_release
    CC "-DSKIP_SYSCLK_INIT"
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
    LINKER devices/RT/RT1060/MIMXRT1062/iar/MIMXRT1062xxxxx_sdram_txt.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_txt_debug sdram_txt_release
    LINKER devices/RT/RT1060/MIMXRT1062/arm/MIMXRT1062xxxxx_sdram_txt.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_txt_debug sdram_txt_release
    LINKER devices/RT/RT1060/MIMXRT1062/gcc/MIMXRT1062xxxxx_sdram_txt.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_txt_debug sdram_txt_release
    LINKER examples/_boards/${board}/driver_examples/flexram/ram_allocate/linker/iar/evkbmimxrt1060_flexram_ram_allocate.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_txt_debug sdram_txt_release
    LINKER examples/_boards/${board}/driver_examples/flexram/ram_allocate/linker/mdk/evkbmimxrt1060_flexram_ram_allocate.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS sdram_txt_debug sdram_txt_release
    LINKER examples/_boards/${board}/driver_examples/flexram/ram_allocate/linker/armgcc/evkbmimxrt1060_flexram_ram_allocate.ld
)
