# Generate SSC for FoE

Due to the license issue, the MCUXpresso SDK does not include the EtherCAT Subdevice Stack Code \(SSC\). The SSC tool must generate the subdevice stack code first. This section lists the steps to download the SSC tool and generate the subdevice stack code for the FoE example via the SSC tool.

**Note**: You must have a Backoff account to download the EtherCAT SSC tool.

1.  Download and install the SSC tool.
    1.  Click the link [https://www.ethercat.org/en/downloads.html](https://www.ethercat.org/en/downloads.html).
    2.  Type `SSC` in the **Text Filter** field.
    3.  Select the **EtherCAT Subdevice Stack Code \(SSC\)** link under the **Description** column.

        **Note:** To proceed with the download, you must provide your account credentials, if you are not already logged in.

        ![](../images/image7.png "Search and select EtherCAT SSC")

2.  Install the downloaded SSC tool.
    1.  Launch the SSC tool.
    2.  Import the configuration file for the FoE example.
    3.  Select **Tool \> Options**.

        ![](../images/image8.png "Select tool options")

        The **Subdevice Stack Code Tool \| Options** window appears.

    4.  Click the **Configurations** tab.
    5.  To open the **Import Configuration file\(s\)** dialog box, click the **＋** icon.
    6.  Navigate to the `\boards\evkmimxrt1180\ecat_examples\foe\<core>\SSC` folder.
    7.  Select the **foe.xml** file.
    8.  Click the **Open** button.

        ![](../images/image9.png "Select the foe.xml file")

        The configuration is imported.

3.  Create a new project with the imported configuration.
    1.  Select **File** \> **New**.

        ![](../images/image10.png "Select the custom project")

        The **New Project** dialog box appears.

    2.  Select the **Custom** option.
4.  Select **NXP ECAT FOE modular <NXP\>** from the drop-down list.

    ![](../images/image11.png "Select the custom project")

5.  Save this new project as **ECAT-FOE.esp** to any folder.
    1.  Select **File** \> **Save**.

        ![](../images/image12.png "Select File > Save")

    2.  Save the file with an appropriate name. For example,**ECAT-FOE.esp**.

        ![](../images/image13.png "Save the file with the name ECAT-FOE.esp")

6.  Create the FoE example application by importing the **foe.xlsx** file.
    1.  Select **Tool** \> **Application** \> **Import**.

        ![](../images/image14.png "Import file")

        The **File selection** dialog box appears.

    2.  Navigate to the ‘\\boards\\evkmimxrt1180\\ecat\_examples\\foe\\<core\>\\SSC’ folder.
    3.  Select the **foe.xlsx** file.
    4.  Click the **Open** button.

        ![](../images/image15.png "Select the foe.xlsx file")

        The filename appears in the **Application name** field.

    5.  Rename the application to **ECAT-FOE** and click **OK**.

        ![](../images/image16.png "Confirm the import")

7.  Create the subdevice stack code for the FoE example.
    1.  Click **Project** \> **Create new Subdevice Files**.

        ![](../images/image17.png "Create new files")

    2.  To create the code, select **Start**. By default, the code is located in the Src subfolder under the folder where the project is saved.

        ![](../images/image18.png "Start to create the code")

8.  Copy the **Src** subfolder to `boards\evkmimxrt1180\ecat_examples\foe\<core>\SSC\Src` under the SDK source code.
9.  Delete the **foe.c** file under the **Src** folder because this file is provided by MCUXpresso SDK.

