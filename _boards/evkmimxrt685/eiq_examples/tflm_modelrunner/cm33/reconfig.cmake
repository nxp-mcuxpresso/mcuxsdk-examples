mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES examples/eiq_examples/common/
  )
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/common/timer.c
          examples/eiq_examples/common/timer.h
  )
mcux_add_mdk_configuration(
        CX "-std=gnu++17"
)

