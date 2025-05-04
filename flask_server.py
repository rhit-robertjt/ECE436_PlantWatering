# Follow guide here to make a webserver
# https://flask.palletsprojects.com/en/2.3.x/patterns/fileuploads/
# Hint: 
# 1. Don't forget to start your webserver!
# 2. You should view the page at http://127.0.0.1:8080
# 3. upload images to the public/uploads folder
# 4. Only allow filetypes:  jpg, png, jpeg, gif
# 5. Pay attention to server logs to debug!

import os
from flask import Flask, Request, flash, request, redirect, url_for, send_from_directory
from werkzeug.utils import secure_filename

app = Flask(__name__,                       # ie "http_server_starter"
            static_url_path = '', 	        # Treat all files as static files.
            static_folder = '.')	        # Look in this folder.

username = None
password = None

@app.route("/variables")
def get_vars():
    return ", ".join(["0", "1", "2", "3"])

@app.route("/logout")
def logout():
    return "true"

@app.route('/hasCredentials', methods=['GET'])
def check_pass_availability():
    print("true" if (username != None and password != None) else "false")
    return "true" if (username != None and password != None) else "false"

@app.route('/setUIDPASS', methods=['POST'])
def set_uid_pass():
    global username, password
    data = request.data.decode("utf-8").split(" ")
    username = data[0]
    password = data[1]
    return "true"

@app.route('/manual', methods=['POST'])
def manual():
    data = request.data.decode("utf-8")
    if "on" in data:
        print("turning on the pump")
    elif "off" in data:
        print("turning off the pump")
    return "ok"

@app.route('/circ', methods=['POST'])
def circulation():
    data = request.data.decode("utf-8")
    if "on" in data:
        print("turning on circ")
    elif "off" in data:
        print("turning off circ")
    return "ok"

@app.route('/setWaterings', methods=['POST'])
def set_watering():
    data = request.data.decode("utf-8").split(" ")
    print("received watering timings: " + data[0] + "/" + data[1])
    return "true"

@app.route('/reset', methods=['POST'])
def reset():
    data = request.data.decode("utf-8")
    print("received: " + data)
    return "He's beginning to believe"

@app.route('/plant_type', methods=['POST'])
def plant_type():
    data = request.data.decode("utf-8")
    print("received: " + data)
    return "true"

@app.route('/modes', methods=['POST'])
def modes():
    data = request.data.decode("utf-8")
    print("received: " + data)
    return "true"

@app.route('/login', methods=['POST'])
def login():
    if (request.form.get('username') == username and request.form.get('password') == password):
        return redirect("/control.html")
    else:
        return redirect("/login.html")

@app.route('/', methods=['GET'])
def upload_file():
    fd = open("webpage.html", "r")
    return fd.read()

@app.route('/<file>.html', methods=['GET'])
def get_file(file):
    if "ico" in file: # don't support the icon file, for Arduino's sake
        return ''
    fd = open(file + ".html", "r")
    return fd.read()

app.run(host = '0.0.0.0', port = 8080, debug = True)