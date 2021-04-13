#include <Wire.h>      
#include "Adafruit_LEDBackpack.h"       
#include "Adafruit_GFX.h"    

#define PIRSENSOR_PIN       A10
#define TEMPSENSOR_PIN      A7
#define POTENTIOMETER_PIN   A0

#define MATRIX_1_I2C_ADDR   0x70
#define MATRIX_2_I2C_ADDR   0x71
#define MATRIX_3_I2C_ADDR   0x72

Adafruit_8x8matrix m1 = Adafruit_8x8matrix();      
Adafruit_8x8matrix m2 = Adafruit_8x8matrix();   
Adafruit_8x8matrix m3 = Adafruit_8x8matrix();   

void setup() {       
 Serial.begin(9600);       
       
 Serial.println("8x8 LED Matrix Test");        
 
 initDisplays();
}       
       
void loop() { 
  int sector = getPotentioSector();
  clearAllScreens();
  if(sector == 0){ // "Setting" 1
    // initializeren van schermpjes zodat je ze op de goede plek kan zetten
    initDisplays();
    screenNumbers();
    delay(1500);
  
  } else if (sector < 3){ // "Setting" 2
    // altijd uitlezen van temperatuur sensor en altijd gemeten temperatuur laten zien op matrix displays
    drawTemperature(readTempSensor());
    delay(2000);
        
  } else { // "Setting 3"
    // uitlezen van PIR sensor om te kijken of er iemand in de buurt is, zo ja meet de temperatuur en laat deze zien op het schermpje. Dit doet hij met een variabele refreshrate bepaald door de potentio meter
    if(readPIRSensor()){
      clearAllScreens();
      double temp1 = readTempSensor();
      drawTemperature(temp1);
      }
     delay(getRefreshDelay(sector));
     
  }
}

bool readPIRSensor(){ // Leest de PIR sensor uit
  bool pir = analogRead(PIRSENSOR_PIN);
  Serial.print("PIR sensor: ");
  Serial.println(pir);
  Serial.println();
  return pir;
}

double readTempSensor(){ // Leest de temperatuur sensor uit
  float val = analogRead(TEMPSENSOR_PIN);
  //double temp = (val/1024.0)*500;
  
  double temp = val*5000/(1024*10);
  Serial.print("temp: ");
  Serial.print(temp);
  Serial.println();
  return temp;
}

int readPotentiometer(){ // Leest de potentiometer uit
  int val = analogRead(POTENTIOMETER_PIN);
  Serial.print("potentiometer: ");
  Serial.print(val);
  Serial.println();
  return val;
}

int getPotentioSector(){ // Geeft een waarde terug die is berekend door de waarde van de potentiometer te delen door 128, hierdoor komt een getal tussen 0 en 7 terug wat gebruikt kan worden als instelling selector.
  return readPotentiometer()/128;
}

int getRefreshDelay(int sector){// Geeft een waarde terug om als delay te setten voor de verschillende sectoren waar dit nodig is
  switch (sector){
    case 4: return 10000;
    case 5: return 5000;
    case 6: return 2500;
    case 7: return 500;
  }
}

void initDisplays(){ // Displays opstarten en op goede orientatie zetten
  m1.begin(MATRIX_1_I2C_ADDR);
  m2.begin(MATRIX_2_I2C_ADDR);
  m3.begin(MATRIX_3_I2C_ADDR);  
  
  m1.setRotation(3);
  m2.setRotation(3);
  m3.setRotation(3);

  m1.writeDisplay();
  m2.writeDisplay();
  m3.writeDisplay();
}

void screenNumbers(){ // Op de schermen het nummer laten zien in welke volgorde deze moeten staan zodat de schermen op de goede plek gezet kunnen worden
  drawNumber('1', 0, 0);
  drawNumber('2', 8, 0);
  drawNumber('3', 16, 0);
}

