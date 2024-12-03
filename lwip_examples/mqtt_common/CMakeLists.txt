mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/lwip_examples/mqtt_common
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/lwip_examples/mqtt_common/mqtt_freertos.c
        examples/lwip_examples/mqtt_common/mqtt_freertos.h
)