



# Add additional configuration
mcux_remove_iar_configuration(
    TARGETS flash_release
    CC "-Oh"
)
mcux_add_iar_configuration(
    TARGETS flash_release
    CC "-Om"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/Wireless/RW/RW612/iar/RW612_flash.icf
)

mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/Wireless/RW/RW612/gcc/RW612_flash.ld
)

mcux_remove_mdk_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/Wireless/RW/RW612/arm/RW612_flash.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/RW610_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/RW610_flash.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/RW610_flash.scf
)
