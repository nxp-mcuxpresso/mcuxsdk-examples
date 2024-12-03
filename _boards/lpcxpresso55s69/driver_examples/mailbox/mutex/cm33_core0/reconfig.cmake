
mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS armgcc
    TARGETS debug
    INCLUDES examples/_boards/lpcxpresso55s69/driver_examples/mailbox/mutex/cm33_core0/../cm33_core1/armgcc/debug
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS armgcc
    TARGETS release
    INCLUDES examples/_boards/lpcxpresso55s69/driver_examples/mailbox/mutex/cm33_core0/../cm33_core1/armgcc/release
)
mcux_add_mdk_configuration(
    LD "--keep=*(*core1_code)"
)
mcux_add_macro(
    TOOLCHAINS mdk
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS mdk
    TARGETS debug
    INCLUDES examples/_boards/lpcxpresso55s69/driver_examples/mailbox/mutex/cm33_core0/../cm33_core1/mdk/debug
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS mdk
    TARGETS release
    INCLUDES examples/_boards/lpcxpresso55s69/driver_examples/mailbox/mutex/cm33_core0/../cm33_core1/mdk/release
)
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)
