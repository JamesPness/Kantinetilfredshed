#include <Wire.h>
#include "rgb_lcd.h"
#include <Arduino.h>
#include <pins_arduino.h>
#include <math.h>


#define RED_LIGHT 6
#define RED_INPUT 7

#define YELLOW_LIGHT 8
#define YELLOW_INPUT 9

#define GREEN_LIGHT 2
#define GREEN_INPUT 3

rgb_lcd lcd;

short colorR = 0;
short colorG = 255;
short colorB = 0;

short input_delay = 2000;

short buttons[3];
short lights[3];

void setup() {
  Serial.begin(9600);
  while (!Serial);

  buttons[0] = RED_INPUT;
  buttons[1] = YELLOW_INPUT;
  buttons[2] = GREEN_INPUT;

  lights[0] = RED_LIGHT;
  lights[1] = YELLOW_LIGHT;
  lights[2] = GREEN_LIGHT;

  pinMode(RED_LIGHT, OUTPUT);
  pinMode(RED_INPUT, INPUT_PULLUP);

  pinMode(YELLOW_LIGHT, OUTPUT);
  pinMode(YELLOW_INPUT, INPUT_PULLUP);

  pinMode(GREEN_LIGHT, OUTPUT);
  pinMode(GREEN_INPUT, INPUT_PULLUP);

  digitalWrite(RED_LIGHT, LOW);
  digitalWrite(YELLOW_LIGHT, LOW);
  digitalWrite(GREEN_LIGHT, LOW);

  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("Kantine-");
  lcd.setCursor(0,1);
  lcd.print("tilfredshed");

  delay(1500);

  colorR = 255;
  colorG = 255;
  colorB = 255;
}

void loop() {
  for (int i = 0; i<3; i++){
    if(digitalRead(buttons[i]) == HIGH){
      digitalWrite(lights[i], HIGH);
      Serial.println(i);
      if (i == 0) {
        lcd.setRGB(255, 0, 0);
      } else if (i == 1) {
        lcd.setRGB(255, 255, 0);
      } else if (i == 2) {
        lcd.setRGB(0, 255, 0);
      }
    } else {
      digitalWrite(lights[i], LOW);
    }
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Er maden god?");
  lcd.setCursor(0,1);
  lcd.print("Giv os et preg!");

  delay(100);

}