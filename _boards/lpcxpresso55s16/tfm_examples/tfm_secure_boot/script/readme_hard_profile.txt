################################################################################
Secure Boot - Getting Started
################################################################################

################################################################################
Secure Boot - Requirements
################################################################################

Hardware:
    - LPC55S16-EVK
    - USB cable

Software and Tools:
    - Windows 10 patform, to execute batch scripts.
    - Keil µVision® IDE V5.29.0.0 or higher. http://www2.keil.com/mdk5
    - srec_cat.exe v1.64 or higher. Already installed in <SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\tools.
    - elftosb.exe v5.1.19 or higher. Already installed in <SDK>\in middleware\tfm\platform\ext\target\nxp\common\secure_boot\tools.
    - blhost.exe v2.6.2 or higher. Already installed in <SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\tools.
    - ymltopfr.exe v1.0.0 or higher. Already installed in <SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\tools.

Please follow the step by step guide how to provission, to prepare and to upload secure a firmware.

################################################################################
0. Connect device to the PC
################################################################################

    0.1 Connect Micro USB to the J1 connector on the device.
    0.2 Get port number:
    0.2.1 Open the Device Manager in the Windows 10.
    0.2.2 Find the "LPC-LinkII UCom Port (COMx)" port number in the "Ports(COM & LPT)" folder. Remember the used COM port number, you will need it for the step 2 and 3.
    0.3 Switch board to the In System Programming (ISP) mode:
    0.3.1 Press and hold the RESET button (Switch SW2) on the LPC55S1x development board.
    0.3.2 Press and hold the ISP button (Switch SW4) on the LPC55S1x development board.
    0.3.3 Release the RESET button (Switch SW2) but still hold the ISP button (Switch SW4).
    0.3.4 Release the ISP button (Switch SW4).
    0.3.5 The bootloader entered to the ISP mode.

################################################################################
1. Create Secure Binary
################################################################################

    1.1 Build Secure and Non-Secure Applications:
    
        1.1.1 Go to "../tfm_secure_boot_s/mdk".
        1.1.2 Open the "tfm_secure_boot.uvmpw" project file in the Keil µVision® IDE.
        1.1.3 Click on the "Batch Build" button in the "Build Toolbar". It starts building of tfm_secure_boot_s and tfm_secure_boot_ns projects.
        1.1.4 Wait until all builds processes are finished.
    
        NOTE:
             - The output "tfm_secure_boot_s.hex" file is in the "../tfm_secure_boot_s/mdk/release" folder.
             - The output "tfm_secure_boot_ns.hex" file is in the "../tfm_secure_boot_ns/mdk/release" folder.
             
    1.2 Prepare "Customer Filed Programmable Area" (CFPA) binary:

         Execute the "1_1_make_cfpa.bat" file.

         NOTE:
              - The output "cfpa.bin" file is located in the ""<SDK>\boards\lpcxpresso55s16\tfm_examples\tfm_secure_boot\script\pfr"" folder.    
              - This step is mandatory for very first load of Firmware (FW). It is optional for next FW updates.
                More details are described in "..\..\..\..\..\middleware\tfm\platform\ext\target\nxp\common\secure_boot\advancedReadme.txt".
                
    1.3 Build CFPA Application:
    
        1.3.1 Go to "../tfm_cfpa/mdk".
        1.3.2 Open the "tfm_cfpa.uvmpw" project file in the Keil µVision® IDE.
        1.3.3 Click on the "Batch Build" button in the "Build Toolbar". It starts building of tfm_cfpa project.
        1.3.4 Wait until all builds processes are finished.
    
        NOTE:
             - The output "tfm_cfpa.hex" file is in the "../tfm_cfpa/mdk/release" folder.
             - This step is mandatory for very first load of Firmware (FW). It is optional for next FW updates.
             - The output "tfm_cfpa.bin" file is in the "script\pfr" folder.
             - The "cfpa.bin" file is located at address 0x20000.
    
    1.4 Prepare binaries:
        
        Execute "1_2_make_bin.bat" file:
            1. It merges "tfm_secure_boot_s.hex" and "tfm_secure_boot_ns.hex" files, align them to 0x38000 (PRINCE region end) and converts to the binary file.
            2. It convers "tfm_cfpa.hex" converts to the binary file.
    
        NOTE:
             - The output "tfm_secure_boot.bin" file is in the "\output" folder.
             - The step 2 is perfomed only when "tfm_cfpa.hex" file exists.
    
    1.5 Sign the binary:
    
        Execute the "1_3_sign_bin.bat" file.
        
        NOTE: 
            - The output "signed_tfm_secure_boot.bin" file is in the "\output" folder.
            - The output "signed_tfm_cfpa.bin" file is in the "\output" folder. (only if tfm_cfpa.bin exists)
    
    1.6 Prepare Secure Binary.
    
        Execute the "1_4_make_sb.bat" file.
    
        NOTE:
             - The output "tfm_secure_boot.sb2" file is in the "\output" folder.
             - The output "tfm_cfpa.sb2" file is in the "\output" folder. (only if signed_tfm_cfpa.bin exists)

################################################################################
2. Provision Protected Flash Region (PFR) (it is performed only once)
################################################################################

    2.1 Prepare Protected Flash Region (PFR) binary:

        2.1.1 Go to "<SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\template" folder
        2.1.2 Open file lpc55s1x_allPFR.yml
        2.1.3 Be shore that variable SEAL is set to 1 (SEAL: 1)
        2.1.4 Execute the "2_1_make_pfr.bat" file.
        
        NOTE:
             The ouput "cmpa.bin" and "cfpaInit.bin" files are locate in the temporary "<SDK>\boards\lpcxpresso55s16\tfm_examples\tfm_secure_boot\script\output" folder.

    2.2 Initialize the Key Store and Loading of PFR binaries:

        Execute the "2_2_provision.bat COM<number>"
         
        NOTE:
             This batch file must be executed with the "COM<number>" argument,
             where <number> is number of COM port you have got in step (0.2).

################################################################################
3. Update Firmware (Secure Binary)
################################################################################

    Execute "3_update_fw.bat COM<number>" in a command prompt.
    It loads the Secure Binary generated by step (1) to the device.

    NOTE:
         This batch file must be executed with the "COM<number>" argument,
         where <number> is number of COM port you have got in step (0.2).

################################################################################
4. Erase \output folder
################################################################################

    Execute the "clean.bat" file.

More details are described in the "<SDK>\middleware\tfm\platform\ext\target\nxp\common\secure_boot\advancedReadme.txt" file.

*Copyright (c) 2020, NXP. All rights reserved.* 