
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
    CC  "--no_inline --no_clustering"
)

mcux_add_macro(
    CC "FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)

# Use board secific implementation
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
      ${board_root}/${board}/sdmmc_config.c
      ${board_root}/${board}/sdmmc_config.h
      ${board_root}/${board}/wifi_bt_config.c
      ${board_root}/${board}/wifi_bt_config.h
)

# Remove default flash linker scripts (flash_debug/flash_release targets)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)

# Add wifi-specific flash linker scripts for all targets
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flash_debug flash_release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_wifi.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flash_debug flash_release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_wifi.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flash_debug flash_release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_wifi.scf
)