# TFLite ModelRunner User Guide

##  Overview
ModelRunner is a benchmarking tool for running TensorFlow Lite models on NXP microcontrollers. It supports both HTTP and UART communication modes and provides detailed latency profiling for each model layer.

##  Supported Toolchains
- **MCUXpresso IDE**
- **IAR Embedded Workbench for ARM**
- **Keil uVision MDK**
- **ArmGCC (GNU Tools ARM Embedded)**

##  Supported Boards
- FRDM-MCXN947
- MCX-N5XX-EVK
- MCX-N9XX-EVK
- MIMXRT700-EVK
- MIMXRT595-EVK
- MIMXRT685-EVK
- MIMXRT1060-EVK
- FRDM-IMXRT1152
- EVKB-IMXRT1050
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/eiq_examples/tflm_modelrunner/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/eiq_examples/tflm_modelrunner/example_board_readme.md)
- MIMXRT1040-EVK
- [FRDM-IMXRT700](../../_boards/frdmimxrt700/eiq_examples/tflm_modelrunner/example_board_readme.md)
- MIMXRT1060-EVKC
- MIMXRT1060-EVKB
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/eiq_examples/tflm_modelrunner/example_board_readme.md)
- MIMXRT1170-EVKB
- EVK-MIMXRT1064

##  Running the Demo

###  HTTP Mode (Default for `mcxn9xxevk`)
1. **Get Device IP**

   Connect device console to check the IP address:
   ```
   *************************************************                                       
              TFLite Modelrunner
   *************************************************
    Initializing PHY... 
    DHCP state : SELECTING 
    DHCP state : REQUESTING 
    DHCP state : BOUND

    IPv4 Address : 10.193.20.56 
    IPv4 Subnet mask : 255.255.255.0 
    IPv4 Gateway : 10.193.20.254
    
    Initialized TFLiteMicro modelrunner server at port 10818
   ```

2. **Upload Model**

   ```
   curl -X PUT http://10.193.20.56:10818/v1 -F 'file=@"{model_name}.tflite";filename="model_name;name=block_content"'
   ```
   users can get agent response as below:
   ```
   {
      "reply": "success"
   }
   ```

3. **Run Latency Benchmark**
   ```
   curl -s -X POST http://<IP>:10818/v1?run=1
   ```

   users can get agent response of inference time (us):
   ```
   {
      "timing":54238
   }
   ```

