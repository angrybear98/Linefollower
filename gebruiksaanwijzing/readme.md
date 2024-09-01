# Gebruiksaanwijzing

### opladen / vervangen batterijen
 De batterijen kunnen vervangen worden of opgeladen, de gebruikte laadstation was van nitecore ui2 batterijlader.
De batterijen zitten onderaan de robot en kunnen uitgetroken worden.
### draadloze communicatie
de app "Serial Bluetooth Terminal" van de playstore op android word gebruikt voor seriele communicatie met de HC-05 module.
#### verbinding maken
open de app terwijl dat de robot aan staat en zet de telefoon bluetotth functie aan, in de app kan je bij devices de HC-05 module vinden, dit zal voor een passwoord vragen, de standaard passwoord zal 1234 zijn of 0000.
Hierna kan je in de terminal van de app verbinding maken met de module.
#### commando's
"debug" geeft de volgende parameters weer op het apparaat:

Cycletime: hoe snel de loop zal worden uitgevoerd in microseconden (voorbeeld: 100000 stelt de loop in op 10 keer per seconde).
P-Waarde: de P-waarde van de PID-parameters.

I-Waarde: de I-waarde van de PID-parameters.

D-Waarde: de D-waarde van de PID-parameters.

maxSpeed: de maximale snelheid waarmee het programma de motoren zal laten draaien (fysieke limiet: 255).

baseSpeed: de basissnelheid waarmee het programma de motoren in een rechte lijn zal laten draaien (uit tests blijkt dat de minimale snelheid om de motoren te laten draaien 35 is).

threshold: de waarde waarmee het programma bepaalt of er een lijn aanwezig is of niet (gebruikt voor verschillende soorten tracks met verschillende verlichting).

calculationtime: de tijd die het programma nodig heeft om een enkele loop uit te voeren in microseconden.

"set(space)(parameter)(space)(waarde)" stelt de vorige parameters in op de gewenste waarde. Voorbeeld: set cycle 100000 stelt de cycletime in op 100000 microseconden. 

Beschikbare commando's: cycle, kp, ki, kd, max, base, threshold. 

calculationtime is een indicator voor fouten en kan daarom niet worden aangepast.

"calibrate white/black": dit kalibreert de sensoren voor witte en zwarte oppervlakken in het geval het programma de lijn niet direct kan detecteren. 

"debug calibration": dit geeft de gekalibreerde waarden voor zwart en wit weer. 

"start": start het programma en laat de robot lopen. 

"stop": stopt het programma en zet de robot stil.

### kalibratie
Door de robot boven een zuiver wit oppervlakte te plaatsen en calibrate white in te geven, zal de robot de waardes voor wit opslaan
doe hetzelfde voor zwart boven een zuiver zwart oppervlakte.
de robot zal nu de 2 extremen mappen en constrainen binnen de 2 waardes en duidelijk de lijn vinden.
### settings
cycletime: 50000

P-Value: 1

I-Value: 2.7

D-Value 0.46

maxSpeed: 150

baseSpeed: 35

threshold: 650

### start/stop button
door start te sturen zal de robot beginnen rijden en automatisch stoppen als de lijn verloren is.
door stop te sturen zal de robot onmiddelijk stoppen met rijden en wachten op volgende orders.
### macros terminal
voor gebruiksgemak kunnen macros gemaakt worden in de app die voorgestelde commando's kan sturen met 1 knop of commando's opbouwen door het een insert functie te maken bij de instellingen.
