#include <PhiOT.h>

PhiOT morse("1002");//your application token

void setup() {

  Serial.begin(115200);
  morse.setup_wifi();
  
}

void loop() {
 
  morse.phiLoop();
  
}