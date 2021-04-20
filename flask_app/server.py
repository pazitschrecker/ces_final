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
        "explanation": "Iris is the rainbow goddess in Greek mythology. She is also a messenger for the Olympians and symbolizes communication. In Rick Riordan's interpretation of the myths in the Percy Jackson and Heroes of Olympus series', characters send Iris-messages by creating small rainbows and throwing a golden drachma (coin) into it to honor Iris.",
    },
    {
        "q_id": 2,
        "question": "Which land exists somewhere over the rainbow?",
        "answer": "Oz",
        "explanation": "Oz is a land from The Wizard of Oz, a book and movie from the 20th century. In this story, protagonist Dorothy sings about want to go 'somewhere over the rainbow' and ends up in the land of Oz after a twister picks up her house.",
    },
    {
        "q_id": 3,
        "question": "Which color is often accidentally excluded when drawing rainbows? (Remember ROYGBIV)",
        "answer": "Indigo",
        "explanation": "Indigo is often forgotten in rainbow drawings made by children because the marker is less common and it may be difficult to differentiate the color from its neighbors. You can note that this rainbow omits it as well, due to material limitations.",
    },
    {
        "q_id": 4,
        "question": "You are most likely to see rainbows in this month:",
        "answer": "June",
        "explanation": "June is pride month and the pride flag is rainbow! In many cities there are also pride parades to celebrate the LGBTQ+ community and participants often where colorful (rainbow) clothing and accessories.",
    },
    {
        "q_id": 5,
        "question": "What band has a famous album with (refracting) rainbow artwork? (Hint: the name contains a color not usually present in drawings of rainbows)",
        "answer": "Pink Floyd",
        "explanation": "Pink's Floyd's album 'The Dark Side of the Moon' shows a beam of white light refracting through a triangle. Fun fact: when doing the math with glass, the angle of this refraction for the colors not quite right, but we don't know for sure the material of the triangular object.",
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


@app.route('/done')
def done():
    return render_template('finish.html', score=score)


@app.route('/check_answer', methods=['GET', 'POST'])
def check_answer(id=None):
    global score

    data = request.get_json()
    num = int(data["question"])
    answer = data["answer"]
    feedback = ""
    is_correct = False

    if answer.lower() == (questions[num]["answer"]).lower():
        is_correct = True
        score = score + 1
        feedback = "Correct! \n\n", questions[num]["explanation"]
        send_signal()
    else:
        feedback = "Not quite. The correct answer is ", questions[
            num]["answer"], ".\n\n", questions[num]["explanation"]

    return jsonify(feedback=feedback, score=score, is_correct=is_correct)


# @app.route('/send_signal', methods=['GET', 'POST'])
def send_signal():
    global score
    sock.send(str(score).encode())
    # sock.send('Hello ESP32'.encode())
    print("sent light up instructions with score: ", score)

    # return jsonify()


if __name__ == '__main__':
    app.run(debug=True)
