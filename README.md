# Kantinetilfredshed | Informatik eksamen 2025
Elevrådet vil gerne kunne krotlægge elevernes tilfredshed med kantinens mad. I skal designe et system, hvor elever kan give feedback med knapper, og hvor data kan aflæses digitalt. Da det er en lokalt produceret løsning skal dele af systemet kunne produceres i skolens makerspace (3D print / Lasercut)

## Opstart
Vi startede hele projektet med at læse opgaven og derefter lave en brainstorm over hvordan man kunne løse den.
![brainstorm](https://github.com/user-attachments/assets/55b62f28-e19b-47f2-abf4-e8fdc6c43e21)
Da vi så havde brainstormen på plads fandt vi nogle krav til løsningen og derefter farvende vi idé'erne alt efter om vi synes det var en god idé og om det var muligt.
![kravfeatures](https://github.com/user-attachments/assets/ed3a3d63-d905-458a-bf46-b621b74b4859)
![brainstorm_afgrænset](https://github.com/user-attachments/assets/cfc68d32-98df-477f-b8ac-485239ecaf4d)

### Gruppe Struktur
Det første vi skulle bruge for at have en god gruppestruktur er at oprette et github repo. Det har til formål at lagre alt vi arbejder med og har arbejdet med kan ligge så alle i gruppen kan tilgå og arbjde med det.
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
Efter fiflen fik vi den første prototype op og køre:
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
Da den grove functionalitet nu er blevet skrevet ned, skal et mere avanceret og fyldesgørende design laves:


For at gøre det nemt for brugeren at interagere med har vi valgt at benytte store knapper der både har en farve og et symbol der viser "hvad de gør".

Koden til arduinoen er blevet forbedret i en iterativ process, koden kan ses inde på dette Github Repo eller på dette link: <br>
[https://github.com/JamesPness/Kantinetilfredshed/blob/main/main.cpp](url)

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

## Server / Datalaget
Serveren har 2 formål.
1. Lagre dataen med timestamps.
2. Modtage requests om at aflevere data, og requests om at tilgå data.

Det lavede vi et flowchart over så det var nemt at få overblik. <br>
![server_flowchart](https://github.com/user-attachments/assets/7ba723df-c004-42d7-9ade-bae8c5f2fc96)

## Fremtiden
