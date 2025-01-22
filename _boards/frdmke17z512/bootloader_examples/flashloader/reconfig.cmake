
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/mcu_bootloader/bin/create_flashloader_image.bat
            middleware/mcu_bootloader/bin/create_fl_image.py
            middleware/mcu_bootloader/bin/elf.py
            middleware/mcu_bootloader/bin/create_fl_image_mdk.py
            middleware/mcu_bootloader/bin/create_fl_image_mcux.py
            middleware/mcu_bootloader/boards/${board}/bootloader/pin_mux.c
            middleware/mcu_bootloader/boards/${board}/bootloader/pin_mux.h
            middleware/mcu_bootloader/boards/${board}/bootloader/clock_config.c
            middleware/mcu_bootloader/boards/${board}/bootloader/clock_config.h
)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/mcu_bootloader/boards/${board}/flashloader
             middleware/mcu_bootloader/boards/${board}/bootloader
)
mcux_add_macro(
    TOOLCHAINS mdk
    AS "-DBL_HAS_BOOTLOADER_CONFIG=0"
)
mcux_add_macro(
    CC "-DBL_TARGET_RAM\
       -DFRDM_KE17Z9\
       -DFSL_OSA_BM_TIMER_CONFIG=FSL_OSA_BM_TIMER_NONE"
)
mcux_remove_mdk_configuration(
    LD "--keep=*(.FlashConfig)"
)
mcux_add_custom_command(
        TARGETS release debug
        TOOLCHAINS iar 
        BUILD_EVENT POST_BUILD
        BUILD_COMMAND ielftool --bin $CONFIG_NAME$/flashloader.out flashloader.bin && python ../create_fl_image.py $CONFIG_NAME$/flashloader.out flashloader.bin $CONFIG_NAME$/flashloader_image.c
)

# mcux_add_custom_command(
        # TARGETS release debug
        # TOOLCHAINS iar 
        # BUILD_EVENT POST_BUILD
        # BUILD_COMMAND ielftool --bin $$CONFIG_NAME$$/flashloader.out flashloader.bin && python ../create_fl_image.py $$CONFIG_NAME$$/flashloader.out flashloader.bin $$CONFIG_NAME$$/flashloader_image.c
# )
mcux_add_custom_command(
        TARGETS release debug
        TOOLCHAINS armgcc
        BUILD_EVENT POST_BUILD
        BUILD_COMMAND - python ${ProjDirPath}/../create_fl_image_mcux.py ${EXECUTABLE_OUTPUT_PATH}/flashloader.elf
            ${EXECUTABLE_OUTPUT_PATH}/flashloader.bin ${EXECUTABLE_OUTPUT_PATH}/flashloader_image.c
)
mcux_add_custom_command(
        TARGETS debug
        TOOLCHAINS mdk
        BUILD_EVENT POST_BUILD
        BUILD_COMMAND fromelf --bincombined debug/flashloader.out --output debug/flashloader.bin
)
mcux_add_custom_command(
        TARGETS debug
        TOOLCHAINS mdk
        BUILD_EVENT POST_BUILD
        BUILD_COMMAND python ../create_fl_image_mdk.py debug/flashloader.out debug/flashloader.bin debug/flashloader_image.c
)
mcux_add_custom_command(
        TARGETS release
        TOOLCHAINS mdk
        BUILD_EVENT POST_BUILD
        BUILD_COMMAND fromelf --bincombined release/flashloader.out --output release/flashloader.bin
)
mcux_add_custom_command(
        TARGETS release
        TOOLCHAINS mdk
        BUILD_EVENT POST_BUILD
        BUILD_COMMAND python ../create_fl_image_mdk.py release/flashloader.out release/flashloader.bin release/flashloader_image.c
)
mcux_add_custom_command(
        TARGETS release debug
        TOOLCHAINS mcux
        BUILD_EVENT POST_BUILD
        BUILD_COMMAND arm-none-eabi-objcopy -I elf32-littlearm -O binary "${ProjName}.axf"
            "${ProjName}.bin"; python ../source/create_fl_image_mcux.py "${ProjName}.axf"
            "${ProjName}.bin" "flashloader_image.c"; mkdir -p "../../${ProjName}_loader/Release/";
            cp "flashloader_image.c" "../../${ProjName}_loader/Release/"; mkdir -p
            "../../${ProjName}_loader/Debug/"; cp "flashloader_image.c" "../../${ProjName}_loader/Debug/"
)