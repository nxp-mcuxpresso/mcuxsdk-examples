
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/tee_fault/core0/resource_config.h
            examples/_boards/${board}/demo_apps/tee_fault/core0/resource_config.c
            examples/demo_apps/tee_fault/tee_fault_common.h
)

mcux_add_mdk_configuration(
    LD "--keep=*(*core1_code)"
)

mcux_add_macro(
    CC "-D__SEMIHOST_HARDFAULT_DISABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS armgcc
    TARGETS debug
    INCLUDES examples/_boards/evkmimxrt1160/demo_apps/tee_fault/core0/../core1/armgcc/debug
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug
    INCLUDES examples/_boards/evkmimxrt1160/demo_apps/tee_fault/core0/../core1/armgcc/debug
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS armgcc
    TARGETS release
    INCLUDES examples/_boards/evkmimxrt1160/demo_apps/tee_fault/core0/../core1/armgcc/release
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_release
    INCLUDES examples/_boards/evkmimxrt1160/demo_apps/tee_fault/core0/../core1/armgcc/release
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS mdk
    TARGETS debug
    INCLUDES examples/_boards/evkmimxrt1160/demo_apps/tee_fault/core0/../core1/mdk/debug
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS mdk
    TARGETS flexspi_nor_debug
    INCLUDES examples/_boards/evkmimxrt1160/demo_apps/tee_fault/core0/../core1/mdk/debug
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS mdk
    TARGETS release
    INCLUDES examples/_boards/evkmimxrt1160/demo_apps/tee_fault/core0/../core1/mdk/release
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS mdk
    TARGETS flexspi_nor_release
    INCLUDES examples/_boards/evkmimxrt1160/demo_apps/tee_fault/core0/../core1/mdk/release
)
