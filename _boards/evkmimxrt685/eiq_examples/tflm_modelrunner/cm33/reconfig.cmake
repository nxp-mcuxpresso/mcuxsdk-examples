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
mcux_remove_mdk_configuration(
  CC "-fshort-wchar"
  CX "-fshort-wchar"
  )
mcux_add_macro(
        CX "MODEL_SIZE=1024*1024"
        CC "PRINTF_ADVANCED_ENABLE=1 \
            PRINTF_FLOAT_ENABLE=1"
)
