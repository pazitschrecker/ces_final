# Somewhere Over the ESP32

# Description
This project is meant to provide users with an interactive and whimsical learning experience. Users interact with both the hardware and software aspects of this project. The user takes an interactive quiz (built as a Python Flask application) and tracks their progress on the physical lit-up rainbow (built on an ESP32 with LED strips). As the user answers questions correctly, more sections of the rainbow light up, moving from left to right. Once the user has answered every question correctly, a cloud cutout at the end of the rainbow moves to reveal a prize.

Watch a video demo: https://youtu.be/fSHOEaCylMY 

# Running the Web Application
To run the project, connect both the ESP32 and laptop to the same wifi network (I used a mobile hotspot). The ESP32 has a small LED attached that will light up once connected to wifi. Once this is connected, begin running the flask web application by cd-ing into the correct folder and typing `python3 server.py` into the terminal. Once the server is running, type: `http://127.0.0.1:5000/` into the search bar of your browser (I recommend using Chrome). After that you're good to go!
 
# Dependencies
This project requires the Arduino IDE to run. You can download it here: https://www.arduino.cc/en/software
Make sure to select the correct download based on your machine (I used a MacBook Pro).
In order to run the project, you will also need to install the follow arduino IDE libraries:

`Freenove_WS2812_Lib_for_ESP32.h`

`Adafruit_NeoPixel.h`

These libraries can be installed by going to Arduino IDE --> "Tools" --> "Manage Libraries" --> Search for the name of the library and install it. You will need to restart the Arduino IDE before using.

This project also requires the Flask for python as well as python3. 
python 3 -- I installed with brew install python3 If you do not have homebrew (or are not on a macOS/Linux): https://www.python.org/downloads/

flask framework for python -- pip3 install flask

If you do not already have pip installed, type the following into the command line: curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py followed by python3 get-pip.py

# Physical Setup

For this project, all physical components were connected to the ESP32 via a breadboard. First, set up the ESP32 by plugging it into the GPIO Extender and then into a breadboard.

Connect the first LED strip to the breadboard by plugging a M-M wire into the port associated with the black wire at the start of the strip. Plug the other end of this wire into GND on the breadboard. Use two more M-M wires to connect the middle wire to GPIO port 2 and the power wire (red) to 5V on the breadboard. Do the same with the second LED strip, but this time connect the middle wire to GPIO port 5.??

Now connect the servo motor brown wire to GND on the breadboard, the orange wire to GPIO port 14, and the middle red wire (power) for 5V power on the ESP32. I did this by plugging three F-M into the three ports on the servo and then used three M-M wires to connect these to the breadboard. It could be done with only three M-M wires but I did not have any that were long enough.??

Lastly, plug a single LED into the breadboard vertically so that its longer wire and shorter wire are in two different rows. Use a short M-M wire to connect the short end of the LED to GND. Place one end of a 220-ohm resistor in the same row as the long end of the LED and stick the other end of the resistor into another row. Connect this second end of the resistor to GPIO port 13 using a M-M wire. This step is not entirely necessary for the project to function, but is used to tell when the ESP32 has connected to wifi. The LED lights up once the ESP32 is correctly connected to a mobile hotspot.
