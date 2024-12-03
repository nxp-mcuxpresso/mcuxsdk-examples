
mcux_add_iar_configuration(
    CC "--diag_suppress=Pa082,Pa050,Pe188,Pa089,Pe546,Pa084"
    CX "--diag_suppress=Pa082,Pa050,Pe188,Pa089,Pe546,Pe167,Pe144,Pa183,Pa084"
)

mcux_add_mdk_configuration(
    TARGETS debug release
    CC "-O0"
    CX "-O0"
    LD "--legacyalign\
       --diag_suppress=3912\
       --diag_suppress=6329"
)

mcux_add_macro(
    CC "-DBOARD_USE_VIRTUALCOM=1\
        -DLPC_55x=1\
        -DSDK_DEBUGCONSOLE_UART=1\
        -DUSB_STACK_BM\
        -DUSB_ISR_REDIRECT\
        -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
        -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\"\
        -DMBEDTLS_USER_CONFIG_FILE=\\\"sss_ksdk_mbedtls_config.h\\\"\
        -DPRINTF_ADVANCED_ENABLE=1\
        -DXIP_EXTERNAL_FLASH=1\
        -DXIP_BOOT_HEADER_ENABLE=1\
        -DSDK_OS_BAREMETAL\
        -DT1oI2C\
        -DT1oI2C_UM11225\
        -DNO_SECURE_CHANNEL_SUPPORT\
        -DCR_INTEGER_PRINTF\
        -D__USE_CMSIS\
        -DSDK_DEBUGCONSOLE=1"
)

mcux_remove_iar_configuration(
    CC "--diag_suppress=Pa082,Pa050"
    CX "--diag_suppress=Pa082,Pa050"
)

mcux_remove_mdk_configuration(
    TARGETS debug
    CC "-O1"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x4000 \
        -Xlinker --defsym=__heap_size__=0x4000"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x4000 \
       --config_def=__heap_size__=0x4000"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x4000\" \
        --predefine=\"-D__heap_size__=0x4000\""
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/se_hostlib/hostlib/hostLib/vcom/ksdk/usb_device_descriptor.c"
    "middleware/se_hostlib/hostlib/hostLib/vcom/ksdk/usb_device_descriptor.h"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
            "middleware/usb/device/class"
)