void clearAllScreens(){ // Maak alle schermen leeg
  m1.clear();
  m2.clear();
  m3.clear();
  m1.writeDisplay();
  m2.writeDisplay();
  m3.writeDisplay();
}

void writePixel3Screen(int x, int y, int on){ // Teken een pixel op de 3 schermen
  if (x < 8){
    m1.drawPixel(x, y, on);
    m1.writeDisplay();
  }else if(x > 7 && x < 16){
    m2.drawPixel(x - 8, y, on);
    m2.writeDisplay();
  }else{
    m3.drawPixel(x - 16, y, on);
    m3.writeDisplay();
  }
}

void drawTemperature(double temp) { // Teken een gemeten temperatuur op de schermen
  String t1 = String(temp, 1);
  char d1 = t1[0];
  //Serial.print("d1: ");
  //Serial.print(d1);
  char d2 = t1[1];
  //Serial.print(", d2: ");
  //Serial.print(d2);
  char d3 = t1[3];
  //Serial.print(", d3: ");
  //Serial.print(d3);
  //Serial.println();
  draw3Digits(d1, d2, d3);
}

void draw3Digits(char d1, char d2, char d3){ // Teken een getal met een punt na de eerste 2 getallen en een celsius teken aan het eind
  drawNumber(d1, 0, 0);
  drawNumber(d2, 6, 0);
  drawPoint(12,0);
  drawNumber(d3, 14, 0);
  drawCelsius(20, 0);
}

void drawNumber(char number, int startX, int startY){ // Teken 1 nummer
  switch(number){
    case '0': draw0(startX, startY); break;
    case '1': draw1(startX, startY); break;
    case '2': draw2(startX, startY); break;
    case '3': draw3(startX, startY); break;
    case '4': draw4(startX, startY); break;
    case '5': draw5(startX, startY); break;
    case '6': draw6(startX, startY); break;
    case '7': draw7(startX, startY); break;
    case '8': draw8(startX, startY); break;
    case '9': draw9(startX, startY); break;
  }
}



//Alle methodes hieronder zijn hulpmethodes om de getallen en tekens te tekenen op de verschillende schermen

void draw1(int startX, int startY){
  writePixel3Screen(startX + 0, startY + 0, 0); // row 0
  writePixel3Screen(startX + 1, startY + 0, 0);
  writePixel3Screen(startX + 2, startY + 0, 1);
  writePixel3Screen(startX + 3, startY + 0, 0);
  writePixel3Screen(startX + 4, startY + 0, 0);

  writePixel3Screen(startX + 0, startY + 1, 0); // row 1
  writePixel3Screen(startX + 1, startY + 1, 1);
  writePixel3Screen(startX + 2, startY + 1, 1);
  writePixel3Screen(startX + 3, startY + 1, 0);
  writePixel3Screen(startX + 4, startY + 1, 0);

  writePixel3Screen(startX + 0, startY + 2, 0); // row 2
  writePixel3Screen(startX + 1, startY + 2, 0);
  writePixel3Screen(startX + 2, startY + 2, 1);
  writePixel3Screen(startX + 3, startY + 2, 0);
  writePixel3Screen(startX + 4, startY + 2, 0);

  writePixel3Screen(startX + 0, startY + 3, 0); // row 3
  writePixel3Screen(startX + 1, startY + 3, 0);
  writePixel3Screen(startX + 2, startY + 3, 1);
  writePixel3Screen(startX + 3, startY + 3, 0);
  writePixel3Screen(startX + 4, startY + 3, 0);

  writePixel3Screen(startX + 0, startY + 4, 0); // row 4
  writePixel3Screen(startX + 1, startY + 4, 0);
  writePixel3Screen(startX + 2, startY + 4, 1);
  writePixel3Screen(startX + 3, startY + 4, 0);
  writePixel3Screen(startX + 4, startY + 4, 0);

  writePixel3Screen(startX + 0, startY + 5, 0); // row 5
  writePixel3Screen(startX + 1, startY + 5, 0);
  writePixel3Screen(startX + 2, startY + 5, 1);
  writePixel3Screen(startX + 3, startY + 5, 0);
  writePixel3Screen(startX + 4, startY + 5, 0);

  writePixel3Screen(startX + 0, startY + 6, 0); // row 6
  writePixel3Screen(startX + 1, startY + 6, 1);
  writePixel3Screen(startX + 2, startY + 6, 1);
  writePixel3Screen(startX + 3, startY + 6, 1);
  writePixel3Screen(startX + 4, startY + 6, 0);
}

