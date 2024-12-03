mcux_set_variable(CONFIG_ZB_PLATFORM "RW612")

mcux_set_variable(OTA_VERSION_STRING "NXPZBED-----FRDMRW612-----------")
mcux_set_variable(NXP_SDK_MIDDLEWARE ${SdkRootDirPath}/middleware)
mcux_set_variable(MCUBOOT_IMGTOOL ${NXP_SDK_MIDDLEWARE}/mcuboot_opensource/scripts/imgtool.py)
mcux_set_variable(MCUBOOT_SIGNATURE_KEY_FILE "${NXP_SDK_MIDDLEWARE}/mcuboot_opensource/boot/nxp_mcux_sdk/keys/sign-rsa2048-priv.pem")

#zigbee applications defines
mcux_add_macro(
    APP_END_DEVICE_NODE_CLI
    gAppButtonCnt_c=1
    gAppLedCnt_c=2
    SERIAL_MANAGER_TASK_PRIORITY=8
    TM_TASK_PRIORITY=6
    OTA_IGNORE_CRC_CHECK=1
    CLD_OTA_KEY=\\\"${ENCRYPTION_KEY}\\\"
    CLD_OTA_MANF_ID_VALUE=${MANUFACTURER_CODE}
    INTERNAL_ENCRYPTED
    APP_OTA_OFFSET_WRITEN_BEFORE_LINKKEY_VERIFICATION=8128
    PRINTF_ADVANCED_ENABLE=1
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --sort-section=alignment"
)

mcux_add_linker_symbol(
    SYMBOLS
        "gUseNVMLink_d=1 \
        gNVMSectorCountLink_d=8"
)
mcux_remove_armgcc_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)

mcux_add_armgcc_linker_script(
    TARGETS debug release flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/wireless_examples
    LINKER linker/gcc/RW61x_flash_zb_mcuboot.ld
)

