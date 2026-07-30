#!/usr/bin/env python3
# Copyright 2024 NXP
# All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

from flask import Flask, request, Response, make_response
import base64
import collections
import json
import os
import time

from modelrunner import Dut

BasePath = os.path.dirname(os.path.realpath(__file__))
if(not os.path.isdir("%s/logs" %BasePath)):
    os.mkdir("%s/logs" %BasePath)

from flask_cors import CORS


app = Flask(__name__)
CORS(app, supports_credentials=True)

model_info = {"inputs": [], "outputs": []}

@app.route("/serial/<serialId>/v1", methods = ["GET"])
@app.route("/serial/<serialId>/")
def v1(serialId):
    resp = {
            "engine": "TensorFlow Lite",
            "model_limits": {
                "block_size": 104857600,
                "max_layers": 512,
                "max_input_size": 607500,
                "max_model_size": 20971520,
                }
            }

    return resp

@app.route("/serial/<serialId>/v1", methods = ["PUT"])
def v1_put(serialId):
    resp = {
            "reply": "success"
            }
    bc = request.form.get("block_count", None)
    global block_count 
    global buf
    if bc:
        block_count = int(bc)
        buf = b''
        dut = Dut(serialId)
        #dut.reset()
        del(dut)
        return resp
    for filename in request.files.keys():
        if filename == "block_content":
            buf += request.files[filename].stream.read()
            block_count = block_count - 1
        elif filename == "block_count":
            buf = request.files[filename].stream.read()
            block_count = int(buf)
            buf = b''
            dut = Dut(serialId)
            #dut.reset()
            del(dut)
            return resp
    if block_count == 0:
       with open('%s/model.tflite' %BasePath, 'wb+') as fd:
           fd.write(buf)
       dut = Dut(serialId)
       dut.send_file("model_loadb" ,"%s/model.tflite" %BasePath)
       del(dut)

    return resp

@app.route("/serial/<serialId>/v1", methods = ["POST"])
def v1_post(serialId):
    # Index-based input tensor loading via field name prefix "input_idx_<N>":
    #   -F "input_idx_0=@a.bin"              -> tensor_loadb input_idx_0
    #   -F "input_idx_1=@b.bin"              -> tensor_loadb input_idx_1
    # Name-based (original) when field name does NOT start with "input_idx_":
    #   -F "input_name=@a.bin"               -> tensor_loadb input_name
    #
    # Multiple outputs via repeated URL param:
    #   ?run=1&output_idx=0&output_idx=1
    dut = Dut(serialId)
    for i, fieldname in enumerate(request.files.keys()):
        buf = request.files[fieldname].stream.read()
        tmp_path = "%s/tmp_%d.input" % (BasePath, i)
        with open(tmp_path, 'wb+') as fd:
            fd.write(buf)
        if fieldname.startswith("input_idx_"):
            # extract tensor index from field name, e.g. "input_idx_0" -> 0
            tensor_idx = fieldname[len("input_idx_"):]
            ret, err_msg = dut.send_file("tensor_loadb input_idx_%s" % tensor_idx, tmp_path)
        else:
            # name-based fallback (original behavior)
            ret, err_msg = dut.send_file("tensor_loadb %s" % fieldname, tmp_path)
        if ret != 0:
            del(dut)
            return {"error": err_msg}
    # forward the full query string as run params (output_idx repeats are preserved)
    param = request.full_path.split("?")[1]
    param = param.replace("&", " ")
    results = dut.send_cmd("run %s" % param)
    r = json.loads(results)
    del(dut)
    return r

@app.route("/serial/<serialId>/v1/model", methods = ["GET"])
def v1_model(serialId):
    dut = Dut(serialId)
    results = dut.send_cmd("model")
    r = json.loads(results)
    del(dut)
    return r

if __name__ == "__main__":
    app.run(host="0.0.0.0", debug = True, port = 10919)