4. **Get Model Info**
   ```
   curl http://<IP>:10818/v1/model
   ```
   users can get detail model info:
   ```
   {"timing": 54238,"model_size": 53936,"ktensor_arena_size": 22812,"inputs": [{"name": "input_1","scale": 0.584703,"zero_points": 83,"data_type": "INT8","shape": [1,49,10,1]}],"outputs": [{"name": "Identity","scale": 0.003906,"zero_points": -128,"data_type": "INT8","shape": [1,12]}],"layer_count": 13,"layers": [{"name": "functional_1/activation/Relu;functional_1/batch_normalization/FusedBatchNormV3;functional_1/conv2d/BiasAdd/ReadVariableOp/resource;functional_1/conv2d/BiasAdd;functional_1/conv2d_4/Conv2D;functional_1/conv2d/Conv2D1","type": "CONV_2D","timing": 8257,"process_unit": "CPU","shape": [1,49,10,1]},{"name": "functional_1/activation_1/Relu;functional_1/batch_normalization_1/FusedBatchNormV3;functional_1/depthwise_conv2d/depthwise;functional_1/depthwise_conv2d/BiasAdd;functional_1/conv2d_4/Conv2D;functional_1/depthwise_conv2d/BiasAdd/ReadVariableOp/resource1","type": "DEPTHWISE_CONV_2D","timing": 4055,"process_unit": "CPU","shape": [1,25,5,64]},{"name": "functional_1/activation_2/Relu;functional_1/batch_normalization_2/FusedBatchNormV3;functional_1/conv2d_1/BiasAdd/ReadVariableOp/resource;functional_1/conv2d_1/BiasAdd;functional_1/conv2d_4/Conv2D;functional_1/conv2d_1/Conv2D1","type": "CONV_2D","timing": 7374,"process_unit": "CPU","shape": [1,25,5,64]},{"name": "functional_1/activation_3/Relu;functional_1/batch_normalization_3/FusedBatchNormV3;functional_1/depthwise_conv2d_1/depthwise;functional_1/depthwise_conv2d_1/BiasAdd;functional_1/conv2d_4/Conv2D;functional_1/depthwise_conv2d_1/BiasAdd/ReadVariableOp/resource1","type": "DEPTHWISE_CONV_2D","timing": 3982,"process_unit": "CPU","shape": [1,25,5,64]},{"name": "functional_1/activation_4/Relu;functional_1/batch_normalization_4/FusedBatchNormV3;functional_1/conv2d_2/BiasAdd/ReadVariableOp/resource;functional_1/conv2d_2/BiasAdd;functional_1/conv2d_4/Conv2D;functional_1/conv2d_2/Conv2D1","type": "CONV_2D","timing": 7448,"process_unit": "CPU","shape": [1,25,5,64]},{"name": "functional_1/activation_5/Relu;functional_1/batch_normalization_5/FusedBatchNormV3;functional_1/depthwise_conv2d_2/depthwise;functional_1/depthwise_conv2d_2/BiasAdd;functional_1/conv2d_4/Conv2D;functional_1/depthwise_conv2d_2/BiasAdd/ReadVariableOp/resource1","type": "DEPTHWISE_CONV_2D","timing": 3983,"process_unit": "CPU","shape": [1,25,5,64]},{"name": "functional_1/activation_6/Relu;functional_1/batch_normalization_6/FusedBatchNormV3;functional_1/conv2d_3/BiasAdd/ReadVariableOp/resource;functional_1/conv2d_3/BiasAdd;functional_1/conv2d_4/Conv2D;functional_1/conv2d_3/Conv2D1","type": "CONV_2D","timing": 7372,"process_unit": "CPU","shape": [1,25,5,64]},{"name": "functional_1/activation_7/Relu;functional_1/batch_normalization_7/FusedBatchNormV3;functional_1/depthwise_conv2d_3/depthwise;functional_1/depthwise_conv2d_3/BiasAdd;functional_1/conv2d_4/Conv2D;functional_1/depthwise_conv2d_3/BiasAdd/ReadVariableOp/resource1","type": "DEPTHWISE_CONV_2D","timing": 3984,"process_unit": "CPU","shape": [1,25,5,64]},{"name": "functional_1/activation_8/Relu;functional_1/batch_normalization_8/FusedBatchNormV3;functional_1/conv2d_4/BiasAdd/ReadVariableOp/resource;functional_1/conv2d_4/BiasAdd;functional_1/conv2d_4/Conv2D1","type": "CONV_2D","timing": 7375,"process_unit": "CPU","shape": [1,25,5,64]},{"name": "functional_1/average_pooling2d/AvgPool","type": "AVERAGE_POOL_2D","timing": 350,"process_unit": "CPU","shape": [1,25,5,64]},{"name": "functional_1/flatten/Reshape","type": "RESHAPE","timing": 6,"process_unit": "CPU","shape": [1,1,1,64]},{"name": "functional_1/dense/BiasAdd","type": "FULLY_CONNECTED","timing": 27,"process_unit": "CPU","shape": [1,64]},{"name": "Identity","type": "SOFTMAX","timing": 25,"process_unit": "CPU","shape": [1,12]}]}
   ```

5. **Upload Input Tensor & Get Output**


   ```bash
   curl -s -X POST "http://<IP>:10818/v1?run=1&output=Identity" -F 'file=@tst_000011_Left_2.bin;filename="input_tensor;name=input_1"' |jq .outputs[0].data | xargs -i echo {$i} |base64 -d |hexdump -C
   ```
   users can get output data by hexdump for further post-process:
   ```
   00000000  ff 80 80 80 80 ff 80 80  80 80 80 81              |............|
   0000000c
   ```

###  UART Mode (Default for `mimxrt700evk`)
1. **Flash the board with the firmware**

  check moderunner run successfully on board, and must exit uart console to make sure modelrunner agent can communicate with uart.
  ```
	*************************************************
        	       TFLite Modelrunner
	*************************************************
  	XSPI Psram Enabled!

  	=>
  ```

