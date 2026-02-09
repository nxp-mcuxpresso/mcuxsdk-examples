cmake_minimum_required(VERSION 3.22.0)

set(CONF_FILE "${CMAKE_CURRENT_LIST_DIR}/prj.conf")
if (ISPSDK_ENABLE_ETH)
    set(CONF_FILE "${CONF_FILE};${CMAKE_CURRENT_LIST_DIR}/prj_eth.conf")
    message(STATUS "Ethernet configuration enabled for isp_ccl (${CONF_FILE})")
endif()
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/prj_local.conf")
    set(CONF_FILE "${CONF_FILE};${CMAKE_CURRENT_LIST_DIR}/prj_local.conf")
    message(STATUS "Found local project override, applying... (${CONF_FILE})")
endif()

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(isp_ccl LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/isp_ccl)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
    examples/demo_apps/isp_ccl/isp_ccl.c
)

mcux_add_include(INCLUDES
    ./
)

mcux_add_configuration(CC "-DDEBUG_PRINT_ENABLE=0 -DPRINTF_ADVANCED_ENABLE=1 -DPRINTF_FLOAT_ENABLE=1")

# add debug symbols into .elf file so we can read RTOS statistics even in release
# mcux_add_configuration(CC "-g")
# target_link_options(${MCUX_SDK_PROJECT_NAME} PRIVATE -g)

# solve hard fault for unaligned access - this should not be needed as MPU sets access to "code, data and DDR" as normal memory which should not have align problems
mcux_add_configuration(CC "-mno-unaligned-access")
target_link_options(${MCUX_SDK_PROJECT_NAME} PRIVATE -mno-unaligned-access)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/isp_ccl/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

if (CONFIG_MCUX_COMPONENT_middleware.ispsdk.gcc_coverage)
    mcux_add_configuration(CC "-D__GCC_COVERAGE__")

    if (CONFIG_MCUX_COMPONENT_middleware.ispsdk.gcc_coverage.select_files)
        # CC flags to be added to files selected for coverage
        set(COVERAGE_CC_FLAGS "--coverage")

        # add linker options - this will add libgcov to the project and do some other coverage magic during linking
        target_link_options(${MCUX_SDK_PROJECT_NAME} PRIVATE --coverage)

        # this optional command directs gcc to place some of the variables into different segment - needs linker script support for the actual placement:
        #   .gcov_info :
        #   {
        #       . = ALIGN(4);
        #       PROVIDE (__gcov_info_start = .);
        #       KEEP (*(.gcov_info))
        #       PROVIDE (__gcov_info_end = .);
        #       . = ALIGN(4);
        #   } > m_data_ddr
        if (CONFIG_MCUX_COMPONENT_middleware.ispsdk.gcc_coverage.override_placement)
            target_link_options(${MCUX_SDK_PROJECT_NAME} PRIVATE -fprofile-info-section)
            list(APPEND COVERAGE_CC_FLAGS "-fprofile-info-section")
        endif()

        # separate single line into multiple items
        separate_arguments(PATH_FILTERS NATIVE_COMMAND "${CONFIG_MCUX_COMPONENT_middleware.ispsdk.gcc_coverage.file_filters}")
        # get list of all files
        get_target_property(ALL_FILES ${MCUX_SDK_PROJECT_NAME} SOURCES)

        foreach(source_file IN LISTS ALL_FILES)
            # Convert to an absolute path for reliable filtering
            get_filename_component(FILE_ABS "${source_file}" ABSOLUTE)

            foreach(filter IN LISTS PATH_FILTERS)
                # Do coverage only on files specified by filter
                if(FILE_ABS MATCHES ".*${filter}.*")
                    message (STATUS "GCC coverage enabled for: ${FILE_ABS}")
                    set_source_files_properties(${FILE_ABS} PROPERTIES COMPILE_OPTIONS "${COVERAGE_CC_FLAGS}")
                endif()
            endforeach()
        endforeach()
    else()
        # Apply coverage flags to all source files if no specific filters are defined
        message (STATUS "GCC coverage enabled for: All files")

        if (CONFIG_MCUX_COMPONENT_middleware.ispsdk.gcc_coverage.override_placement)
            mcux_add_configuration(CC "--coverage -fprofile-info-section" )
        else()
            mcux_add_configuration(CC "--coverage")
        endif()
    endif()
endif()
