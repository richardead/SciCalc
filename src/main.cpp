#include <Arduino.h>
#include <SPIFFS.h>
#include "buzzer.h"
#include "IO.h"
#include "screen.h"
#include "keyboard.h"
#include "menu.h"
#include "apps/calc.h"
#include "apps/badApple.h"
#include "apps/mandelbrot.h"
#include "apps/dziuraWsamolocie.h"


void setup() {
  Serial.begin(115200);
  delay(500);

  if(!SPIFFS.begin(true)) { 
    Serial.println("An Error has occurred while mounting SPIFFS"); 
    return; 
  }

  if(!psramFound()) {
    Serial.println("PSRAM is NOT available!");
    return;
  }

  setupIO();
  initScreen();
  initKeyboard();

  Menu main(true);

  main.addOption("Calculator", calc);
  // main.addOption("Bad Apple", badApple);
  main.addOption("IR", dziuraWSamolocie);

  main.run();
}



void loop() {
  Serial.println("hello, world");
  delay(500);

}






