#include <UTFT.h>

UTFT myGLCD(SSD1963_800ALT, 3, 4, 22, 23);

void setup() {
  delay(500);
  myGLCD.InitLCD();
  delay(250);
  myGLCD.clrScr();
  delay(250);
  myGLCD.setColor(0b1111111111111111);
  myGLCD.drawPixel(8, 8);
  delay(500);
}

void loop() {
  delay(1);
}
