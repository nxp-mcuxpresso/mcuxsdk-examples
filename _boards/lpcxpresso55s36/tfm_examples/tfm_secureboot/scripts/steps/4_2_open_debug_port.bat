@echo off
@rem - The script to open debug port -

@rem -- Set variables --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

@echo ### Debug Authentication ###

@echo nxpdebugmbox --protocol 2.1 auth --beacon 1 --certificate "debug_auth\debug_auth_cert.dc" --key "keys\debug_authentication_key_ECC_384.pem"
call "%nxpdebugmbox%" --protocol 2.1 auth --beacon 1 --certificate "debug_auth\debug_auth_cert.dc" --key "keys\debug_authentication_key_ECC_384.pem"
if errorlevel 1 goto ERROR

call "%nxpdebugmbox%" test-connection
if errorlevel 1 goto ERROR

@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2

