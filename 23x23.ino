#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <FastLED.h>
#define NUM_LEDS 56
#define DATA_PIN D7
#define COLOR_ORDER RGB
#define FRAMES_PER_SECOND 10
#define MAXVAL 125
#define SPEEDFACTOR 3
CRGB leds[NUM_LEDS];
int lasttime = 0;
int anicounter = 0;
int foo = -1;
int r = 0;
  int connex[51][2] = {{0, 51}, {1, 50}, {2, 49}, {3, 37}, {4, 34}, {5, 36}, {6, 33}, {7, 32}, {8, 35}, {9, 31}, {10, 30}, {11, 29}, {12, 13}, {13, 12}, {14, 15}, {15, 14}, {16, 19}, {17, 18}, {19, 16}, {20, 28}, {21, 40}, {22, 39}, {23, 38}, {24, 27}, {25, 26}, {26, 25}, {27, 24}, {28, 20}, {29, 11}, {30, 10}, {31, 9}, {32, 7}, {33, 6}, {34, 4}, {36, 5}, {37, 3}, {38, 23}, {39, 22}, {40, 21}, {41, 48}, {42, 47}, {43, 46}, {44, 45}, {45, 44}, {46, 43}, {47, 42}, {48, 41}, {49, 2}, {50, 1}, {51, 0}};
  

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
       FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  randomSeed(analogRead(0));
  fill_solid(leds, NUM_LEDS, CHSV(170, 255, MAXVAL));
  FastLED.show();
}

int connect(int in, int out, int incolor, int outcolor, int currentStep) {
  if (currentStep < MAXVAL+1) {
    leds[in] = CHSV(incolor, 255, MAXVAL-currentStep);
    currentStep++;
    return currentStep;
  }
  else if (currentStep < MAXVAL*2+1) { 
    leds[in] = CHSV(outcolor, 255, currentStep-MAXVAL);
    currentStep++;
    return currentStep;
  }
  else if (currentStep < MAXVAL*3+1) { 
    leds[out] = CHSV(incolor, 255, MAXVAL*3-currentStep);
    currentStep++;
    return currentStep;
  }
  else if (currentStep < MAXVAL * 4+1){
   leds[out] = CHSV(outcolor, 255, currentStep-MAXVAL*3);
   leds[in] = CHSV(outcolor, 255, MAXVAL*4-currentStep);
   currentStep++;
   return currentStep;
  }
  else if (currentStep < MAXVAL * 5+1) {
   leds[in] = CHSV(incolor, 255, currentStep-MAXVAL*4);
   leds[out] = CHSV(outcolor, 255, MAXVAL*5-currentStep);
   currentStep++;
   return currentStep;
  }
  else if (currentStep <MAXVAL * 6+1) {
    leds[out] = CHSV(incolor, 255, currentStep-MAXVAL*5);
    currentStep++;
    return currentStep;
  }
  else if (currentStep == MAXVAL*6+1) { 
    return -1;
  }
}

void loop() {
  if (foo == -1) {
    delay(1000);
    r = random(52);
    foo = connect(connex[r][0], connex[r][1], 170, 255, 0);    
  }
  else { 
    foo = connect(connex[r][0], connex[r][1], 170, 255, foo);
    delay(10*SPEEDFACTOR);
  }
  FastLED.show();

}