void draw2(int startX, int startY) {
  writePixel3Screen(startX + 0, startY + 0, 0); // row 0
  writePixel3Screen(startX + 1, startY + 0, 1);
  writePixel3Screen(startX + 2, startY + 0, 1);
  writePixel3Screen(startX + 3, startY + 0, 1);
  writePixel3Screen(startX + 4, startY + 0, 0);

  writePixel3Screen(startX + 0, startY + 1, 1); // row 1
  writePixel3Screen(startX + 1, startY + 1, 0);
  writePixel3Screen(startX + 2, startY + 1, 0);
  writePixel3Screen(startX + 3, startY + 1, 0);
  writePixel3Screen(startX + 4, startY + 1, 1);

  writePixel3Screen(startX + 0, startY + 2, 0); // row 2
  writePixel3Screen(startX + 1, startY + 2, 0);
  writePixel3Screen(startX + 2, startY + 2, 0);
  writePixel3Screen(startX + 3, startY + 2, 0);
  writePixel3Screen(startX + 4, startY + 2, 1);

  writePixel3Screen(startX + 0, startY + 3, 0); // row 3
  writePixel3Screen(startX + 1, startY + 3, 1);
  writePixel3Screen(startX + 2, startY + 3, 1);
  writePixel3Screen(startX + 3, startY + 3, 1);
  writePixel3Screen(startX + 4, startY + 3, 0);

  writePixel3Screen(startX + 0, startY + 4, 1); // row 4
  writePixel3Screen(startX + 1, startY + 4, 0);
  writePixel3Screen(startX + 2, startY + 4, 0);
  writePixel3Screen(startX + 3, startY + 4, 0);
  writePixel3Screen(startX + 4, startY + 4, 0);

  writePixel3Screen(startX + 0, startY + 5, 1); // row 5
  writePixel3Screen(startX + 1, startY + 5, 0);
  writePixel3Screen(startX + 2, startY + 5, 0);
  writePixel3Screen(startX + 3, startY + 5, 0);
  writePixel3Screen(startX + 4, startY + 5, 0);

  writePixel3Screen(startX + 0, startY + 6, 1); // row 6
  writePixel3Screen(startX + 1, startY + 6, 1);
  writePixel3Screen(startX + 2, startY + 6, 1);
  writePixel3Screen(startX + 3, startY + 6, 1);
  writePixel3Screen(startX + 4, startY + 6, 1);
}

