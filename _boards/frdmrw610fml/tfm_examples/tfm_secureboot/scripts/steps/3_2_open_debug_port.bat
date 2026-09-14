@echo off
@rem - The script to open debug port -

@rem -- Set variables --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

@echo ### Debug Authentication ###
 
@echo nxpdebugmbox %interface% --operation-timeout 20000 --protocol 2.0 %JTAG% auth --beacon 1 --certificate "debug_auth\debug_auth_cert.dc" --key "keys\debug_authentication_key_ECC_256.pem"
call "%nxpdebugmbox%" %interface% -v --operation-timeout 20000 --protocol 2.0 %JTAG% auth --beacon 1 --certificate "debug_auth\debug_auth_cert.dc" --key "keys\debug_authentication_key_ECC_256.pem"
if errorlevel 1 goto ERROR

call "%nxpdebugmbox%" %interface% %JTAG% test-connection
if errorlevel 1 goto ERROR

@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2

