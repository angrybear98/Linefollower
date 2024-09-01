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
“debug” lists the following parameters on the device:
1.	Cycletime in microseconds. (how fast the loop will be run, example: 100000 will set the loop to run 10/s.)
2.	P-Value: the P-Value of the PID parameters.
3.	I-Value: the I-Value of the PID parameters.
4.	D-Value: the D-Value of the PID parameters.
5.	maxSpeed: the maximum speed that the program will allow the motors to turn (physical limit: 255).
6.	baseSpeed: the base speed that the program will let the motors turn in a straight line (from testing the minimum speed that lets the motors turn is 35).
7.	threshold: the value at which the program will determine if a line is present or not (used for different types of tracks with different illumination).
8.	calculation time : the time the program takes to run a single loop in microseconds.
“set(space)(parameters)(space)(value)” sets the previous parameters to the desired value.
Example: set cycle 100000 will set the cycletime to 100000 microseconds
commands: cycle, kp, ki, kd, max, base, threshold.
Calculation time is an indicator for errors and therefore cannot be altered.
“calibrate white/black”: this will calibrate the sensors for white and black surfaces in case the program can’t immediately detect the line.
“debug calibration”: this will list the calibrated values for black and white.
“start”: starts the program and runs the robot.
“stop”: stops the program and halts the robot.


### kalibratie
Door de robot boven een zuiver wit oppervlakte te plaatsen en calibrate white in te geven, zal de robot de waardes voor wit opslaan
doe hetzelfde voor zwart boven een zuiver zwart oppervlakte.
de robot zal nu de 2 extremen mappen en constrainen binnen de 2 waardes en duidelijk de lijn vinden.
### settings
cycletime: 100000
P-Value: 1
I-Value: 4.4
D-Value 0.58
maxSpeed: 150
baseSpeed: 35
threshold: 650
### start/stop button
door start te sturen zal de robot beginnen rijden en automatisch stoppen als de lijn verloren is.
door stop te sturen zal de robot onmiddelijk stoppen met rijden en wachten op volgende orders.
### macros terminal
voor gebruiksgemak kunnen macros gemaakt worden in de app die voorgestelde commando's kan sturen met 1 knop of commando's opbouwen door het een insert functie te maken bij de instellingen.
