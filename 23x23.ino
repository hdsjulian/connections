#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <FastLED.h>
// Use True Random from https://github.com/marvinroger/ESP8266TrueRandom
#include <ESP8266TrueRandom.h>
#define NUM_LEDS 52
#define DATA_PIN D7
#define COLOR_ORDER RGB
#define MAXVAL 255
#define SPEEDFACTOR 1
#define SPEEDMODIFIER 1
#define RINGSIZE 15
#define OVERALL_COLORCHANGE_FREQUENCY 20
#define SATURATION 140
#define OUTSAT 255
CRGB leds[NUM_LEDS];
int lasttime = 0;
int anicounter = 0;
int foo = -1;
int r = 0;

int ring[RINGSIZE][3];

int color = 174;
int counterColor = 255;
// nata 
//int connex[52][2] = {{0, 51}, {1, 50}, {2, 49}, {3, 37}, {4, 34}, {5, 36}, {6, 33}, {7, 32}, {8, 35}, {9, 31}, {10, 30}, {11, 29}, {12, 13}, {13, 12}, {14, 15}, {15, 14}, {16, 19}, {17, 18}, {18, 17}, {19, 16}, {20, 28}, {21, 40}, {22, 39}, {23, 38}, {24, 27}, {25, 26}, {26, 25}, {27, 24}, {28, 20}, {29, 11}, {30, 10}, {31, 9}, {32, 7}, {33, 6}, {34, 4}, {35, 8}, {36, 5}, {37, 3}, {38, 23}, {39, 22}, {40, 21}, {41, 48}, {42, 47}, {43, 46}, {44, 45}, {45, 44}, {46, 43}, {47, 42}, {48, 41}, {49, 2}, {50, 1}, {51, 0}};
// sarah 
//int connex[52][2] = {{0, 51}, {1, 50}, {2, 49}, {3, 48}, {4, 21}, {5, 16}, {6, 15}, {7, 14}, {8, 13}, {9, 12}, {10, 11}, {12, 9}, {13, 8}, {14, 7}, {15, 6}, {16, 5}, {17, 47}, {18, 44}, {19, 42}, {20, 43}, {21, 4}, {22, 29}, {23, 28}, {24, 27}, {25, 26}, {26, 25}, {27, 24}, {28, 23}, {29, 22}, {30, 41}, {31, 40}, {32, 46}, {33, 45}, {34, 37}, {35, 36}, {36, 35}, {37, 34}, {38, 39}, {39, 38}, {40, 31}, {41, 30}, {42, 19}, {43, 20}, {44, 18}, {45, 33}, {46, 32}, {47, 17}, {48, 3}, {49, 2}, {50, 1}, {51, 0}};
// dani
//int connex[52][2] = {{0, 1}, {1, 0}, {2, 51}, {3, 50}, {4, 5}, {5, 4}, {6, 33}, {7, 32}, {8, 21}, {9, 20}, {10, 19}, {11, 18}, {12, 13}, {13, 12}, {14, 17}, {15, 16}, {16, 15}, {17, 14}, {18, 11}, {19, 10}, {20, 9}, {21, 8}, {22, 29}, {23, 28}, {24, 27}, {25, 26}, {26, 25}, {27, 24}, {28, 23}, {29, 22, }, {30, 41}, {31, 40}, {32, 7}, {33, 6}, {34, 49}, {35, 47}, {36, 39}, {37, 45}, {38, 44}, {39, 36}, {40, 31}, {41, 30}, {42, 48}, {43, 46}, {44, 38}, {45, 37}, {46, 43}, {47, 35}, {48, 42}, {49, 34}, {50, 3}, {51, 2}};
//tobi
int connex[52][2] = {{0, 51}, {1, 6}, {2, 5}, {3, 4}, {4, 3}, {5, 2}, {6, 1}, {7, 48}, {8, 15}, {9, 14}, {10, 11}, {11, 10}, {12, 13}, {13, 12}, {14, 9, }, {15, 8}, {16, 47}, {17, 46}, {18, 45}, {19, 42}, {20, 41}, {21, 30}, {22, 29}, {23, 28}, {24, 27}, {25, 26}, {26, 25}, {27, 24}, {28, 23, }, {29, 22}, {30, 21}, {31, 32}, {32, 31}, {33, 50}, {34, 49}, {35, 44}, {36, 43}, {37, 40}, {38, 39}, {39, 38}, {40, 37}, {41, 20}, {42, 19}, {43, 36}, {44, 37}, {45, 18}, {46, 17}, {47, 16}, {48, 7}, {49, 34}, {50, 33}, {51, 0}};
int colorCounter = 0;
int rollingCounter = 1;

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
    ring[i][2] = 1;
  }
}

void loop() {
  //fill_solid(leds, NUM_LEDS, CHSV(color, SATURATION, MAXVAL));
  for (int i = 0; i<NUM_LEDS; i++) {
    leds[i] = CHSV((color+section(i)*2)%255, SATURATION, MAXVAL);
  }
  // Chance is one in 40 to create a new connection
  r = ESP8266TrueRandom.random(40);
  if (r == 1) {
    //If the color should change
    colorCounter++;
    if (colorCounter == OVERALL_COLORCHANGE_FREQUENCY) {
      color = (color + 1) % 255;
      counterColor = (counterColor + 1) % 255;
      colorCounter = 0;
    }
    // find out next empty position in ring"
    int nextRingPos = nextRing();
    if (nextRingPos != -1) {
      // if there is an empty place in the ring
      Serial.print ("New empty place found ");
      Serial.println(nextRingPos);
      // chose at which position
      r = ESP8266TrueRandom.random(52);
      if (testRing(r)) {
        //if its empty, then add a new colorchange here
        Serial.print("Added at ");
        Serial.print(r);
        Serial.print(" From ");
        Serial.print(connex[r][0]);
        Serial.print(" To: ");
        Serial.print(connex[r][1]);
        ring[nextRingPos][0] = r;
        ring[nextRingPos][1] = connect(connex[ring[nextRingPos][0]][0], connex[ring[nextRingPos][0]][1], color, counterColor, 0);
        int s = ESP8266TrueRandom.random(10);
        ring[nextRingPos][2] = s + SPEEDMODIFIER;
        Serial.print(" speedmod = ");
        Serial.println(s);
  
      }
      
    }
    else {
      //Serial.println ("Blocked");
    }
  }
  //cycle through the whole ring
  for (int i = 0; i < RINGSIZE; i++) {
    if (ring[i][0] != -1) {
      // change / update color
      if ((rollingCounter % ring[i][2]) == 0) {
        ring[i][1] = connect(connex[ring[i][0]][0], connex[ring[i][0]][1], color, counterColor, ring[i][1]);
      }
      else {
        ring[i][1] = connect(connex[ring[i][0]][0], connex[ring[i][0]][1], color, counterColor, (ring[i][1]-1 < 0) ? 0 : ring[i][1]-1);    
      }
      //if we're done set to -1
      if (ring[i][1] == -1) {
        ring[i][0] = -1;
      }
    }

    
  }
  delay(1 * SPEEDFACTOR);
  FastLED.show();
  rollingCounter = (rollingCounter + 1) % 100;
}