void draw3(int startX, int startY) {
  writePixel3Screen(startX + 0, startY + 0, 1); // row 0
  writePixel3Screen(startX + 1, startY + 0, 1);
  writePixel3Screen(startX + 2, startY + 0, 1);
  writePixel3Screen(startX + 3, startY + 0, 1);
  writePixel3Screen(startX + 4, startY + 0, 1);

  writePixel3Screen(startX + 0, startY + 1, 0); // row 1
  writePixel3Screen(startX + 1, startY + 1, 0);
  writePixel3Screen(startX + 2, startY + 1, 0);
  writePixel3Screen(startX + 3, startY + 1, 0);
  writePixel3Screen(startX + 4, startY + 1, 1);

  writePixel3Screen(startX + 0, startY + 2, 0); // row 2
  writePixel3Screen(startX + 1, startY + 2, 0);
  writePixel3Screen(startX + 2, startY + 2, 0);
  writePixel3Screen(startX + 3, startY + 2, 1);
  writePixel3Screen(startX + 4, startY + 2, 0);

  writePixel3Screen(startX + 0, startY + 3, 0); // row 3
  writePixel3Screen(startX + 1, startY + 3, 0);
  writePixel3Screen(startX + 2, startY + 3, 1);
  writePixel3Screen(startX + 3, startY + 3, 1);
  writePixel3Screen(startX + 4, startY + 3, 0);

  writePixel3Screen(startX + 0, startY + 4, 1); // row 4
  writePixel3Screen(startX + 1, startY + 4, 0);
  writePixel3Screen(startX + 2, startY + 4, 0);
  writePixel3Screen(startX + 3, startY + 4, 0);
  writePixel3Screen(startX + 4, startY + 4, 1);

  writePixel3Screen(startX + 0, startY + 5, 1); // row 5
  writePixel3Screen(startX + 1, startY + 5, 0);
  writePixel3Screen(startX + 2, startY + 5, 0);
  writePixel3Screen(startX + 3, startY + 5, 0);
  writePixel3Screen(startX + 4, startY + 5, 1);

  writePixel3Screen(startX + 0, startY + 6, 0); // row 6
  writePixel3Screen(startX + 1, startY + 6, 1);
  writePixel3Screen(startX + 2, startY + 6, 1);
  writePixel3Screen(startX + 3, startY + 6, 1);
  writePixel3Screen(startX + 4, startY + 6, 0);
}

void draw4(int startX, int startY) {
  writePixel3Screen(startX + 0, startY + 0, 0); // row 0
  writePixel3Screen(startX + 1, startY + 0, 0);
  writePixel3Screen(startX + 2, startY + 0, 0);
  writePixel3Screen(startX + 3, startY + 0, 1);
  writePixel3Screen(startX + 4, startY + 0, 0);

  writePixel3Screen(startX + 0, startY + 1, 0); // row 1
  writePixel3Screen(startX + 1, startY + 1, 0);
  writePixel3Screen(startX + 2, startY + 1, 1);
  writePixel3Screen(startX + 3, startY + 1, 1);
  writePixel3Screen(startX + 4, startY + 1, 0);

  writePixel3Screen(startX + 0, startY + 2, 0); // row 2
  writePixel3Screen(startX + 1, startY + 2, 1);
  writePixel3Screen(startX + 2, startY + 2, 0);
  writePixel3Screen(startX + 3, startY + 2, 1);
  writePixel3Screen(startX + 4, startY + 2, 0);

  writePixel3Screen(startX + 0, startY + 3, 1); // row 3
  writePixel3Screen(startX + 1, startY + 3, 0);
  writePixel3Screen(startX + 2, startY + 3, 0);
  writePixel3Screen(startX + 3, startY + 3, 1);
  writePixel3Screen(startX + 4, startY + 3, 0);

  writePixel3Screen(startX + 0, startY + 4, 1); // row 4
  writePixel3Screen(startX + 1, startY + 4, 1);
  writePixel3Screen(startX + 2, startY + 4, 1);
  writePixel3Screen(startX + 3, startY + 4, 1);
  writePixel3Screen(startX + 4, startY + 4, 1);

  writePixel3Screen(startX + 0, startY + 5, 0); // row 5
  writePixel3Screen(startX + 1, startY + 5, 0);
  writePixel3Screen(startX + 2, startY + 5, 0);
  writePixel3Screen(startX + 3, startY + 5, 1);
  writePixel3Screen(startX + 4, startY + 5, 0);

  writePixel3Screen(startX + 0, startY + 6, 0); // row 6
  writePixel3Screen(startX + 1, startY + 6, 0);
  writePixel3Screen(startX + 2, startY + 6, 0);
  writePixel3Screen(startX + 3, startY + 6, 1);
  writePixel3Screen(startX + 4, startY + 6, 0);
}

