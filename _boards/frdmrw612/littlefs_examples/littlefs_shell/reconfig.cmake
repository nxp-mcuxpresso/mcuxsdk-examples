
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/littlefs_examples/littlefs_shell/peripherals.c
            ${board_root}/${board}/littlefs_examples/littlefs_shell/peripherals.h
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS hyperram_release release sdram_release
    CC "-DLFS_NO_ASSERT"
)
mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1\
       -DSDK_DEBUGCONSOLE_UART"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/Wireless/RW/RW612/iar/RW612_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/Wireless/RW/RW612/arm/RW612_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/Wireless/RW/RW612/gcc/RW612_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/iar/RW612_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/mdk/RW612_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/armgcc/RW612_flash.ld
)
