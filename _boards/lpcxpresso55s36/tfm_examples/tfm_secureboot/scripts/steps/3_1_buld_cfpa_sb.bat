@echo off
@rem -- The script for building of the CFPA SB file. --

@rem -- Set variables. --
call ..\config.cmd
if errorlevel 1 goto ERROR

pushd "%SPT_WORKSPACE%"

@echo ### Create CFPA binary ###
@echo pfr generate-binary -c configs\cfpa.json -o configs\cfpa.bin
"%pfr%" generate-binary -c "configs\cfpa.json" -o "configs\cfpa.bin"
if errorlevel 1 goto ERROR

@echo ### Create CFPA SB file ###
@echo nxpimage sb31 export configs\sb3_cfpa_config.json
"%nxpimage%" sb31 export "configs\sb3_cfpa_config.json"
if errorlevel 1 goto ERROR

@echo SUCCESS
    popd
    if not defined NO_PAUSE (pause)
    exit /b 0
:ERROR
    popd
    if not defined NO_PAUSE (pause)
    exit /b 2
