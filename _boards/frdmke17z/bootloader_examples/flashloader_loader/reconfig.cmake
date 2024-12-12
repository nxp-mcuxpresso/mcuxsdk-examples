
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
