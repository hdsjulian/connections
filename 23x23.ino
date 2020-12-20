#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <FastLED.h>
// Use True Random from https://github.com/marvinroger/ESP8266TrueRandom
#include <ESP8266TrueRandom.h>
#define NUM_LEDS 56
#define DATA_PIN D7
#define COLOR_ORDER RGB
#define MAXVAL 197du
#define SPEEDFACTOR 15
#define RINGSIZE 12
#define OVERALL_COLORCHANGE_FREQUENCY 5
#define SATURATION 85
CRGB leds[NUM_LEDS];
int lasttime = 0;
int anicounter = 0;
int foo = -1;
int r = 0;

int ring[RINGSIZE][2];

int color = 174;
int counterColor = 170;

int connex[52][2] = {{0, 51},{1, 50},{2, 49}, {3, 37}, {4, 34}, {5, 36}, {6, 33}, {7, 32}, {8, 35}, {9, 31}, {10, 30}, {11, 29}, {12, 13}, {13, 12}, {14, 15}, {15, 14}, {16, 19}, {17, 18}, {18,17}, {19, 16}, {20, 28}, {21, 40}, {22, 39}, {23, 38}, {24, 27}, {25, 26}, {26, 25}, {27, 24}, {28, 20}, {29, 11}, {30, 10}, {31, 9}, {32, 7}, {33, 6}, {34, 4}, {35,8}, {36, 5}, {37, 3}, {38, 23}, {39, 22}, {40, 21}, {41, 48}, {42, 47}, {43, 46}, {44, 45}, {45, 44}, {46, 43}, {47, 42}, {48, 41}, {49, 2}, {50, 1}, {51, 0}};
int colorCounter = 0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
       FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  randomSeed(analogRead(0));
  fill_solid(leds, NUM_LEDS, CHSV(170, 255, MAXVAL));
  FastLED.show();
  for (int i = 0; i < RINGSIZE; i++) {
    ring[i][0] = -1;
    ring[i][1] = -1;
  }
}

int connect(int in, int out, int incolor, int outcolor, int currentStep) {
  //TURN in to zero
  if (currentStep < MAXVAL+1) {
    leds[in] = CHSV(incolor, SATURATION, MAXVAL-currentStep);
    currentStep++;
    return currentStep;
  }
  //turn in to red
  else if (currentStep < MAXVAL*2+1) { 
    leds[in] = CHSV(outcolor, SATURATION, currentStep-MAXVAL);
    currentStep++;
    return currentStep;
  }
  //turn out to zero, keep in at red. 
  else if (currentStep < MAXVAL*3+1) { 
    leds[in] = CHSV(outcolor, SATURATION, MAXVAL);
    leds[out] = CHSV(incolor, SATURATION, MAXVAL*3-currentStep);
    currentStep++;
    return currentStep;
  }
  //turn out to red, turn in to zero.
  else if (currentStep < MAXVAL * 4+1){
   leds[out] = CHSV(outcolor, SATURATION, currentStep-MAXVAL*3);
   leds[in] = CHSV(outcolor, SATURATION, MAXVAL*4-currentStep);
   currentStep++;
   return currentStep;
  }
  //turn in to blue, turn out to zero. 
  else if (currentStep < MAXVAL * 5+1) {
   leds[in] = CHSV(incolor, SATURATION, currentStep-MAXVAL*4);
   leds[out] = CHSV(outcolor, SATURATION, MAXVAL*5-currentStep);
   currentStep++;
   return currentStep;
  }
  //turn out to blue
  else if (currentStep <MAXVAL * 6+1) {
    leds[in] = CHSV(incolor, SATURATION, MAXVAL);
    leds[out] = CHSV(incolor, SATURATION, currentStep-MAXVAL*5);
    currentStep++;
    return currentStep;
  }
  else if (currentStep == MAXVAL*6+1) { 
    return -1;
  }
}

bool testRing(int r) {
  for (int j = 0; j < RINGSIZE; j++) {
    if (ring[j][0] == r or ring[j][1] == r) {
      return false;
    }
  }
  return true;
}

int nextRing() {
  for (int i = 0; i<RINGSIZE;i++) {
    if (ring[i][0] == -1) { 
      return i;
    }
  }
  return -1;
}

void loop() {
  fill_solid(leds, NUM_LEDS, CHSV(color, SATURATION, MAXVAL));
  r = ESP8266TrueRandom.random(40);
  if (r == 1) {
    colorCounter++;
    if (colorCounter == OVERALL_COLORCHANGE_FREQUENCY) {
      color = (color + 1) % 255;
      counterColor = (counterColor + 1) % 255;
      colorCounter = 0;
    }
    //Serial.println("New ");
    int nextRingPos = nextRing();
    if (nextRingPos != -1) { 
      //Serial.print("New Connection at Ringpos: ");
      //Serial.println(nextRingPos);
      r = ESP8266TrueRandom.random(52);
      //Serial.print("Random = ");
      //Serial.println(r);
      if (testRing(r)) {
        ring[nextRingPos][0] = r;
        ring[nextRingPos][1] = connect(connex[ring[nextRingPos][0]][0], connex[ring[nextRingPos][0]][1], color, counterColor, 0);
      } 
    }
    else {
      //Serial.println ("Blocked");
    }
  }
  for (int i = 0; i < RINGSIZE; i++) {
    if (ring[i][0] != -1) {
      ring[i][1] = connect(connex[ring[i][0]][0], connex[ring[i][0]][1], color, counterColor, ring[i][1]);
      if (ring[i][1] == -1) { 
        ring[i][0] = -1;
      }
    }
  }
  delay(1*SPEEDFACTOR);
  FastLED.show();

}
