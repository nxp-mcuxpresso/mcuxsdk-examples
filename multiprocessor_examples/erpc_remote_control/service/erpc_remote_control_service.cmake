#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

mcux_add_include(
    INCLUDES .
)
mcux_add_source(
    SOURCES erpc_remote_control_app_0_common.h
            erpc_remote_control_app_0_common.hpp
            erpc_remote_control_app_1_common.h
            erpc_remote_control_app_1_common.hpp
            erpc_remote_control_app_0_interface.hpp
            erpc_remote_control_app_0_interface.cpp
            erpc_remote_control_app_1_interface.hpp
            erpc_remote_control_app_1_interface.cpp
            erpc_remote_control_app_0_server.hpp
            erpc_remote_control_app_0_server.cpp
            c_erpc_remote_control_app_0_server.h
            c_erpc_remote_control_app_0_server.cpp
            erpc_remote_control_app_1_client.hpp
            erpc_remote_control_app_1_client.cpp
            c_erpc_remote_control_app_1_client.h
            c_erpc_remote_control_app_1_client.cpp
            erpc_remote_control.erpc
            __init__.py
            erpc_remote_control_app_0/*.*
            erpc_remote_control_app_1/*.*
)
