#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <FastLED.h>
#define NUM_LEDS 48
#define DATA_PIN D7
#define COLOR_ORDER RGB
#define FRAMES_PER_SECOND 120
CRGB leds[NUM_LEDS];
int lasttime = 0;
int anicounter = 0;
int valTop = 0;
int valBottom = 255;
int topIncrement = 1;
int bottomIncrement = -1;
int topHue = 0;
int bottomHue = 64;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
       FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}


void loop() {
    for (int i = 1; i < 24; i++) {
     leds[i] = CHSV((i*2+topHue) % 255, 255, valTop);  
     leds[i+24] = CHSV((i*2+bottomHue) % 255, 255, valBottom);
    }
    valTop += topIncrement;
    valBottom += bottomIncrement;
    if (valTop == 255) {
      topIncrement = -1;
      
    }
    if (valTop == 0) {
      topIncrement = 1;
      topHue = (topHue + 1) % 255;
    }
    if (valBottom == 255) {
      bottomIncrement = -1;
      
    }
    if (valBottom == 0) {
      bottomIncrement = 1;
      bottomHue = (bottomHue + 1) % 255;
    }
    
    FastLED.show();
    lasttime = millis();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
   
}
