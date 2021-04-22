#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"
#include <Wire.h>
#include <ESP32Servo.h>

#include <stdio.h>
#include <string.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    2 // warm colors/strip1
#define LED2_PIN   5 // cool colors/strip2

#define SINGLE_LED_PIN  13  // single LED to show that wifi is connected
Servo myservo; // create servo object to control a servo 
int posVal = 0; // variable to store the servo position 
int servoPin = 14; // Servo motor pin

// How many LEDs in strip 
#define LED_COUNT 60

const char* ssid = "Guest Network";
const char* password = "guest_pw";
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  //  port to listen on
char incomingPacket[255];  // buffer for incoming packets
char restart[255] = "restart";
char openCloud[255] = "open";

// Declare LED strip objects
Adafruit_NeoPixel warm_strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel cool_strip(LED_COUNT, LED2_PIN, NEO_GRB + NEO_KHZ800);

int score;
int played;

// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  // INITIALIZE NeoPixel strip objects (wartm and cool)
  warm_strip.begin(); 
  cool_strip.begin();

  // Turn OFF all pixels at start
  warm_strip.show(); 
  cool_strip.show();   
  
  // Set BRIGHTNESS to about 1/8 (max = 255)
  warm_strip.setBrightness(30); 
  cool_strip.setBrightness(30); 

  pinMode(SINGLE_LED_PIN, OUTPUT);

  // standard 50 hz servo
  myservo.setPeriodHertz(50);    
     
  // attaches the servo on servoPin to the servo  
  myservo.attach(servoPin, 500, 2500);



  score = 0;
  played = 0;

  // try connecting to wifi
  int status = WL_IDLE_STATUS;
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // turn on single LED to show that ESP32 is connected to wifi
  digitalWrite(SINGLE_LED_PIN, HIGH);
  
  Serial.println("Connected to wifi");
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort); 
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  bool readPacket = false;
  while (!readPacket) {
    int packetSize = Udp.parsePacket();
    if (packetSize)
     {
      // receive incoming UDP packets
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      Serial.printf("UDP packet contents: %s\n", incomingPacket);
      readPacket = true;
      
      // packet tells ESP32 to reset
      if (strcmp (incomingPacket,restart) == 0) {
        Serial.printf(" incomingPacket: %s\n restart: %s\n", incomingPacket, restart);
        resetStrips();
      }

      // open cloud with servo
      else if (strcmp (incomingPacket,openCloud) == 0) {
        Serial.printf("opening cloud");
        moveCloud(1);
      }

      // score updated, fill in more of rainbow
      else {
       score = score + 1;
        Serial.printf("SCORE: %d\n", score);
        fill(score, 50);
      }
    } 
  }
}


// Helper functions -----------------

// function to fill certain percent of strip
void fill(int numFifths, int wait) {
  Serial.printf("reached fill function; numFifths = %d\n", numFifths);
  // number of pixels to fill: numFifths * 12
  // number to fill per color: numFifths * 12 /3 = numFifths * 4
  
  for (int j = 0; j < numFifths * 4; j++) {
    // set first strip: red, orange, yellow
    warm_strip.setPixelColor(j, warm_strip.Color(255, 0, 0)); // 1 - 20: red
    warm_strip.setPixelColor(40-j, warm_strip.Color(255, 60, 0)); // 40 - 21: orange
    warm_strip.setPixelColor(j+41, warm_strip.Color(255, 100, 0)); // 41 - 60: yellow
    warm_strip.show(); 

    // set second strip: green, blue, purple
    cool_strip.setPixelColor(j, cool_strip.Color(0, 255, 0)); // 1 - 20: green
    cool_strip.setPixelColor(40-j, cool_strip.Color(0, 0, 255)); // 40 - 21: blue
    cool_strip.setPixelColor(j+41, cool_strip.Color(200, 0, 255)); // 41 - 60: purple
    cool_strip.show(); 
    delay(wait); 
  }
}

// function to reset
void resetStrips() {
  score = 0;
  
  if (played == 1) {
    // move cloud back to original position if not in original position
    moveCloud(-1);
  }

  // turn off all pixels
  for (int j = 0; j < LED_COUNT; j++) {
      warm_strip.setPixelColor(j, warm_strip.Color(0, 0, 0));
      cool_strip.setPixelColor(j, cool_strip.Color(0, 0, 0));
  }

  warm_strip.show();
  cool_strip.show();
}

void moveCloud(int dir) {
  if (dir > 0) {
    for (int posVal = 0; posVal <= 90; posVal += 1) {
       myservo.write(posVal); // tell servo to go to position in variable 'pos' 
       delay(15);
        Serial.printf("CLOUD OPENING");
       played = 1;
    }
  }
  else {
    for (int posVal = 90; posVal >= 0; posVal -= 1) {
       myservo.write(posVal); // tell servo to go to position in variable 'pos' 
       delay(15);
       played = 0;
    }
  }
}
