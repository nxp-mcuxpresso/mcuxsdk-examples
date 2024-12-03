
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ecat_digital_io LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ecat_examples/digital_io)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ecat_examples/digital_io/digital_io.c
    SOURCES examples/ecat_examples/digital_io/ecat_hw.c
    SOURCES examples/ecat_examples/digital_io/ecat_hw.h
    SOURCES examples/ecat_examples/digital_io/SSC/digital_io.xlsx
    SOURCES examples/ecat_examples/digital_io/SSC/digital_io.xml
    SOURCES examples/ecat_examples/digital_io/SSC/Src/applInterface.h
    SOURCES examples/ecat_examples/digital_io/SSC/Src/coeappl.c
    SOURCES examples/ecat_examples/digital_io/SSC/Src/coeappl.h
    SOURCES examples/ecat_examples/digital_io/SSC/Src/digital_io.h
    SOURCES examples/ecat_examples/digital_io/SSC/Src/digital_ioObjects.h
    SOURCES examples/ecat_examples/digital_io/SSC/Src/ecat_def.h
    SOURCES examples/ecat_examples/digital_io/SSC/Src/ecatappl.c
    SOURCES examples/ecat_examples/digital_io/SSC/Src/ecatappl.h
    SOURCES examples/ecat_examples/digital_io/SSC/Src/ecatcoe.c
    SOURCES examples/ecat_examples/digital_io/SSC/Src/ecatcoe.h
    SOURCES examples/ecat_examples/digital_io/SSC/Src/ecatslv.c
    SOURCES examples/ecat_examples/digital_io/SSC/Src/ecatslv.h
    SOURCES examples/ecat_examples/digital_io/SSC/Src/esc.h
    SOURCES examples/ecat_examples/digital_io/SSC/Src/mailbox.c
    SOURCES examples/ecat_examples/digital_io/SSC/Src/mailbox.h
    SOURCES examples/ecat_examples/digital_io/SSC/Src/objdef.c
    SOURCES examples/ecat_examples/digital_io/SSC/Src/objdef.h
    SOURCES examples/ecat_examples/digital_io/SSC/Src/sdoserv.c
    SOURCES examples/ecat_examples/digital_io/SSC/Src/sdoserv.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ecat_examples/digital_io
    INCLUDES examples/ecat_examples/digital_io/SSC
    INCLUDES examples/ecat_examples/digital_io/SSC/Src
)

include(${SdkRootDirPath}/examples/_boards/${board}/ecat_examples/digital_io/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)