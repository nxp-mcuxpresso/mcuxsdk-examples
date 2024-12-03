# edgefast
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    PREINCLUDE TRUE
    SOURCES examples/_boards/${board}/coex_examples/coex_zigbee/edgefast/edgefast_bluetooth_app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/coex_examples/coex_zigbee/edgefast
)

# monolithihc feature config
mcux_add_macro(
  CC "-DCONFIG_MONOLITHIC_BLE_15_4=1\
      -DCONFIG_MONOLITHIC_BLE=1"
)

# edgefast config
mcux_add_macro(
  CC "-DBT_PLATFORM\
      -DEDGEFAST_BT_LITTLEFS_MFLASH\
      -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0\
      -DFSL_FEATURE_FLASH_PAGE_SIZE_BYTES=4096\
      -DGATT_CLIENT\
      -DGATT_DB\
      -DHAL_AUDIO_DMA_INIT_ENABLE=0\
      -DgMemManagerLight=0\
      -DgPlatformDisableBleLowPower_d=1\
      -DSHELL_TASK_STACK_SIZE=6144\
      -DSHELL_TASK_PRIORITY=1 \
      -DLPUART_RING_BUFFER_SIZE=1024U"
)