2. **Run HTTP-to-UART agent on x86**

  HTTP-to-UART agent scripts are under ${project_files}/scripts. User can install dependency from requirement.txt in the server that have local platforms plugined in. The agent is implemented by PEXPECT that can operate URAT serial, it can transfer data packages between agent ethernet port and target device serial port.

   ```bash
   python3 main.py
   ```
   Server runs at: `http://$ip:$port`, users can modify the port id according to IT firewall policy, we set default port to 10919. 

   ```
   * Serving Flask app 'main'
   * Debug mode: on
   WARNING: This is a development server. Do not use it in a production deployment. Use a production WSGI server instead.
   * Running on all addresses (0.0.0.0)
   * Running on http://127.0.0.1:10919
   * Running on http://10.192.208.139:10919
   ```

3. **Connect UART cable**
   - Linux: Check `/dev/serial/by-id/`
   - Windows: Use `COM<x>` port

  MCU devices without ethernet port, the connection IP will be like "http://$server_ip:$port/serial/001063836560/v1"

4. **Benchmark**
   Users can provide dataset according to model's input and prepare benchmark test code for further interpretation.

  4.1. **Upload Model**
   ```bash
   curl -X PUT http://<IP>:<port>/serial/<serial-id>/v1 -d "block_count=1"
   
   curl -X PUT http://<IP>:<port>/serial/<serial-id>/v1 -F 'block_content=@"${model_path}";filename="${model_name},name=block_content"'
   ```
   users can get agent response as below:
   ```
   {
      "reply": "success"
   }
   ```
  Model will upload to modelrunner, it may take different time for different model size, it will generate log file under agent repo that you can track model upload log with this command:
   ```
  tail -f logs/tty-<serial-id>.log
   ```
 The sample log as below:
   ```
model_loadb 53936936
######### Ready for TFLite model download MEM

######### 53936 bytes received #########
   ```

  4.2. **Run Latency Benchmark**
   ```bash
   curl -X POST http://<IP>:<port>/serial/<serial-id>/v1?run=1
   ```
   users can get agent response of inference time (us): 
   ```
   {
      "timing": 511
   }
   ```

  4.3. **Get Model Info**
   ```bash
   curl http://<IP>:<port>/serial/<serial-id>/v1/model
   ```
   users can get detail model info layer by layer format with json:
   ```
   {
  "inputs": [
    {
      "data_type": "INT8",
      "name": "input_1",
      "scale": 0.584703,
      "shape": [
        1,
        49,
        10,
        1
      ],
      "zero_points": 83
    }
  ],
  "ktensor_arena_size": 17172,
  "layer_count": 3,
  "layers": [
    {
      "avg_timing": 485000.0,
      "name": "functional_1/dense/BiasAdd",
      "timing": 485000.0,
      "type": "NeutronGraph"
    },
    {
      "avg_timing": 17000.0,
      "name": "Identity",
      "timing": 17000.0,
      "type": "SOFTMAX"
    }
  ],
  "outputs": [
    {
      "data_type": "INT8",
      "name": "Identity",
      "scale": 0.003906,
      "shape": [
        1,
        12
      ],
      "zero_points": -128
    }
  ],
  "timing": 502
}
   ```

  4.4. **Upload Input Tensor & Get Output Data**
   ```bash
   curl -X POST "http://<IP>:<port>/serial/<serial-id>/v1?run=1&output=${output_tensor_name}" \
     -F 'file=@<path_to_input_tensor>.bin;filename="input_tensor;name=${input_tensor_name}"'
   ```  
   users can get output data and do post-process accordingly:
   ```
{
  "outputs": [
    {
      "data": "/4CAgID/gICAgICB",
      "datatype": "INT8",
      "name": "Identity",
      "shape": [
        1,
        12
      ]
    }
  ],
  "timing": 498
}
   ```

##  ModelRunner CLI
connect to uart console of device, and run with cli as below:
```bash
python cli.py com20
=> reset
=> model_loadb model.tflite
=> model
=> tensor_loadb input_1 tmp.input
=> run output=Identity
```
