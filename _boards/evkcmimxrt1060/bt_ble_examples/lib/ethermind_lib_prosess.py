# import regular expression module
# coding=UTF-8
import re
# import os for file expolre and cmd line
import os
import sys
#import threading
# Repository path
ROOT_PATH = "..\\..\\..\\..\\"
# ethermind lib project generator batch script path
LIB_BATCH_PATH= "bin\\generator\\batch_v2\\batch_wireless\\"

CREATE_LIB_PROJECT_CMD = "ruby ethermind_example_evkcmimxrt1060.rb"

IAR_PATH = 'C:\\Program Files\\IAR Systems\\Embedded Workbench 9.0\\common\\bin'
IAR_BUILD_PATH = IAR_PATH + '\\' + 'IarBuild.exe'
KEIL_BUILD_PATH = "C:\\Keil_v5\\UV4\\UV4.exe"


LIB_list = [
    'bt_a2dp',
    'bt_avrcp',
    'bt_bip',
    'bt_bpp',
    'bt_core',
    'bt_ctn',
    'bt_ftp',
    'bt_gatt',
    'bt_hfp',
    'bt_hid',
    'bt_map',
    'bt_opp',
    'bt_pan',
    'bt_pbap',
    'bt_protocol',
    'bt_sap',
    'bt_spp',
    'bt_util',
    'sbc_common',
    'sbc_decoder',
    'sbc_encoder',
]

#Change LIBRARY_OUTPUT_PATH to "middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/cm7f/gcc" in armgcc "CMakeLists.txt" file.
def ChangeCMakelistOutputPath():
    cMakeListsFileName = "CMakeLists.txt"
    cMakeListsFileNameReplace = "CMakeLists_replace.txt"
    originalLibOutputPath = "LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE}"
    expectedLibOutputPath = "LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE}/../../../../../../../middleware/wireless/ethermind/bluetooth/private/lib/mcux/default/cm7f/gcc"

    print("\r\nCase 2: Modify LIBRARY_OUTPUT_PATH in armgcc \"CMakeLists.txt\" file")
    for num in range(len(LIB_list)):
        projectCMakelistPath = ".\\{}\\{}\\".format(LIB_list[num], "armgcc")

        if os.path.exists(projectCMakelistPath + cMakeListsFileName):

            f = open(projectCMakelistPath + cMakeListsFileName, "r")
            fileStr = f.read()
            if fileStr.find(expectedLibOutputPath) < 0:
                fileStrReplace = fileStr.replace(originalLibOutputPath, expectedLibOutputPath)

                fReplace = open(projectCMakelistPath + cMakeListsFileNameReplace, "w+")
                fReplace.write(fileStrReplace)
                fReplace.close()

            f.close()

            if os.path.exists(projectCMakelistPath + cMakeListsFileNameReplace):
                os.remove(projectCMakelistPath + cMakeListsFileName)
                os.rename(projectCMakelistPath + cMakeListsFileNameReplace, projectCMakelistPath + cMakeListsFileName)
        else:
            print("\t" + projectCMakelistPath + cMakeListsFileName + "does not exist！")
    print("\tChange CMakelist Output Path complete")

# auto to generate the lib project
def libProjectGenerate():
    print("\r\nCase 1: Generate the ethermind lib {} Projects:".format(len(LIB_list)))
    lib_generate_log_file_name = "lib_generate_log.txt"
    all_lib_demo_name = ""

    current_path = os.getcwd()
    for num in range(len(LIB_list)):
        all_lib_demo_name = all_lib_demo_name + "libethermind_{},".format(LIB_list[num])

    os.chdir(ROOT_PATH + LIB_BATCH_PATH)
    print("\tWaiting generate complete. see the " + lib_generate_log_file_name + " file")
    return_value = os.system(CREATE_LIB_PROJECT_CMD + " >"+ current_path + "\\" + lib_generate_log_file_name)
    if return_value == 0:
        os.chdir(current_path)
        print("\tGenerate success!")
    else:
        print("\tGenerate lib Project Failed! please check the generator environment.")

