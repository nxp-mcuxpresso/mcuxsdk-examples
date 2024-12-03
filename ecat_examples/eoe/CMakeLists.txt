
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ecat_eoe LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ecat_examples/eoe)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ecat_examples/eoe/eoe_freertos.c
    SOURCES examples/ecat_examples/eoe/ecat_hw.c
    SOURCES examples/ecat_examples/eoe/ecat_hw.h
    SOURCES examples/ecat_examples/eoe/eoe_lwip_interface.c
    SOURCES examples/ecat_examples/eoe/eoe_lwip_interface.h
    SOURCES examples/ecat_examples/eoe/FreeRTOSConfig.h
    SOURCES examples/ecat_examples/eoe/httpsrv_freertos.c
    SOURCES examples/ecat_examples/eoe/httpsrv_freertos.h
    SOURCES examples/ecat_examples/eoe/httpsrv_fs_data.c
    SOURCES examples/ecat_examples/eoe/lwipopts.h
    SOURCES examples/ecat_examples/eoe/webpage/auth.html
    SOURCES examples/ecat_examples/eoe/webpage/cgi.html
    SOURCES examples/ecat_examples/eoe/webpage/favicon.ico
    SOURCES examples/ecat_examples/eoe/webpage/httpsrv.css
    SOURCES examples/ecat_examples/eoe/webpage/index.html
    SOURCES examples/ecat_examples/eoe/webpage/NXP_logo.png
    SOURCES examples/ecat_examples/eoe/webpage/poll.html
    SOURCES examples/ecat_examples/eoe/webpage/request.js
    SOURCES examples/ecat_examples/eoe/webpage/ssi.shtml
    SOURCES examples/ecat_examples/eoe/webpage/websocket.html
    SOURCES examples/ecat_examples/eoe/webpage/welcome.html
    SOURCES examples/ecat_examples/eoe/SSC/eoe_freertos.xlsx
    SOURCES examples/ecat_examples/eoe/SSC/eoe.xml
    SOURCES examples/ecat_examples/eoe/SSC/Src/applInterface.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/coeappl.c
    SOURCES examples/ecat_examples/eoe/SSC/Src/coeappl.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/ecat_def.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/ecatappl.c
    SOURCES examples/ecat_examples/eoe/SSC/Src/ecatappl.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/ecatcoe.c
    SOURCES examples/ecat_examples/eoe/SSC/Src/ecatcoe.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/ecateoe.c
    SOURCES examples/ecat_examples/eoe/SSC/Src/ecateoe.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/ecatslv.c
    SOURCES examples/ecat_examples/eoe/SSC/Src/ecatslv.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/eoe_freertos.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/eoe_freertosObjects.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/eoeappl.c
    SOURCES examples/ecat_examples/eoe/SSC/Src/eoeappl.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/esc.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/mailbox.c
    SOURCES examples/ecat_examples/eoe/SSC/Src/mailbox.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/objdef.c
    SOURCES examples/ecat_examples/eoe/SSC/Src/objdef.h
    SOURCES examples/ecat_examples/eoe/SSC/Src/sdoserv.c
    SOURCES examples/ecat_examples/eoe/SSC/Src/sdoserv.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ecat_examples/eoe
    INCLUDES examples/ecat_examples/eoe/webpage
    INCLUDES examples/ecat_examples/eoe/SSC
    INCLUDES examples/ecat_examples/eoe/SSC/Src
)

include(${SdkRootDirPath}/examples/_boards/${board}/ecat_examples/eoe/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)