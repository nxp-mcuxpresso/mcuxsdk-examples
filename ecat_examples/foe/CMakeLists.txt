
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ecat_foe LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ecat_examples/foe)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ecat_examples/foe/foe.c
    SOURCES examples/ecat_examples/foe/ecat_hw.c
    SOURCES examples/ecat_examples/foe/ecat_hw.h
    SOURCES examples/ecat_examples/foe/bootutil_port/board_specific/flash_partitioning.c
    SOURCES examples/ecat_examples/foe/bootutil_port/board_specific/flash_partitioning.h
    SOURCES examples/ecat_examples/foe/bootutil_port/board_specific/sblconfig.h
    SOURCES examples/ecat_examples/foe/foe_support.c
    SOURCES examples/ecat_examples/foe/foe_support.h
    SOURCES examples/ecat_examples/foe/SSC/foe.xlsx
    SOURCES examples/ecat_examples/foe/SSC/foe.xml
    SOURCES examples/ecat_examples/foe/SSC/Src/applInterface.h
    SOURCES examples/ecat_examples/foe/SSC/Src/bootmode.c
    SOURCES examples/ecat_examples/foe/SSC/Src/bootmode.h
    SOURCES examples/ecat_examples/foe/SSC/Src/coeappl.c
    SOURCES examples/ecat_examples/foe/SSC/Src/coeappl.h
    SOURCES examples/ecat_examples/foe/SSC/Src/ecat_def.h
    SOURCES examples/ecat_examples/foe/SSC/Src/ecatappl.c
    SOURCES examples/ecat_examples/foe/SSC/Src/ecatappl.h
    SOURCES examples/ecat_examples/foe/SSC/Src/ecatcoe.c
    SOURCES examples/ecat_examples/foe/SSC/Src/ecatcoe.h
    SOURCES examples/ecat_examples/foe/SSC/Src/ecatfoe.c
    SOURCES examples/ecat_examples/foe/SSC/Src/ecatfoe.h
    SOURCES examples/ecat_examples/foe/SSC/Src/ecatslv.c
    SOURCES examples/ecat_examples/foe/SSC/Src/ecatslv.h
    SOURCES examples/ecat_examples/foe/SSC/Src/esc.h
    SOURCES examples/ecat_examples/foe/SSC/Src/foe.h
    SOURCES examples/ecat_examples/foe/SSC/Src/foeappl.c
    SOURCES examples/ecat_examples/foe/SSC/Src/foeappl.h
    SOURCES examples/ecat_examples/foe/SSC/Src/foeObjects.h
    SOURCES examples/ecat_examples/foe/SSC/Src/mailbox.c
    SOURCES examples/ecat_examples/foe/SSC/Src/mailbox.h
    SOURCES examples/ecat_examples/foe/SSC/Src/objdef.c
    SOURCES examples/ecat_examples/foe/SSC/Src/objdef.h
    SOURCES examples/ecat_examples/foe/SSC/Src/sdoserv.c
    SOURCES examples/ecat_examples/foe/SSC/Src/sdoserv.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ecat_examples/foe
    INCLUDES examples/ecat_examples/foe/bootutil_port
    INCLUDES examples/ecat_examples/foe/bootutil_port/board_specific
    INCLUDES examples/ecat_examples/foe/bootutil_port/mcuboot_config
    INCLUDES examples/ecat_examples/foe/bootutil_port/sysflash
    INCLUDES examples/ecat_examples/foe/SSC
    INCLUDES examples/ecat_examples/foe/SSC/Src
)

include(${SdkRootDirPath}/examples/_boards/${board}/ecat_examples/foe/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)