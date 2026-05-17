# TFLite ModelRunner User Guide

##  Overview
ModelRunner_hifi4 is a benchmarking tool for running TensorFlow Lite models on NXP microcontrollers. It supports UART communication modes and provides detailed latency profiling for each model layer.

##  Supported Toolchains
- **xcc

##  Supported Boards
- MIMXRT700-EVK

##  Running the Demo
1. **Flash the board with the firmware**

  check modelrunner run successfully on board, and must exit uart console to make sure modelrunner agent can communicate with uart.
  ```
  Starting HiFi4 example from Cortex-M33 core
  DSP Init Susccessfully

  *************************************************
                 TFLite Modelrunner
  *************************************************
  =>
  ```

2. **Run HTTP-to-UART agent on x86**
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

  4.2. **Run Latency Benchmark**
   ```bash
   curl -X POST http://<IP>:<port>/serial/<serial-id>/v1?run=1
   ```
   users can get agent response of inference time:
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
     -F 'file=@<path_to_input_tensor>.bin'
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