void draw5(int startX, int startY) {
  writePixel3Screen(startX + 0, startY + 0, 1); // row 0
  writePixel3Screen(startX + 1, startY + 0, 1);
  writePixel3Screen(startX + 2, startY + 0, 1);
  writePixel3Screen(startX + 3, startY + 0, 1);
  writePixel3Screen(startX + 4, startY + 0, 1);

  writePixel3Screen(startX + 0, startY + 1, 1); // row 1
  writePixel3Screen(startX + 1, startY + 1, 0);
  writePixel3Screen(startX + 2, startY + 1, 0);
  writePixel3Screen(startX + 3, startY + 1, 0);
  writePixel3Screen(startX + 4, startY + 1, 0);

  writePixel3Screen(startX + 0, startY + 2, 1); // row 2
  writePixel3Screen(startX + 1, startY + 2, 1);
  writePixel3Screen(startX + 2, startY + 2, 1);
  writePixel3Screen(startX + 3, startY + 2, 1);
  writePixel3Screen(startX + 4, startY + 2, 0);

  writePixel3Screen(startX + 0, startY + 3, 0); // row 3
  writePixel3Screen(startX + 1, startY + 3, 0);
  writePixel3Screen(startX + 2, startY + 3, 0);
  writePixel3Screen(startX + 3, startY + 3, 0);
  writePixel3Screen(startX + 4, startY + 3, 1);

  writePixel3Screen(startX + 0, startY + 4, 0); // row 4
  writePixel3Screen(startX + 1, startY + 4, 0);
  writePixel3Screen(startX + 2, startY + 4, 0);
  writePixel3Screen(startX + 3, startY + 4, 0);
  writePixel3Screen(startX + 4, startY + 4, 1);

  writePixel3Screen(startX + 0, startY + 5, 1); // row 5
  writePixel3Screen(startX + 1, startY + 5, 0);
  writePixel3Screen(startX + 2, startY + 5, 0);
  writePixel3Screen(startX + 3, startY + 5, 0);
  writePixel3Screen(startX + 4, startY + 5, 1);

  writePixel3Screen(startX + 0, startY + 6, 0); // row 6
  writePixel3Screen(startX + 1, startY + 6, 1);
  writePixel3Screen(startX + 2, startY + 6, 1);
  writePixel3Screen(startX + 3, startY + 6, 1);
  writePixel3Screen(startX + 4, startY + 6, 0);
}

void draw6(int startX, int startY) {
  writePixel3Screen(startX + 0, startY + 0, 0); // row 0
  writePixel3Screen(startX + 1, startY + 0, 0);
  writePixel3Screen(startX + 2, startY + 0, 1);
  writePixel3Screen(startX + 3, startY + 0, 1);
  writePixel3Screen(startX + 4, startY + 0, 1);

  writePixel3Screen(startX + 0, startY + 1, 0); // row 1
  writePixel3Screen(startX + 1, startY + 1, 1);
  writePixel3Screen(startX + 2, startY + 1, 0);
  writePixel3Screen(startX + 3, startY + 1, 0);
  writePixel3Screen(startX + 4, startY + 1, 0);

  writePixel3Screen(startX + 0, startY + 2, 1); // row 2
  writePixel3Screen(startX + 1, startY + 2, 0);
  writePixel3Screen(startX + 2, startY + 2, 0);
  writePixel3Screen(startX + 3, startY + 2, 0);
  writePixel3Screen(startX + 4, startY + 2, 0);

  writePixel3Screen(startX + 0, startY + 3, 1); // row 3
  writePixel3Screen(startX + 1, startY + 3, 1);
  writePixel3Screen(startX + 2, startY + 3, 1);
  writePixel3Screen(startX + 3, startY + 3, 1);
  writePixel3Screen(startX + 4, startY + 3, 0);

  writePixel3Screen(startX + 0, startY + 4, 1); // row 4
  writePixel3Screen(startX + 1, startY + 4, 0);
  writePixel3Screen(startX + 2, startY + 4, 0);
  writePixel3Screen(startX + 3, startY + 4, 0);
  writePixel3Screen(startX + 4, startY + 4, 1);

  writePixel3Screen(startX + 0, startY + 5, 1); // row 5
  writePixel3Screen(startX + 1, startY + 5, 0);
  writePixel3Screen(startX + 2, startY + 5, 0);
  writePixel3Screen(startX + 3, startY + 5, 0);
  writePixel3Screen(startX + 4, startY + 5, 1);

  writePixel3Screen(startX + 0, startY + 6, 0); // row 6
  writePixel3Screen(startX + 1, startY + 6, 1);
  writePixel3Screen(startX + 2, startY + 6, 1);
  writePixel3Screen(startX + 3, startY + 6, 1);
  writePixel3Screen(startX + 4, startY + 6, 0);
}

