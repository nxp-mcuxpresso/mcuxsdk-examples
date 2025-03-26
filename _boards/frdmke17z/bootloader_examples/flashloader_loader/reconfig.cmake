
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/mcu_bootloader/targets/MKE17Z7/src/bootloader_config.h
            middleware/mcu_bootloader/targets/MKE17Z7/src/target_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/${board}/bootloader_examples/flashloader_loader/../flashloader/iar
             examples/${board}/bootloader_examples/flashloader_loader/../flashloader/mdk
             examples/${board}/bootloader_examples/flashloader_loader/../flashloader/armgcc
             middleware/mcu_bootloader/src
             src
             examples/${board}/src
             middleware/mcu_bootloader/targets/MKE17Z7/src
)

mcux_add_macro(
    TOOLCHAINS mdk
    AS "-DBL_HAS_BOOTLOADER_CONFIG=0"
)
mcux_add_macro(
    CC "-DBL_TARGET_FLASH\
       -DFRDM_KE17Z"
    AS "-DLITE_FOOTPRINT"
)
mcux_remove_iar_configuration(
    CC  "-DDEBUG"
    TARGETS release
)
mcux_remove_iar_configuration(
    CC  "-DNDEBUG"
    TARGETS debug
)
mcux_remove_mdk_configuration(
    CC  "-DDEBUG"
    TARGETS release
)
mcux_remove_mdk_configuration(
    CC  "-DNDEBUG"
    TARGETS debug
)
mcux_remove_armgcc_configuration(
    CC  "-DDEBUG"
    TARGETS release
)
mcux_remove_armgcc_configuration(
    CC  "-DNDEBUG"
    TARGETS debug
)
mcux_add_custom_command(
        TARGETS release debug
        TOOLCHAINS iar 
        BUILD_EVENT PRE_BUILD
        BUILD_COMMAND ielftool --bin ../../flashloader/iar/$CONFIG_NAME$/flashloader.out ../../flashloader/iar/flashloader.bin && python ../../flashloader/create_fl_image.py ../../flashloader/iar/$CONFIG_NAME$/flashloader.out ../../flashloader/iar/flashloader.bin ../../flashloader/iar/$CONFIG_NAME$/flashloader_image.c
)
mcux_add_custom_command(
        TARGETS release debug
        TOOLCHAINS armgcc
        BUILD_EVENT PRE_BUILD
        BUILD_COMMAND python ${ProjDirPath}/../../flashloader/create_fl_image_mcux.py ${ProjDirPath}/../../flashloader/armgcc/${CMAKE_BUILD_TYPE}/flashloader.elf ${ProjDirPath}/../../flashloader/armgcc/${CMAKE_BUILD_TYPE}/flashloader.bin ${ProjDirPath}/../../flashloader/armgcc/${CMAKE_BUILD_TYPE}/flashloader_image.c
)
mcux_add_custom_command(
        TARGETS debug
        TOOLCHAINS mdk
        BUILD_EVENT PRE_BUILD
        BUILD_COMMAND python ../../flashloader/create_fl_image_mdk.py ../../flashloader/mdk/debug/flashloader.out ../../flashloader/mdk/debug/flashloader.bin ../../flashloader/mdk/debug/flashloader_image.c
)
mcux_add_custom_command(
        TARGETS release
        TOOLCHAINS mdk
        BUILD_EVENT PRE_BUILD
        BUILD_COMMAND python ../../flashloader/create_fl_image_mdk.py ../../flashloader/mdk/release/flashloader.out ../../flashloader/mdk/release/flashloader.bin ../../flashloader/mdk/release/flashloader_image.c
)