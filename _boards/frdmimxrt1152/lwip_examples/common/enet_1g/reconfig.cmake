mcux_add_macro(
  CC
  "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 -DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_add_source(
  BASE_PATH ${SdkRootDirPath} SOURCES
  ${board_root}/${board}/lwip_examples/common/enet_1g/hardware_init.c
  ${board_root}/${board}/lwip_examples/common/enet_1g/app.h)

mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES
                 ${board_root}/${board}/lwip_examples/common/enet_1g)
