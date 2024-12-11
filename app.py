from flask import Flask, request, jsonify
import pandas as pd
import time

app = Flask(__name__)

data = []

@app.route('/data', methods=['POST'])
def receive_data():
    global data
    content = request.json
    content['timestamp'] = time.time()
    data.append(content)
    return jsonify({"status": "success"})

@app.route('/get_data', methods=['GET'])
def get_data():
    return jsonify(data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
