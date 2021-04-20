// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"
#include <Wire.h>
#include <string>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    2 // warm colors/strip1
//#define LED2_PIN   5 // cool colors/strip2

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 60

const char* ssid = "Guest Network";
const char* password = "Guest_pw";
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  //  port to listen on
char incomingPacket[255];  // buffer for incoming packets


// Declare our NeoPixel strip object:
Adafruit_NeoPixel warm_strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel cool_strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------
int score;

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  // INITIALIZE NeoPixel strip objects (wartm and cool)
  warm_strip.begin(); 
  //cool_strip.begin();

  // Turn OFF all pixels at start
  warm_strip.show(); 
  //cool_strip.show();   

  // Set BRIGHTNESS to about 1/5 (max = 255)
  warm_strip.setBrightness(50); 
  //cool_strip.setBrightness(50); 

  score = 0;

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

  Serial.println("Connected to wifi");
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

 
  // we recv one packet from the remote so we can know its IP and port
  /*bool readPacket = false;
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
    } 
  }*/
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
      Serial.printf("UDP packet contents (should be a number score): %s\n", incomingPacket);
      readPacket = true;
      //sscanf(incomingPacket, "%d", &score) 
      //score = (int)incomingPacket;
      score = score + 1;
      Serial.printf("SCORE: %d\n", score);
      fill(score, 50);
    } 
  }
  //fill(score, 50);
  //delay(500);
}


// Some functions of our own for creating animated effects -----------------

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
    //cool_strip.setPixelColor(j, strip.Color(0, 255, 0)); // 1 - 20: green
    //cool_strip.setPixelColor(40-j, strip.Color(0, 0, 255)); // 40 - 21: blue
    //cool_strip.setPixelColor(j+41, strip.Color(200, 0, 255)); // 41 - 60: purple
    //cool_strip.show(); 
    delay(wait); 
  }
}
