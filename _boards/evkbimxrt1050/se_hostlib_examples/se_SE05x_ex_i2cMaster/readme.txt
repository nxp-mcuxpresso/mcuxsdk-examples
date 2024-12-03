.. highlight:: bat

.. _ex-i2cMaster:

=======================================================================
 I2C Master Example
=======================================================================

Prerequisites
=======================================================================

- Bring Up Hardware. (Refer :ref:`dev-platforms`)

- Connection to be done for I2C Master example.

.. image:: i2cm-accelerometer-k64f-wiring.png


Here is a photograph of above wiring diagram.

.. image:: i2cm-accelerometer-k64f-photo.png


.. note:: - We are using 2nd freedom K64F board only for connecting Accelerometer device to I2CM.
          - Short Jumper J9 & J10 of se050ARD board.

Disable K64F on board 2
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. warning:: If the K64F of the 2nd board doing some operations on I2C Pins, this demo would not work.

Follow below steps to make this demo work.

- Flash :file:`frdmk64f_nop_wfi.bin` binary located at :file:`demos/se05x/ex_i2cMaster` directory to K64F board.
- This binary will put K64F in unoperation state and we will have easy access to Accelerometer through I2C pins.

Below is the c code ::

  int main(void)
  {
      /* Init board hardware. */
      BOARD_InitPins();
      BOARD_BootClockRUN();
      BOARD_InitDebugConsole();

      __disable_irq();
      while (1)
      {
          __disable_irq();
          __WFI();
      }
  }


About the Example
=======================================================================

This example reads Accelerometer data via the I2C master interface.

The Accelerometer on other K64F is used as an I2C Slave.

.. image:: I2C-Master-operation.png

It uses the following APIs and data types:

- :cpp:func:`Se05x_i2c_master_txn()`
- :cpp:func:`Se05x_i2c_master_attst_txn()`
- :cpp:enumerator:`kSE05x_I2CM_Configure` from :cpp:type:`SE05x_I2CM_TLV_type_t`
- :cpp:enumerator:`kSE05x_I2CM_Write` from :cpp:type:`SE05x_I2CM_TLV_type_t`
- :cpp:enumerator:`kSE05x_I2CM_Read` from :cpp:type:`SE05x_I2CM_TLV_type_t`
- :cpp:enumerator:`kSE05x_I2CM_Baud_Rate_400Khz` from :cpp:type:`SE05x_I2CM_Baud_Rate_t`


Running the Demo
=======================================================================

#) Import project :file:`cmake_project_frdmk64f` from :file:`simw-top/projects` directory.

#) Mention :samp:`BUILD_TARGET` as :samp:`ex_i2cMaster` or :file:`ex_i2cMaster_with_Attestation` in :file:`Debug/Makefile`.

#) Build the project and flash binary inside :file:`FRDMK64F_SE050ARD` board.

#) Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

#) Rotate second K64F in any direction.

If everything is setup correctly the output would be as follows::

    App:INFO :I2CM example to read Accelerometer value
       App:INFO :x =   113 , y =   -73 , z =  2118
       App:INFO :x =   109 , y =   -67 , z =  2103
       App:INFO :x =   108 , y =   -68 , z =  2120
       App:INFO :x =   117 , y =   -69 , z =  2109
       App:INFO :x =   117 , y =   -71 , z =  2105
       App:INFO :x =   111 , y =   -71 , z =  2108
       App:INFO :x =   115 , y =   -72 , z =  2104
       App:INFO :x =   117 , y =   -69 , z =  2122
       App:INFO :x =   115 , y =   -73 , z =  2120
       App:INFO :x =   115 , y =   -74 , z =  2114
       App:INFO :I2CM test completed !!!...