mcux_add_custom_command(
    BUILD_EVENT POST_BUILD
    BUILD_COMMAND
    COMMAND echo "Generating v1 elf:"
    COMMAND ${PYTHON_EXECUTABLE} ${OTA_TOOL}
    ARGS
        -log INFO
        --embed
        -i ${MCUX_SDK_PROJECT_NAME}.elf
        -o ${MCUX_SDK_PROJECT_NAME}_v1.elf
        -OtaMan \"${MANUFACTURER_CODE}\"
        -OtaImgTyp \"${OTA_IMAGE_TYPE}\"
        -OtaVersionStr \"${OTA_VERSION_STRING}\"
        -OtaFileVersion 1
        -c $ENV{ARMGCC_DIR}
    COMMAND echo "Generating v1 bin:"
    COMMAND ${CMAKE_OBJCOPY} -v -O binary ${MCUX_SDK_PROJECT_NAME}_v1.elf  ${MCUX_SDK_PROJECT_NAME}_v1.bin
    COMMAND echo "Generating v2 elf:"
    COMMAND ${PYTHON_EXECUTABLE} ${OTA_TOOL}
    ARGS
        -log INFO
        --embed
        -i ${MCUX_SDK_PROJECT_NAME}.elf
        -o ${MCUX_SDK_PROJECT_NAME}_v2.elf
        -OtaMan \"${MANUFACTURER_CODE}\"
        -OtaImgTyp \"${OTA_IMAGE_TYPE}\"
        -OtaVersionStr \"${OTA_VERSION_STRING}\"
        -OtaFileVersion 2
        -c $ENV{ARMGCC_DIR}
    COMMAND echo "Generating v2 bin:"
    COMMAND ${CMAKE_OBJCOPY} -v -O binary ${MCUX_SDK_PROJECT_NAME}_v2.elf  ${MCUX_SDK_PROJECT_NAME}_v2.bin
    COMMAND echo "Generating v3 elf:"
    COMMAND ${PYTHON_EXECUTABLE} ${OTA_TOOL}
    ARGS
        -log INFO
        --embed
        -i ${MCUX_SDK_PROJECT_NAME}.elf
        -o ${MCUX_SDK_PROJECT_NAME}_v3.elf
        -OtaMan \"${MANUFACTURER_CODE}\"
        -OtaImgTyp \"${OTA_IMAGE_TYPE}\"
        -OtaVersionStr \"${OTA_VERSION_STRING}\"
        -OtaFileVersion 3
        -c $ENV{ARMGCC_DIR}
    COMMAND echo "Generating v3 bin:"
    COMMAND ${CMAKE_OBJCOPY} -v -O binary ${MCUX_SDK_PROJECT_NAME}_v3.elf  ${MCUX_SDK_PROJECT_NAME}_v3.bin
    COMMAND echo "Signing and confirming v1 image:"
    COMMAND ${Python3_EXECUTABLE} ${MCUBOOT_IMGTOOL}
    ARGS
        sign
        --confirm
        --key ${MCUBOOT_SIGNATURE_KEY_FILE}
        --align 4
        --header-size 0x1000
        --pad-header
        --slot-size 0x440000
        --max-sectors 1088
        --version "1.0.0+0"
        ${MCUX_SDK_PROJECT_NAME}_v1.bin
        ${MCUX_SDK_PROJECT_NAME}_v1.signed.confirmed.bin
    COMMAND echo "Signing v2 image:"
    COMMAND ${Python3_EXECUTABLE} ${MCUBOOT_IMGTOOL}
    ARGS
        sign
        --key ${MCUBOOT_SIGNATURE_KEY_FILE}
        --align 4
        --header-size 0x1000
        --pad-header
        --slot-size 0x440000
        --max-sectors 1088
        --version "2.0.0+0"
        ${MCUX_SDK_PROJECT_NAME}_v2.bin
        ${MCUX_SDK_PROJECT_NAME}_v2.signed.bin
    COMMAND echo "Signing v3 image:"
    COMMAND ${Python3_EXECUTABLE} ${MCUBOOT_IMGTOOL}
    ARGS
        sign
        --key ${MCUBOOT_SIGNATURE_KEY_FILE}
        --align 4
        --header-size 0x1000
        --pad-header
        --slot-size 0x440000
        --max-sectors 1088
        --version "3.0.0+0"
        ${MCUX_SDK_PROJECT_NAME}_v3.bin
        ${MCUX_SDK_PROJECT_NAME}_v3.signed.bin
    COMMAND echo "Generating v2 OTA image:"
    COMMAND ${PYTHON_EXECUTABLE} ${OTA_TOOL}
    ARGS
        -log INFO
        -e
        -k ${ENCRYPTION_KEY}
        -i ${MCUX_SDK_PROJECT_NAME}_v2.elf
        --signed ${MCUX_SDK_PROJECT_NAME}_v2.signed.bin
        -o ${MCUX_SDK_PROJECT_NAME}_v2.ota.bin
        -OtaMan \"${MANUFACTURER_CODE}\"
        -OtaImgTyp \"${OTA_IMAGE_TYPE}\"
        -OtaVersionStr \"${OTA_VERSION_STRING}\"
        -OtaFileVersion 2
        -c $ENV{ARMGCC_DIR}
    COMMAND echo "Generating v3 OTA image:"
    COMMAND ${PYTHON_EXECUTABLE} ${OTA_TOOL}
    ARGS
        -log INFO
        -e
        -k ${ENCRYPTION_KEY}
        -i ${MCUX_SDK_PROJECT_NAME}_v3.elf
        --signed ${MCUX_SDK_PROJECT_NAME}_v3.signed.bin
        -o ${MCUX_SDK_PROJECT_NAME}_v3.ota.bin
        -OtaMan \"${MANUFACTURER_CODE}\"
        -OtaImgTyp \"${OTA_IMAGE_TYPE}\"
        -OtaVersionStr \"${OTA_VERSION_STRING}\"
        -OtaFileVersion 3
        -c $ENV{ARMGCC_DIR}
)
