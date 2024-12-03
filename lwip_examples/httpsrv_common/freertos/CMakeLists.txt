mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/lwip_examples/httpsrv_common/freertos
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/lwip_examples/httpsrv_common/freertos/httpsrv_freertos.c
        examples/lwip_examples/httpsrv_common/freertos/httpsrv_freertos.h
)