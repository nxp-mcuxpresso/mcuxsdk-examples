
@rem ============= Connection parameters =======================
@rem -- Connection parameter for blhost and nxpdevhsm. It should be in format "-p COMx[,baud]" for UART, or "-u VID,PID" for USB --
@rem -- USB --
@rem SET connect=-u 0x1FC9,0x0025
@rem -- COM --
SET COM_PORT=7,115200
SET connect=-p COM%COM_PORT%

@rem ============= Secure Provisioning Tool ====================
@rem  - Absolute path to SPT installation directory\
if "%SPT_INSTALL_BIN%"=="" (
    SET "SPT_INSTALL_BIN=C:\nxp\MCUX_Provi_v6\bin"
)

@rem ============= TFM example =================================
SET "TFM_SCRIPTS=%~dp0"
SET "SPT_WORKSPACE=%TFM_SCRIPTS%spt_workspace\"
SET "TFM_EXAMPLE=%TFM_SCRIPTS%..\"
SET "TFM_EXAMPLE_S=%TFM_EXAMPLE%tfm_secureboot_s\iar\release\tfm_secureboot_s.hex"
SET "TFM_EXAMPLE_NS=%TFM_EXAMPLE%tfm_secureboot_ns\iar\release\tfm_secureboot_ns.hex"

@rem ============= Command line utilities =======================
SET "nxpimage=%SPT_INSTALL_BIN%\tools\spsdk\nxpimage.exe"
@rem -- "blhost" possible exit codes: 0: success, 1: Secure Boot Enabled, 2: Error --
SET "blhost=%SPT_INSTALL_BIN%\tools\spsdk\blhost.exe"
SET "ifr=%SPT_INSTALL_BIN%\tools\spsdk\ifr.exe"
SET "nxpdebugmbox=%SPT_INSTALL_BIN%\tools\spsdk\nxpdebugmbox.exe"
SET "align=%SPT_INSTALL_BIN%\tools\utils\win\align.exe"
SET "sreccat=%TFM_EXAMPLE%..\..\..\..\middleware\tfm\tf-m\platform\ext\target\nxp\common\secure_boot\tools\srec_cat.exe"
SET "securep=%SPT_INSTALL_BIN%\securep.exe"


@rem ============= Cofiguration parameters =======================
@rem -- Do Flash erase using the "nxpdebugmbox" tool (by default, "blhost" is used) --.
@rem set ERASE_DEBUGMBOX=y
@rem -- Create a Provisioning SB file containing CUST-MK-SK, CMPA and OEM FW (by default, Provisioning SB file contains CUST-MK-SK only)--.
@rem set PROVISION_ONE_SB=y

@rem ============= Check parameters ==============================
if not exist "%nxpimage%" (
	@echo %nxpimage% does not exist!
	goto ERROR
)
if not exist "%blhost%" (
	@echo %blhost% does not exist!
	goto ERROR
)
if not exist "%sreccat%" (
	@echo %sreccat% does not exist!
	goto ERROR
)
if not exist "%ifr%" (
	@echo %ifr% does not exist!
	goto ERROR
)

	exit /b 0
:ERROR
    exit /b 2