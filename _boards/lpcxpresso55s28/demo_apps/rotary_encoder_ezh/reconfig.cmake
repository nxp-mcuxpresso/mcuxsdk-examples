
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/rotary_encoder_ezh/qei_config.h
)

mcux_add_library(
    BASE_PATH ${SdkRootDirPath}
    LIBS examples/_boards/${board}/demo_apps/rotary_encoder_ezh/lpc5500_qei_lib.lib
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/rotary_encoder_ezh
)

mcux_add_macro(
    CC "-DBOARD_DEBUG_UART_BAUDRATE=512000"
)

# Add or remove Linker File Configurations
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S28/arm/LPC55S28_flash.scf
)

# Add or remove Linker File Configurations
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/rotary_encoder_ezh/LPC55S28_flash.scf
)
