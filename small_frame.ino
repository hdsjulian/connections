#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <FastLED.h>
#define NUM_LEDS 48
#define DATA_PIN D7
#define COLOR_ORDER RGB
#define FRAMES_PER_SECOND 10
CRGB leds[NUM_LEDS];
int lasttime = 0;
int anicounter = 0;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
       FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}


void loop() {
    for (int i = 1; i < 24; i++) {
     leds[i] = CHSV((i+anicounter) % 255, 255, 255);  
     leds[i+24] = CHSV((i+anicounter+128) % 255, 255, 255);
    }
    anicounter = (anicounter + 1) % 255;
    FastLED.show();
    lasttime = millis();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
   
}