void draw7(int startX, int startY) {
  writePixel3Screen(startX + 0, startY + 0, 1); // row 0
  writePixel3Screen(startX + 1, startY + 0, 1);
  writePixel3Screen(startX + 2, startY + 0, 1);
  writePixel3Screen(startX + 3, startY + 0, 1);
  writePixel3Screen(startX + 4, startY + 0, 1);

  writePixel3Screen(startX + 0, startY + 1, 0); // row 1
  writePixel3Screen(startX + 1, startY + 1, 0);
  writePixel3Screen(startX + 2, startY + 1, 0);
  writePixel3Screen(startX + 3, startY + 1, 0);
  writePixel3Screen(startX + 4, startY + 1, 1);

  writePixel3Screen(startX + 0, startY + 2, 0); // row 2
  writePixel3Screen(startX + 1, startY + 2, 0);
  writePixel3Screen(startX + 2, startY + 2, 0);
  writePixel3Screen(startX + 3, startY + 2, 0);
  writePixel3Screen(startX + 4, startY + 2, 1);

  writePixel3Screen(startX + 0, startY + 3, 0); // row 3
  writePixel3Screen(startX + 1, startY + 3, 0);
  writePixel3Screen(startX + 2, startY + 3, 0);
  writePixel3Screen(startX + 3, startY + 3, 1);
  writePixel3Screen(startX + 4, startY + 3, 0);

  writePixel3Screen(startX + 0, startY + 4, 0); // row 4
  writePixel3Screen(startX + 1, startY + 4, 0);
  writePixel3Screen(startX + 2, startY + 4, 1);
  writePixel3Screen(startX + 3, startY + 4, 0);
  writePixel3Screen(startX + 4, startY + 4, 0);

  writePixel3Screen(startX + 0, startY + 5, 0); // row 5
  writePixel3Screen(startX + 1, startY + 5, 1);
  writePixel3Screen(startX + 2, startY + 5, 0);
  writePixel3Screen(startX + 3, startY + 5, 0);
  writePixel3Screen(startX + 4, startY + 5, 0);

  writePixel3Screen(startX + 0, startY + 6, 1); // row 6
  writePixel3Screen(startX + 1, startY + 6, 0);
  writePixel3Screen(startX + 2, startY + 6, 0);
  writePixel3Screen(startX + 3, startY + 6, 0);
  writePixel3Screen(startX + 4, startY + 6, 0);
}

