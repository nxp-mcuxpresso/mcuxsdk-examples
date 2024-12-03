
# Heap configuration
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x400"
)
# Stack configuration
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x400"
)

# Do not use function inlining for IAR
mcux_add_iar_configuration(
    CC  "--no_inline"
)

# Remove default Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/gcc/RW612_flash.ld
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/iar/RW612_flash.icf
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/arm/RW612_flash.scf
)

# Add Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW610_flash.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW610_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW610_flash.scf
)