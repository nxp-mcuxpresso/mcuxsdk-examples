
@rem ============= Connection parameters =======================
@rem -- Connection parameter for blhost and nxpdevhsm. It should be in format "-p COMx[,baud]" for UART, or "-u VID,PID" for USB --
@rem -- USB --
@rem SET connect=-u 0x1FC9,0x0025
@rem -- COM --
::SET COM_PORT=20
SET COM_PORT=20
SET connect=-p COM%COM_PORT%
SET interface= -i jlink
::SET JTAG=-o use_jtag=100
SET JTAG=
@rem ============= Secure Provisioning Tool ====================
@rem  - Absolute path to SPT installation directory
if "%SPT_INSTALL_BIN%"=="" (
    SET "SPT_INSTALL_BIN=C:\Users\nxf84544\code\spsdk_rw61x_branch\rwvenv\Scripts"
)

@rem ============= TFM example =================================
SET "TFM_SCRIPTS=%~dp0"
SET "SPT_WORKSPACE=%TFM_SCRIPTS%spt_workspace\"
SET "TFM_EXAMPLE=%TFM_SCRIPTS%..\"
SET "TFM_EXAMPLE_S=%TFM_EXAMPLE%..\..\..\Release\tfm_secureboot_s.bin"
SET "TFM_EXAMPLE_NS=%TFM_EXAMPLE%..\..\..\..\rdrw612qfn_tfm_secureboot_ns\Release\tfm_secureboot_ns.bin"

@rem ============= Command line utilities =======================
SET "nxpimage=%SPT_INSTALL_BIN%\nxpimage.exe"
SET "pfr=%SPT_INSTALL_BIN%\pfr.exe"
SET "nxpdevhsm=%SPT_INSTALL_BIN%\nxpdevhsm.exe"
SET "blhost=%SPT_INSTALL_BIN%\blhost.exe"
SET "nxpdebugmbox=%SPT_INSTALL_BIN%\nxpdebugmbox.exe"
SET "shadowregs=%SPT_INSTALL_BIN%\shadowregs.exe"
SET "align=%SPT_INSTALL_BIN%\tools\utils\win\align.exe"

@rem ============= Configuration parameters =======================
@rem -- Create a Provisioning SB file containing CUST-MK-SK only (by default, Provisioning SB file contains CUST-MK-SK, RKTH and SECURE_BOOT_EN)--.
@rem set PROVISION_ONE_SB=y

@rem ============= Check parameters ==============================
if not exist "%nxpimage%" (
	@echo %nxpimage% does not exist!
	goto ERROR
)
if not exist "%pfr%" (
	@echo %pfr% does not exist!
	goto ERROR
)
if not exist "%nxpdevhsm%" (
	@echo %nxpdevhsm% does not exist!
	goto ERROR
)
if not exist "%blhost%" (
	@echo %blhost% does not exist!
	goto ERROR
)

	exit /b 0
:ERROR
    exit /b 2
