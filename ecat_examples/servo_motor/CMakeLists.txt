
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ecat_servo_motor LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ecat_examples/servo_motor)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ecat_examples/servo_motor/servo_motor.c
    SOURCES examples/ecat_examples/servo_motor/servo_motor.h
    SOURCES examples/ecat_examples/servo_motor/ecat_hw.c
    SOURCES examples/ecat_examples/servo_motor/ecat_hw.h
    SOURCES examples/ecat_examples/servo_motor/freemaster_cfg.h
    SOURCES examples/ecat_examples/servo_motor/m1_pmsm_appconfig.h
    SOURCES examples/ecat_examples/servo_motor/m2_pmsm_appconfig.h
    SOURCES examples/ecat_examples/servo_motor/mc_periph_init.c
    SOURCES examples/ecat_examples/servo_motor/mc_periph_init.h
    SOURCES examples/ecat_examples/servo_motor/peripherals.c
    SOURCES examples/ecat_examples/servo_motor/peripherals.h
    SOURCES examples/ecat_examples/servo_motor/SSC/CiA402-combine-the-SSC-slave-with-ec_pmsm-support.patch
    SOURCES examples/ecat_examples/servo_motor/SSC/servo_motor.xml
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/applInterface.h
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/cia402appl.c
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/cia402appl.h
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/coeappl.c
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/coeappl.h
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/ecat_def.h
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/ecatappl.c
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/ecatappl.h
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/ecatcoe.c
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/ecatcoe.h
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/ecatslv.c
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/ecatslv.h
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/esc.h
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/mailbox.c
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/mailbox.h
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/objdef.c
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/objdef.h
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/sdoserv.c
    SOURCES examples/ecat_examples/servo_motor/SSC/Src/sdoserv.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ecat_examples/servo_motor
    INCLUDES examples/ecat_examples/servo_motor/SSC
    INCLUDES examples/ecat_examples/servo_motor/SSC/Src
)

include(${SdkRootDirPath}/examples/_boards/${board}/ecat_examples/servo_motor/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)