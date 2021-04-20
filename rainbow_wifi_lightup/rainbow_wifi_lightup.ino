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

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    2
#define LED2_PIN   5

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 60

// Declare our NeoPixel strip object:
Adafruit_NeoPixel warm_strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel cool_strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  warm_strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  warm_strip.show();            // Turn OFF all pixels ASAP
  warm_strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  fill(1, 50);
}


// Some functions of our own for creating animated effects -----------------

// function to fill certain percent of strip
void fill(int numFifths, int wait) {
  // number of pixels to fill: numFifths * 12
  // number to fill per color: numFifths * 12 /3 = numFifths * 4
 
  for (int j = 0; j < numFifths * 4; j++) {
    // set first strip: red, orange, yellow
    warm_strip.setPixelColor(j, strip.Color(255, 0, 0)); // 1 - 20: red
    warm_strip.setPixelColor(40-j, strip.Color(255, 60, 0)); // 40 - 21: orange
    warm_strip.setPixelColor(j+41, strip.Color(255, 100, 0)); // 41 - 60: yellow
    warm_strip.show(); 

    // set second strip: green, blue, purple
    cool_strip.setPixelColor(j, strip.Color(0, 255, 0)); // 1 - 20: green
    cool_strip.setPixelColor(40-j, strip.Color(0, 0, 255)); // 40 - 21: blue
    cool_strip.setPixelColor(j+41, strip.Color(200, 0, 255)); // 41 - 60: purple
    cool_strip.show(); 
    delay(wait); 
  }
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
