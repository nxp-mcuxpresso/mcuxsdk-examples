mcux_add_source(
  BASE_PATH ${SdkRootDirPath} SOURCES
  ${board_root}/${board}/driver_examples/enet/common/hardware_init.c
  ${board_root}/${board}/driver_examples/enet/common/app.h)

mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES
                 ${board_root}/${board}/driver_examples/enet/common)

mcux_add_linker_symbol(
  SYMBOLS "__heap_size__=0x3000\
             __stack_size__=0x3000\
             __ram_vector_table__=1")
