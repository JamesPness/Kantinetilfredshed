# Kantinetilfredshed | Informatik eksamen 2025
Elevrådet vil gerne kunne krotlægge elevernes tilfredshed med kantinens mad. I skal designe et system, hvor elever kan give feedback med knapper, og hvor data kan aflæses digitalt. Da det er en lokalt produceret løsning skal dele af systemet kunne produceres i skolens makerspace (3D print / Lasercut)

## Opstart
Vi startede hele projektet med at læse opgaven og derefter lave en brainstorm over hvordan man kunne løse den.
![brainstorm](https://github.com/user-attachments/assets/55b62f28-e19b-47f2-abf4-e8fdc6c43e21)
Da vi så havde brainstormen på plads fandt vi nogle krav til løsningen og derefter farvende vi idé'erne alt efter om vi synes det var en god idé og om det var muligt.
![kravfeatures](https://github.com/user-attachments/assets/ed3a3d63-d905-458a-bf46-b621b74b4859)
![brainstorm_afgrænset](https://github.com/user-attachments/assets/cfc68d32-98df-477f-b8ac-485239ecaf4d)

## Systemoversigt
Da vi havde på plads hvad vores system skulle kunne og hvordan den så ud, samt havde gruppe strukturen på plads begynde vi at arbejde med selve systemet. Vi har valgt at dele systemet op i 3 lag: Præsentationslaget, Logiklaget og Datalaget. De ses her:
![blokdiagram-trelagsmodel](https://github.com/user-attachments/assets/6d76eff1-115e-4b9b-9460-9b5e4015b034)

### Knappe-kasse / Præsentationslaget

### Arduinoen / Logiklaget
For at få styr på hvad vores arduino skulle kunne, lavede vi et flowchart det: <br> 
![arduino-flowchart](https://github.com/user-attachments/assets/aa105073-9148-4705-9437-998a68dd29c1)

### Server / Datalaget
Serveren har 2 formål.
1. Lagre dataen med timestamps.
2. Modtage requests om at aflevere data, og requests om at tilgå data.
Det lavede vi et flowchart over så det var nemt at få overblik. <br>
![server_flowchart](https://github.com/user-attachments/assets/7ba723df-c004-42d7-9ade-bae8c5f2fc96)