void draw8(int startX, int startY) {
  writePixel3Screen(startX + 0, startY + 0, 0); // row 0
  writePixel3Screen(startX + 1, startY + 0, 1);
  writePixel3Screen(startX + 2, startY + 0, 1);
  writePixel3Screen(startX + 3, startY + 0, 1);
  writePixel3Screen(startX + 4, startY + 0, 0);

  writePixel3Screen(startX + 0, startY + 1, 1); // row 1
  writePixel3Screen(startX + 1, startY + 1, 0);
  writePixel3Screen(startX + 2, startY + 1, 0);
  writePixel3Screen(startX + 3, startY + 1, 0);
  writePixel3Screen(startX + 4, startY + 1, 1);

  writePixel3Screen(startX + 0, startY + 2, 1); // row 2
  writePixel3Screen(startX + 1, startY + 2, 0);
  writePixel3Screen(startX + 2, startY + 2, 0);
  writePixel3Screen(startX + 3, startY + 2, 0);
  writePixel3Screen(startX + 4, startY + 2, 1);

  writePixel3Screen(startX + 0, startY + 3, 0); // row 3
  writePixel3Screen(startX + 1, startY + 3, 1);
  writePixel3Screen(startX + 2, startY + 3, 1);
  writePixel3Screen(startX + 3, startY + 3, 1);
  writePixel3Screen(startX + 4, startY + 3, 0);

  writePixel3Screen(startX + 0, startY + 4, 1); // row 4
  writePixel3Screen(startX + 1, startY + 4, 0);
  writePixel3Screen(startX + 2, startY + 4, 0);
  writePixel3Screen(startX + 3, startY + 4, 0);
  writePixel3Screen(startX + 4, startY + 4, 1);

  writePixel3Screen(startX + 0, startY + 5, 1); // row 5
  writePixel3Screen(startX + 1, startY + 5, 0);
  writePixel3Screen(startX + 2, startY + 5, 0);
  writePixel3Screen(startX + 3, startY + 5, 0);
  writePixel3Screen(startX + 4, startY + 5, 1);

  writePixel3Screen(startX + 0, startY + 6, 0); // row 6
  writePixel3Screen(startX + 1, startY + 6, 1);
  writePixel3Screen(startX + 2, startY + 6, 1);
  writePixel3Screen(startX + 3, startY + 6, 1);
  writePixel3Screen(startX + 4, startY + 6, 0);
}

void draw9(int startX, int startY) {
  writePixel3Screen(startX + 0, startY + 0, 0); // row 0
  writePixel3Screen(startX + 1, startY + 0, 1);
  writePixel3Screen(startX + 2, startY + 0, 1);
  writePixel3Screen(startX + 3, startY + 0, 1);
  writePixel3Screen(startX + 4, startY + 0, 0);

  writePixel3Screen(startX + 0, startY + 1, 1); // row 1
  writePixel3Screen(startX + 1, startY + 1, 0);
  writePixel3Screen(startX + 2, startY + 1, 0);
  writePixel3Screen(startX + 3, startY + 1, 0);
  writePixel3Screen(startX + 4, startY + 1, 1);

  writePixel3Screen(startX + 0, startY + 2, 1); // row 2
  writePixel3Screen(startX + 1, startY + 2, 0);
  writePixel3Screen(startX + 2, startY + 2, 0);
  writePixel3Screen(startX + 3, startY + 2, 0);
  writePixel3Screen(startX + 4, startY + 2, 1);

  writePixel3Screen(startX + 0, startY + 3, 0); // row 3
  writePixel3Screen(startX + 1, startY + 3, 1);
  writePixel3Screen(startX + 2, startY + 3, 1);
  writePixel3Screen(startX + 3, startY + 3, 1);
  writePixel3Screen(startX + 4, startY + 3, 1);

  writePixel3Screen(startX + 0, startY + 4, 0); // row 4
  writePixel3Screen(startX + 1, startY + 4, 0);
  writePixel3Screen(startX + 2, startY + 4, 0);
  writePixel3Screen(startX + 3, startY + 4, 0);
  writePixel3Screen(startX + 4, startY + 4, 1);

  writePixel3Screen(startX + 0, startY + 5, 0); // row 5
  writePixel3Screen(startX + 1, startY + 5, 0);
  writePixel3Screen(startX + 2, startY + 5, 0);
  writePixel3Screen(startX + 3, startY + 5, 1);
  writePixel3Screen(startX + 4, startY + 5, 0);

  writePixel3Screen(startX + 0, startY + 6, 1); // row 6
  writePixel3Screen(startX + 1, startY + 6, 1);
  writePixel3Screen(startX + 2, startY + 6, 1);
  writePixel3Screen(startX + 3, startY + 6, 0);
  writePixel3Screen(startX + 4, startY + 6, 0);
}

