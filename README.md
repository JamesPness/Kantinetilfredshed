# Kantinetilfredshed | Informatik eksamen 2025
Elevrådet vil gerne kunne krotlægge elevernes tilfredshed med kantinens mad. I skal designe et system, hvor elever kan give feedback med knapper, og hvor data kan aflæses digitalt. Da det er en lokalt produceret løsning skal dele af systemet kunne produceres i skolens makerspace (3D print / Lasercut)

## Opstart
Vi startede hele projektet med at læse opgaven og derefter lave en brainstorm over hvordan man kunne løse den.
![brainstorm](https://github.com/user-attachments/assets/55b62f28-e19b-47f2-abf4-e8fdc6c43e21)
Da vi så havde brainstormen på plads fandt vi nogle krav til løsningen og derefter farvende vi idé'erne alt efter om vi synes det var en god idé og om det var muligt.
![kravfeatures](https://github.com/user-attachments/assets/ed3a3d63-d905-458a-bf46-b621b74b4859)
![brainstorm_afgrænset](https://github.com/user-attachments/assets/cfc68d32-98df-477f-b8ac-485239ecaf4d)

### Gruppe Struktur
Det første vi skulle bruge for at have en god gruppestruktur er at oprette et github repo. Det har til formål at lagre alt vi arbejder med og har arbejdet med kan ligge så alle i gruppen kan tilgå og arbejde med det.
Linket til githubben ses her: <br>
[https://github.com/JamesPness/Kantinetilfredshed/](url) <br>

Derefter kan vi så lave en Trello så alle i gruppe kan se hvad der skal laves, hvad folk er i gang med at lave, og hvad der er lavet. Derudover bliver der også uddelt opgaver igennem Trelloen. Linket til Trellon ses her: <br> 
[https://trello.com/b/xtoLP17R/kantinetilfredshed](url) <br>

Og et billede af Trelloen ses nedenunder: <br>
![trello](https://github.com/user-attachments/assets/b0a4268a-d87c-43f6-9cac-02fdcdf57485)

## Systemoversigt
Da vi havde på plads hvad vores system skulle kunne og hvordan den så ud, samt havde gruppe strukturen på plads begynde vi at arbejde med selve systemet. Vi har valgt at dele systemet op i 3 lag: Præsentationslaget, Logiklaget og Datalaget. De ses her:
![blokdiagram-trelagsmodel](https://github.com/user-attachments/assets/6d76eff1-115e-4b9b-9460-9b5e4015b034)

## Arduinoen / Logiklaget
Det første vi gik i gang med var at lave logiklaget.
Dette havde vi allerede fået givet at det skulle forgå på en Arduino.
For at få styr på hvad vores arduino skulle kunne, lavede vi et flowchart: <br> 
![arduino-flowchart](https://github.com/user-attachments/assets/aa105073-9148-4705-9437-998a68dd29c1)
Dette flowchart skal ræpræsentere den basal funktion af arduinoen.
For at gøre det nemt for brugeren at interagere med har vi valgt at benytte store knapper der både har en farve og et symbol der viser "hvad de gør".
<details open>
<summary>Efter fiflen fik vi den første prototype op og køre:</summary>

```cpp
#include "rgb_lcd.h"
#include <Arduino.h>

// Sæt et navn til de forskellige pins for at undgå forvirring
#define RED_LIGHT 6
#define RED_INPUT 7

#define YELLOW_LIGHT 8
#define YELLOW_INPUT 9

#define GREEN_LIGHT 2
#define GREEN_INPUT 3

// Initialiser de brugte variabler
rgb_lcd lcd;

short colorR = 0;
short colorG = 255;
short colorB = 0;

short input_delay = 2000;

short buttons[3];
short lights[3];

// Setup - Kode der sørger for yderligererr opsætning af variabler
void setup() {
  Serial.begin(9600);
  while (!Serial);

  //færdoggør variabel initialisering
  buttons[0] = RED_INPUT;
  buttons[1] = YELLOW_INPUT;
  buttons[2] = GREEN_INPUT;

  lights[0] = RED_LIGHT;
  lights[1] = YELLOW_LIGHT;
  lights[2] = GREEN_LIGHT;

  // Opsæt pinmode for lys og knapper
  pinMode(RED_LIGHT, OUTPUT);
  pinMode(RED_INPUT, INPUT_PULLUP);

  pinMode(YELLOW_LIGHT, OUTPUT);
  pinMode(YELLOW_INPUT, INPUT_PULLUP);

  pinMode(GREEN_LIGHT, OUTPUT);
  pinMode(GREEN_INPUT, INPUT_PULLUP);

  // SLuk alle lysende
  digitalWrite(RED_LIGHT, LOW);
  digitalWrite(YELLOW_LIGHT, LOW);
  digitalWrite(GREEN_LIGHT, LOW);

  // Initialiser LCD-skærmen og print en lille besked
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("Kantine-");
  lcd.setCursor(0,1);
  lcd.print("tilfredshed");

  //Giv brugeren tid til at læse beskeden
  delay(1500);

  colorR = 255;
  colorG = 255;
  colorB = 255;
}

// Loop - Kode der kører hele tiden
// Denne funktion kører hele tiden og tjekker om der er trykket på en knap
void loop() {

  // Løb igennem alle knapperne igennem, og få dem til at lyse hvis de bliver trykket på.
  for (int i = 0; i<3; i++){
    if(digitalRead(buttons[i]) == HIGH){ 
      digitalWrite(lights[i], HIGH);
      Serial.println(i);

    // lav lcd skærmen samme farve som den knap der trykkes på
      if (i == 0) {
        lcd.setRGB(255, 0, 0);
      } else if (i == 1) {
        lcd.setRGB(255, 255, 0);
      } else if (i == 2) {
        lcd.setRGB(0, 255, 0);
      }
    } else {
      digitalWrite(lights[i], LOW); // hvis ikke de bliver trykket på sørg for at de bliver slukket
    }
  }

  // Skriv en lille besked hvis ikke der sker noget
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Er maden god?");
  lcd.setCursor(0,1);
  lcd.print("Giv os et preg!");

  delay(100);
}
```
</details>
Da den grove funktionalitet nu er blevet skrevet ned, skal et mere avanceret og fyldesgørende design laves:

![arduino-flowchart - v2](https://github.com/user-attachments/assets/a621ffc8-37cf-4642-869c-130d89581540)
Den nye og opdaterede version er forbedret med håndtering af input, samt en let delay for at forhindre for megen spam.
<details open>
<summary>Den nye og kode ser nu sådan ud:</summary>

```cpp
#include <Wire.h>
#include "rgb_lcd.h"
#include <Arduino.h>
#include <pins_arduino.h>
#include <math.h>
#include <string.h>

// Sæt et navn til de forskellige pins for at undgå forvirring
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

  // Sluk alle lys
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
  lcd.print("lge EN knap!");

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
```
</details>

## Præsentationslaget
Derefter gik vi i gang med præsentationslaget, det var delt op i 2 dele, den ene var bruger interagering og det andet var datavisualisering.

### Knappe-kasse / Bruger interagering
Først lavede vi en skitse på baggrund af vores krav / features og brainstorm
![skitse](https://github.com/user-attachments/assets/f0320cdb-e709-45d3-8573-05ae06f13cea)

Derefter gik vi i gang med at designe kassen som den skal kunne være i. Fordi den skulle kunne produceres lokalt, altså enten lasercut eller 3dprint, valgte vi at lave en lasercutet kasse.
Kassen er designet ud fra skitsen og ser således ud før det er skåret ud: <br>
![knappe-kasse](https://github.com/user-attachments/assets/742552c1-f55e-4021-acff-f1301ade6a58)
Kassen er designet med "finger joints" som er med til at holde kassen sammen.

### Skærm / Datavisualisering
Vi lavede først en grow skitse over hvordan vi kunne tænke os at dataen skulle præsenteres for brugerene.
![datavisualisering-grow](https://github.com/user-attachments/assets/d96bdb9b-faf2-460d-9f2e-2bef15ff32d3)

Da vi så skulle endeligt designe layoutet kiggede vi blandt andet på Jacob Nielsen's 10 bud.
![datavisualisering](https://github.com/user-attachments/assets/6e5ba890-0ede-4fee-82c7-25db38d7ae7f)

Dette er den primære måde at se dataen på men dataen kan dog også hentes som en CSV fil fra serveren.

## Server / Datalaget
Serveren har 2 formål.
1. Lagre dataen med timestamps.
2. Modtage requests om at aflevere data, og requests om at tilgå data.

Det lavede vi et flowchart over så det var nemt at få overblik. <br>
![server_flowchart](https://github.com/user-attachments/assets/7ba723df-c004-42d7-9ade-bae8c5f2fc96)

## Fremtiden
