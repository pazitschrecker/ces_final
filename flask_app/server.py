import socket
from pygame import mixer
import serial
import sys
import time
import random

from flask import Flask
from flask import render_template
from flask import Response, request, jsonify
app = Flask(__name__)

score = 0
questions = [
    "0 index placeholder",
    {
        "q_id": 1,
        "question": "Who is the rainbow goddess of old?",
        "answer": "Iris",
    },
    {
        "q_id": 2,
        "question": "Which land exists somewhere over the rainbow?",
    },
    {
        "q_id": 3,
        "question": "Where is the best place in the world to see a rainbow?",
    },
]

UDP_IP = "172.20.10.4"  # The IP that is printed in the serial monitor from the ESP32
SHARED_UDP_PORT = 4210
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Internet  # UDP
sock.connect((UDP_IP, SHARED_UDP_PORT))


@app.route('/')
def home():
    return render_template('index.html')


@app.route('/question/<num>')
def question(num=None):
    n = int(str(num))
    question = questions[n]["question"]
    return render_template('question.html', q_id=n, question=question, score=score)


@app.route('/check_answer', methods=['GET', 'POST'])
def check_answer(id=None):
    global score

    data = request.get_json()
    num = int(data["question"])
    answer = data["answer"]
    feedback = "Not quite. The correct answer is ", questions[num]["answer"]
    is_correct = False

    if answer.lower() == (questions[num]["answer"]).lower():
        is_correct = True
        score = score + 1
        feedback = "Correct! Great Job :)"

    return jsonify(feedback=feedback, score=score, is_correct=is_correct)


@app.route('/send_signal', methods=['GET', 'POST'])
def send_signal():

    sock.send('Hello ESP32'.encode())
    print("hi ESP32")

    return jsonify()


if __name__ == '__main__':
    app.run(debug=True)