void draw0(int startX, int startY) {
  writePixel3Screen(startX + 0, startY + 0, 0); // row 0
  writePixel3Screen(startX + 1, startY + 0, 1);
  writePixel3Screen(startX + 2, startY + 0, 1);
  writePixel3Screen(startX + 3, startY + 0, 1);
  writePixel3Screen(startX + 4, startY + 0, 0);

  writePixel3Screen(startX + 0, startY + 1, 1); // row 1
  writePixel3Screen(startX + 1, startY + 1, 0);
  writePixel3Screen(startX + 2, startY + 1, 0);
  writePixel3Screen(startX + 3, startY + 1, 0);
  writePixel3Screen(startX + 4, startY + 1, 1);

  writePixel3Screen(startX + 0, startY + 2, 1); // row 2
  writePixel3Screen(startX + 1, startY + 2, 0);
  writePixel3Screen(startX + 2, startY + 2, 0);
  writePixel3Screen(startX + 3, startY + 2, 1);
  writePixel3Screen(startX + 4, startY + 2, 1);

  writePixel3Screen(startX + 0, startY + 3, 1); // row 3
  writePixel3Screen(startX + 1, startY + 3, 0);
  writePixel3Screen(startX + 2, startY + 3, 1);
  writePixel3Screen(startX + 3, startY + 3, 0);
  writePixel3Screen(startX + 4, startY + 3, 1);

  writePixel3Screen(startX + 0, startY + 4, 1); // row 4
  writePixel3Screen(startX + 1, startY + 4, 1);
  writePixel3Screen(startX + 2, startY + 4, 0);
  writePixel3Screen(startX + 3, startY + 4, 0);
  writePixel3Screen(startX + 4, startY + 4, 1);

  writePixel3Screen(startX + 0, startY + 5, 1); // row 5
  writePixel3Screen(startX + 1, startY + 5, 0);
  writePixel3Screen(startX + 2, startY + 5, 0);
  writePixel3Screen(startX + 3, startY + 5, 0);
  writePixel3Screen(startX + 4, startY + 5, 1);

  writePixel3Screen(startX + 0, startY + 6, 0); // row 6
  writePixel3Screen(startX + 1, startY + 6, 1);
  writePixel3Screen(startX + 2, startY + 6, 1);
  writePixel3Screen(startX + 3, startY + 6, 1);
  writePixel3Screen(startX + 4, startY + 6, 0);
}

void drawPoint(int startX, int startY) {
  writePixel3Screen(startX + 0, startY + 6, 1);
}

void drawCelsius(int startX, int startY) {
  writePixel3Screen(startX + 0, startY + 0, 0); // row 0
  writePixel3Screen(startX + 1, startY + 0, 1);
  writePixel3Screen(startX + 2, startY + 0, 1);
  writePixel3Screen(startX + 3, startY + 0, 0);

  writePixel3Screen(startX + 0, startY + 1, 1); // row 1
  writePixel3Screen(startX + 1, startY + 1, 0);
  writePixel3Screen(startX + 2, startY + 1, 0);
  writePixel3Screen(startX + 3, startY + 1, 1);

  writePixel3Screen(startX + 0, startY + 2, 1); // row 2
  writePixel3Screen(startX + 1, startY + 2, 0);
  writePixel3Screen(startX + 2, startY + 2, 0);
  writePixel3Screen(startX + 3, startY + 2, 1);

  writePixel3Screen(startX + 0, startY + 3, 0); // row 3
  writePixel3Screen(startX + 1, startY + 3, 1);
  writePixel3Screen(startX + 2, startY + 3, 1);
  writePixel3Screen(startX + 3, startY + 3, 0);
}
