
int section(int ledPos) {
  if (ledPos < 13) { 
    return 2;
  }
  else if (ledPos <26) {
    return 4;
  }
  else if (ledPos <39) { 
    return 6;
  }
  else if (ledPos < 52) {
    return 8;
  }

}
int nextRing() {
  for (int i = 0; i < RINGSIZE; i++) {
    if (ring[i][0] == -1) {
      return i;
    }
  }
  return -1;
}


bool testRing(int r) {
  for (int j = 0; j < RINGSIZE; j++) {
    if (connex[ring[j][0]][0] == connex[r][0] or connex[ring[j][0]][0] == connex[r][1] or connex[ring[j][0]][1] == connex[r][1] or connex[ring[j][0]][1] == connex[r][0]) {
      Serial.print("asked for ");
      Serial.print(r);
      Serial.print(" got ");
      Serial.println(j);
      return false;
    }
  }
  return true;
}



int connect(int in, int out, int incolor, int outcolor, int currentStep) {
  //TURN in to zero
  if (currentStep < MAXVAL + 1) {
    leds[in] = CHSV((incolor+section(in)*2)%255, SATURATION, MAXVAL - currentStep);
    currentStep++;
    return currentStep;
  }
  //turn in to red
  else if (currentStep < MAXVAL * 2 + 1) {
    leds[in] = CHSV(outcolor, OUTSAT, currentStep - MAXVAL);
    currentStep++;
    return currentStep;
  }
  //turn out to zero, keep in at red.
  else if (currentStep < MAXVAL * 3 + 1) {
    leds[in] = CHSV(outcolor, OUTSAT, MAXVAL);
    leds[out] = CHSV((incolor+section(in)*2)%255, SATURATION, MAXVAL * 3 - currentStep);
    currentStep++;
    return currentStep;
  }
  //turn out to red, turn in to zero.
  else if (currentStep < MAXVAL * 4 + 1) {
    leds[out] = CHSV(outcolor, OUTSAT, currentStep - MAXVAL * 3);
    leds[in] = CHSV(outcolor, OUTSAT, MAXVAL * 4 - currentStep);
    currentStep++;
    return currentStep;
  }
  //turn in to blue, turn out to zero.
  else if (currentStep < MAXVAL * 5 + 1) {
    leds[in] = CHSV((incolor+section(in)*2)%255, SATURATION, currentStep - MAXVAL * 4);
    leds[out] = CHSV(outcolor, OUTSAT, MAXVAL * 5 - currentStep);
    currentStep++;
    return currentStep;
  }
  //turn out to blue
  else if (currentStep < MAXVAL * 6 + 1) {
    leds[in] = CHSV((incolor+section(in)*2)%255, SATURATION, MAXVAL);
    leds[out] = CHSV((incolor+section(in)*2)%255, SATURATION, currentStep - MAXVAL * 5);
    currentStep++;
    return currentStep;
  }
  else if (currentStep >= MAXVAL * 6 + 1) {
    return -1;
  }
}
