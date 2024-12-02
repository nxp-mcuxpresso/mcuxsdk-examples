@echo off
@rem Environment variables - absolute paths simplifying re-use of the script on another machine
@rem  - Absolute path to SPT workspace (it is recommended to copy/move all user files in this workspace)
if "%SPT_WORKSPACE%"=="" (
  SET "SPT_WORKSPACE=C:\_ddm\KSDK\sdk_latest\mcu-sdk-2.0\boards\lpcxpresso55s36\tfm_examples\tfm_secureboot\script\sec_workspace"
)
@rem  - Absolute path to SPT installation directory
if "%SPT_INSTALL_BIN%"=="" (
  SET "SPT_INSTALL_BIN=C:\nxp\MCUX_Provi_v5\bin"
)

chcp 65001 >nul
SET "nxpkeygen=C:\nxp\MCUX_Provi_v5\bin\tools\spsdk\nxpkeygen"
@rem Ensure target directories exists
if not exist "%SPT_WORKSPACE%\keys\." (
    mkdir "%SPT_WORKSPACE%\keys"
    if errorlevel 1 exit 2
)
@rem Switch to working directory
pushd "%SPT_WORKSPACE%\keys"
@rem Log tools info
if not exist spt_tools_versions.txt (
    echo This file contains version of the tools used for keys generation: > spt_tools_versions.txt
    echo. >> spt_tools_versions.txt
    echo Key set generated using: >> spt_tools_versions.txt
) else (
    echo. >> spt_tools_versions.txt
    echo Key added using: >> spt_tools_versions.txt
)
echo MCUX Secure Provisioning Tool version 5.0.b220607 >> spt_tools_versions.txt
"%nxpkeygen%" --version >> spt_tools_versions.txt
@rem Generate keys

@echo Generate key: ROT1_p384
"%nxpkeygen%" -k secp384r1 "%SPT_WORKSPACE%\keys\ROT1_p384.pem"
if errorlevel 1 exit 2
@echo Generate key: ROT2_p384
"%nxpkeygen%" -k secp384r1 "%SPT_WORKSPACE%\keys\ROT2_p384.pem"
if errorlevel 1 exit 2
@echo Generate key: ROT3_p384
"%nxpkeygen%" -k secp384r1 "%SPT_WORKSPACE%\keys\ROT3_p384.pem"
if errorlevel 1 exit 2
@echo Generate key: ROT4_p384
"%nxpkeygen%" -k secp384r1 "%SPT_WORKSPACE%\keys\ROT4_p384.pem"
if errorlevel 1 exit 2
popd