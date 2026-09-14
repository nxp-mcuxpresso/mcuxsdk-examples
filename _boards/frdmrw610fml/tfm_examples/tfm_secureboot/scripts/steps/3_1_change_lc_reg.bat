@echo off
@rem - The script to open debug port -

@rem -- Set variables --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

call "%nxpdebugmbox%" %interface% %JTAG% test-connection
if errorlevel 1 goto ERROR


@echo ### loading shadow register contents into device ram to simulate OTP behavior ###
"%shadowregs%" %interface% %JTAG% -dev rw61x  loadconfig -f "..\spt_workspace\debug_auth\filled_template_rw61x_shadow_config.yml"

@echo ### resetting the device ###
call "%nxpdebugmbox%" %interface% %JTAG% write-memory 0xe000ed0c "{{04 00 fa 05}}"

@echo SUCCESS
    popd
	if not defined NO_PAUSE (pause)
	exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2

