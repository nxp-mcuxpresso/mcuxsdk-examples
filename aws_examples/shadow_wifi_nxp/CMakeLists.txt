
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(aws_shadow_wifi_nxp LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/aws_examples/shadow_wifi_nxp)

include(${SdkRootDirPath}/CMakeLists.txt)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/wifi_examples/common/app_config/app_config.h
            examples/aws_examples/common/aws_mbedtls_config.h
            examples/aws_examples/common/config_files/aws_clientcredential.h
            examples/aws_examples/common/config_files/aws_clientcredential_keys.h
            examples/aws_examples/common/config_files/core_mqtt_config.h
            examples/aws_examples/common/config_files/core_pkcs11_config.h
            examples/aws_examples/common/config_files/demo_config.h
            examples/aws_examples/common/config_files/freertos_tasks_c_additions.h
            examples/aws_examples/common/config_files/kvstore_config.h
            examples/aws_examples/common/config_files/shadow_config.h
            examples/aws_examples/common/config_files_wifi_nxp/wifi_config.h
            examples/aws_examples/common/dev_mode_key_provisioning/include/aws_dev_mode_key_provisioning.h
            examples/aws_examples/common/dev_mode_key_provisioning/src/aws_dev_mode_key_provisioning.c
            examples/aws_examples/common/lwipopts_wifi_nxp/lwipopts.h
            examples/aws_examples/common/pkcs11/iot_pkcs11_pal.c
            examples/aws_examples/common/shadow_tasks/shadow_device_task.c
            examples/aws_examples/common/shadow_tasks/shadow_update_task.c
            examples/aws_examples/shadow_wifi_nxp/main_wifi.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/wifi_examples/common/app_config
             examples/aws_examples/common
             examples/aws_examples/common/config_files
             examples/aws_examples/common/config_files_wifi_nxp
             examples/aws_examples/common/dev_mode_key_provisioning/include
             examples/aws_examples/common/lwipopts_wifi_nxp
)


# Add additional configuration
mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DDISABLE_LOGGING"
)

include(${SdkRootDirPath}/examples/_boards/${board}/aws_examples/shadow_wifi_nxp/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)