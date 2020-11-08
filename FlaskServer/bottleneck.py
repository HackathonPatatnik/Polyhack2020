import os
from flask import Flask
from flask import render_template, request, jsonify

app = Flask(__name__)

@app.route('/_find_bottleneck', methods = ['POST'])
def find_bottleneck():
    data = request.data
    o = open("user.in", "w")
    o.write(data.decode("utf-8"))
    o.close()
    os.popen('g++ -std=c++17 main.cpp && ./a.out > output.txt')
    f = open("output.txt","r")
    sp = f.read().split()
    a = []
    for i in sp:
        a.append(i.split(","))
    print(a)
    return jsonify(a)



@app.route('/')
def hello_world():
    message = "Hello, World"
    return render_template('html.html', message=message)

@app.route('/favicon.ico')
def favicon():
    return send_from_directory(os.path.join(app.root_path, 'static'),
                                    'favicon.ico', mimetype='image/vnd.microsoft.icon')