#Search for some kind of file in "seachPath" path
def searchFile(file_type, seachPath):
    file_path_list = []
    for parent,dirnames,filenames in os.walk(seachPath):
        for filename in filenames:
            if re.match(file_type,filename) != None:
                file_path_list.append(os.path.join(parent,filename))
    for value in range(len(file_path_list)):
        if file_path_list[value].find(seachPath) >= 0:
            file_path_list[value] = file_path_list[value].replace(seachPath, '.') 
    return file_path_list

#Create a common keil workspace to add exist lib project
def integrateKeillibWorkspace(file_path_list):
    print("\r\nCase 3: Add {} Keil project to common Workspace".format(len(file_path_list)))
    startStr  = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n<ProjectWorkspace xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"project_mpw.xsd\">\n\n  <SchemaVersion>1.0</SchemaVersion>\n\n  <Header>### uVision Project, (C) Keil Software</Header>\n\n  <WorkspaceName>WorkSpace</WorkspaceName>\n\n"
    middleStr = "  <project>\n    <PathAndName>$file_path_list_name</PathAndName>\n    <NodeIsActive>1</NodeIsActive>\n    <NodeIsExpanded>1</NodeIsExpanded>\n  </project>\n\n"
    endStr    = "</ProjectWorkspace>\n"
    f = open('Keil_Libethermind.uvmpw','w+')
    f.write(startStr)
    for value in range(len(file_path_list)):
        middleStrReplace = middleStr.replace("$file_path_list_name", file_path_list[value])
        f.write(middleStrReplace)
    f.write(endStr)
    f.close()
    print("\tKeil_Libethermind.uvmpw file created")

#Create a common keil workspace to add exist lib project
def integrateIarlibWorkspace(file_path_list):
    print("\r\nCase 3: Add {} Iar project to common Workspace".format(len(file_path_list)))
    startStr  = "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n<workspace>"
    middleStr = "  <project>\n    <path>$file_path_list_name</path>\n  </project>\n"
    endStr    = "</workspace>\n"
    f = open('Iar_Libethermind.eww','w+')
    f.write(startStr)
    for value in range(len(file_path_list)):
        middleStrReplace = middleStr.replace("$file_path_list_name", file_path_list[value])
        f.write(middleStrReplace)
    f.write(endStr)
    f.close()
    print("\tIar_Libethermind.eww file created")


    #Build iar project
def buildIARProject(project_path, project_type='release'):
    print("\r\nCase 4: Build iar ({} {}) project ".format(project_path,project_type))
    IAR_BUILD_CMD = "\"{}\" {} -build {} -log errors".format(IAR_BUILD_PATH, project_path, project_type)
    print(IAR_BUILD_CMD)
    return_value = os.system(IAR_BUILD_CMD)
    if return_value == 0:
        print('\t' + project_path + " Build success!")
    else:
        print('\t' + project_path + " Build Errors!")

#Build keil project
def buildKEILProject(project_path, project_type='Release'):
    print("\r\nCase 4: Build keil ({} {}) project".format(project_path,project_type))
    project_str = os.path.split(project_path)
    project_name = project_str[1].split('.')

    KEIL_BUILD_CMD = "start /wait {} -b -j0  {}  -o build_log.txt -t \"{} {}\"".format(KEIL_BUILD_PATH, project_path, project_name[0], project_type)
    print(KEIL_BUILD_CMD)
    return_value = os.system(KEIL_BUILD_CMD)
    if return_value == 0:
        print('\t' + project_path + " Build success!")
    else:
        print('\t' + project_path + " Build Errors!")


#Build armgcc project
def buildGccProject(project_path, project_type='release'):
    print("\r\nCase 4: Build armgcc ({} {}) project".format(project_path,project_type))
    gccScriptFile = ".\\{}\\armgcc".format(project_path)
    current_path = os.getcwd()
    os.chdir(gccScriptFile)
    # if os.path.exists(gccScriptFile):
    GCC_BUILD_CMD = "build_{}.bat 1>log.txt 2>error.txt".format(project_type)
    print(GCC_BUILD_CMD)
    return_value = os.system(GCC_BUILD_CMD)

    os.chdir(current_path)
    if return_value == 0:
        print("\tBuild success: " + project_path)
    else:
        print("\tBuild Errors: " + project_path)


