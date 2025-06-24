#include <Wire.h>
#include "rgb_lcd.h"
#include <Arduino.h>
#include <pins_arduino.h>
#include <math.h>
#include <string.h>

// Sæt et navn til de forskellige pins for at undgp forvirring
#define RED_LIGHT 6
#define RED_INPUT 7

#define YELLOW_LIGHT 8
#define YELLOW_INPUT 9

#define GREEN_LIGHT 2
#define GREEN_INPUT 3

// Fordi knapperne er mærkelige, så tænder man dem ved at sætte dem på LOW og slukker dem på HIGH
// Ydermere er inputet by deafult HIGH men hvis man trykker er det LOW
// For ikke at undgp forrviring er de traditionelle HIGH og LOW byttet ud med OFF og ON

#define ON LOW
#define OFF HIGH

byte AE[8] = {
  0b00000,
  0b00000,  //
  0b01110,  //  ***
  0b00001,  //     *
  0b01111,  //  ****
  0b10101,  // * * *
  0b11101,  // *** *
  0b00000   //    
};

// Initialiser de brugte functioner
bool two_button_press_check();
void two_button_press_warning();

short check_button_press();
void confirm_button_press(short button);

// Initialiser de brugte variabler
rgb_lcd lcd;

short input_delay = 2000;

short buttons[3] = {RED_INPUT, YELLOW_INPUT, GREEN_INPUT}; // En liste med alle knappernes input pins
short lights[3] = {RED_LIGHT, YELLOW_LIGHT, GREEN_LIGHT}; // En liste med alle lysenes output pins

// En liste med farverne der skal bruges til at vise tilfredshed
short colors[3][3] = {
  {255, 50, 50}, // Rød
  {252, 186, 3}, // Gul
  {50, 255, 50}  // Grøn
};

// Setup - Kode der sørger for yderligererr opsætning af variabler
void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Opsæt pinmode for lys og knapper
  pinMode(RED_LIGHT, OUTPUT);
  pinMode(RED_INPUT, INPUT_PULLUP);

  pinMode(YELLOW_LIGHT, OUTPUT);
  pinMode(YELLOW_INPUT, INPUT_PULLUP);

  pinMode(GREEN_LIGHT, OUTPUT);
  pinMode(GREEN_INPUT, INPUT_PULLUP);

  digitalWrite(RED_LIGHT, LOW);
  digitalWrite(YELLOW_LIGHT, LOW);
  digitalWrite(GREEN_LIGHT, LOW);

  // Initialiser LCD-skærmen
  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 255);
  lcd.setCursor(0,0);
  lcd.print("Kantine-");
  lcd.setCursor(0,1);
  lcd.print("tilfredshed");

  lcd.createChar(0, AE); // Opret et tegn for æ, fordi der ikke findes dansk bugstaver i LCD-biblioteket


  // Vent i halvandet sekund for at give brugeren tid til at læse beskeden
  delay(1500);
}

// Variabel der holder styr kombinationen af knapper der er trykket på
// -1 betyder at ingen knapperne trykkes
// -2 betyder at der er trykket på to eller flere knapper
short button_pressed = -1;

// Loop - Kode der kører hele tiden
// Denne funktion kører hele tiden og tjekker om der er trykket på en knap
void loop() {
  // Opdater knap-tilstande
  button_pressed = check_button_press();


  if (button_pressed == -1){ // Hvis ingen knapper er trykket på
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Er maden god?");
    lcd.setCursor(0,1);
    lcd.print("Giv os et preg!");
    lcd.setRGB(255,255,255);

  } else if (button_pressed == -2){ // Hvis der er trykket på to eller flere knapper
    two_button_press_warning();

  } else { // Hvis der er trykket på en knap
    confirm_button_press(button_pressed);
  }
  // Vent i 100 millisekunder for at undgå for megen flimmer på skærmen
  delay(100);
}

// Denne funktion tjekker om der er trykket på to eller flere knapper
bool two_button_press_check(){
  if(digitalRead(RED_INPUT) + digitalRead(YELLOW_INPUT) + digitalRead(GREEN_INPUT) < 2){ // Normalt er den værdien af alle knappernes inputs lagt sammen 3 
    return true;                                                                         // (da HIGH er 1 og kanppernes deafult er HIGH hvis ikke de trykkes på)  
  } else {                                                                               // Hvis en af kanpper trykkes på, bliver dens værdi LOW (alstå 0) og summen falder til 2
    return false;                                                                        // Hvis summen er mindre end 2, er der trykket på to eller flere knapper
  }
}

// Denne funktion aktiveres hvis der er trykket på to eller flere knapper
void two_button_press_warning(){
  // Sæt skærmen til at vise fejlmeddelelse med en rød baggrund
  lcd.clear();
  lcd.setRGB(255, 0, 0);
  lcd.setCursor(0,0);
  lcd.print("Input overload!");
  lcd.setCursor(0,1);
  lcd.print("V");
  lcd.write((unsigned char)0); // Skriv æ
  lcd.setCursor(2,1);
  lcd.print("lge en knap!");

  // Tænd de knapper der er trykkes på
  for (short i = 0; i<3; i++){
    if(digitalRead(buttons[i]) == ON){
      digitalWrite(lights[i], ON);
    } else {
      digitalWrite(lights[i], OFF);
    }
  }
}

short check_button_press(){ // Denne funktion tjekker om der er trykket på en knap
  // Hvis der trykkes på ingen knapper, retuneres -1
  // Hvis der er trykket på to eller flere knapper, returneres -2
  if(two_button_press_check()){
    return -2;
  } else {
    for (short i = 0; i<3; i++){
      if(digitalRead(buttons[i]) == ON){
        return i;
      } else {
        digitalWrite(lights[i], OFF);
      } 
    }
    return -1;
  };
}

// Denne funktion aktiveres når der trykkes på en knap
// Den viser en besked på skærmen og venter på at knappen holdes nede i den tide der er angivet i input_delay
// Når knappen er holdt nede i den tid, skifter skærmen til en bekræftelsesbesked
void confirm_button_press(short button){
  // Sæt skærmen til at vise ventebesked med den valgte farve
  // Vækg det lys der svarer til den valgte knap
  String wait_tag = "VENT: ";
  lcd.clear();
  lcd.setRGB(colors[button][0], colors[button][1], colors[button][2]);
  digitalWrite(lights[button], ON);
  lcd.setCursor(0,0);
  lcd.print("Hold knappen nede");
  lcd.setCursor(0,1);
  lcd.print(wait_tag);
  
  // Vent i den tid der er angivet i input_delay, mens der vises et # for hvert tegn der mangler
  // Hvis check_button_press() returnerer andet end indexet på den førstvalgte knap, returneres funktionen og processen afbrydes
  for (unsigned short i = 0; i<16-wait_tag.length(); i++){
    lcd.print("#");
    if(check_button_press() != button){
      return;
    }
    delay(input_delay/(16-wait_tag.length()));
  }

  // Hvis knappen er holdt nede i den tid, skifter skærmen til en bekræftelsesbesked
  lcd.clear();
  lcd.setRGB(0,255,0);
  lcd.setCursor(0,0);
  lcd.print("Tak for din tid");
  lcd.setCursor(0,1);
  lcd.print("Svaret er gemt!");
  delay(2000);
}