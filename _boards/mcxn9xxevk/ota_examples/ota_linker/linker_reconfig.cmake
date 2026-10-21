# Remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

# Add the OTA application linker scripts.
# A single linker script handles both flash partition layouts. The custom one
# (bootloader located in the main flash) is selected by the linker symbol
# __mcuboot_main_flash_only__, which is defined by the mcuboot middleware
# when MCUX_COMPONENT_middleware.mcuboot.mcxn_custom_cfg.main_flash_only is
# enabled - see middleware/mcuboot_opensource/CMakeLists.txt
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/mcxn10_cm33_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/mcxn10_cm33_flash.ld
)