def buildCofingOption(toolChain, project_type):
    keil_project_path_list = searchFile(r'libethermind_.+\.uvprojx', current_path)
    iar_project_path_list = searchFile(r'libethermind_.+\.ewp',current_path)

    if toolChain == 'all':
        if project_type == 'release':
            #threading.thread
            for num in range(len(keil_project_path_list)):
                buildKEILProject(keil_project_path_list[num], 'Release')
        elif project_type == 'debug':
            for num in range(len(keil_project_path_list)):
                buildKEILProject(keil_project_path_list[num], 'Debug')

        for num in range(len(iar_project_path_list)):
            buildIARProject(iar_project_path_list[num], project_type)
        for num in range(len(LIB_list)):
            buildGccProject(LIB_list[num], project_type)

    elif toolChain == 'iar':
        for num in range(len(iar_project_path_list)):
            buildIARProject(iar_project_path_list[num], project_type)

    elif toolChain == 'keil':
        if project_type == 'release':
            for num in range(len(keil_project_path_list)):
                buildKEILProject(keil_project_path_list[num], 'Release')
        elif project_type == 'debug':
            for num in range(len(keil_project_path_list)):
                buildKEILProject(keil_project_path_list[num], 'Debug')

    elif toolChain == 'gcc':
        for num in range(len(LIB_list)):
            buildGccProject(LIB_list[num], project_type)


def Execution_process(current_path):
    generate_flag = False
    intergate_flag =False
    build_flag = False
    toolChain_type = 'all'
    target_type = 'release'

    print(" \n****************************ethermind_lib_process_script********************************\
            \nScript funtion 1.Generate lib project.  2.Change armgcc output path.  3.integration commom workspace.  4.build project.\
            \nDefault function(not added the parameters): Generate lib project + Change armgcc output path + integration commom workspace.\
            \n  parameters:\
            \n\t'-g': Generate all lib project + change armgcc output path\
            \n\t'-i': integration iar and keil lib project to commom workspace\
            \n\t'-b': build project(default: 'all' 'release'). Also can add other build config: {'all','keil','iar','gcc'}, {'release', 'debug'}\
            \n\t           such as: 'ethermind_lib_process.py -b iar debug'\
            \n****************************************************************************************")
    for num in range(len(sys.argv)):
        if (sys.argv[num] == "-g"):
            generate_flag = True
        if (sys.argv[num] == "-i"):
            intergate_flag = True
        if (sys.argv[num] == "-b"):
            build_flag = True
        if (sys.argv[num] == "debug"):
            target_type = 'debug'
        if (sys.argv[num] == "iar"):
            toolChain_type = 'iar'
        if (sys.argv[num] == "keil"):
            toolChain_type = 'keil'
        if (sys.argv[num] == "gcc"):
            toolChain_type = 'gcc'
            


    if generate_flag == False and intergate_flag == False and build_flag == False:
        #default process: generate project and create common workspace
        libProjectGenerate()
        ChangeCMakelistOutputPath()

        keil_project_path_list = searchFile(r'libethermind_.+\.uvprojx', current_path)
        integrateKeillibWorkspace(keil_project_path_list)

        iar_project_path_list = searchFile(r'libethermind_.+\.ewp', current_path)
        integrateIarlibWorkspace(iar_project_path_list)

    else:
        if generate_flag == True:
            libProjectGenerate()
            ChangeCMakelistOutputPath()
        if intergate_flag == True:
            keil_project_path_list = searchFile(r'libethermind_.+\.uvprojx', current_path)
            integrateKeillibWorkspace(keil_project_path_list)
            iar_project_path_list = searchFile(r'libethermind_.+\.ewp',current_path)
            integrateIarlibWorkspace(iar_project_path_list)
        if build_flag == True:
            buildCofingOption(toolChain_type, target_type)

    os.system("pause")
        


if __name__== "__main__":
    current_path = os.getcwd()
    Execution_process(current_path)




