#include <SoftwareSerial.h>


const int LDR = A0;          //Read the LDR value from the point where it meets the Resistor
const int LASER = 7;          //LASER trigger pin
const int buzzer = 12;     //Buzzer pin
int LDR_Value;

SoftwareSerial sim800l(3, 2);    // RX,TX for Arduino and for the module it's TXD RXD, they should be inverted

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
   sim800l.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(sim800l.available()) 
  {
    Serial.write(sim800l.read());//Forward what Software Serial received to Serial Port
  }
}


void setup() {
  
  pinMode(LDR, INPUT);          //Define pinModes
  pinMode(LASER, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  Serial.begin(9600);           //Start the Serial communication with the PC and SIM800L
  sim800l.begin(9600);
  Serial.println("Initializing..."); 
  sim800l.println("AT"); //Once the handshake test is successful, i t will back to OK
  updateSerial();
  delay(1000);
  
  digitalWrite(LASER,HIGH);     //Put the LASER at high state
  delay(5000);
}

void loop() {
  
  LDR_Value=analogRead(LDR);      //Constantly read and display the LDR value
  Serial.println(LDR_Value);
  delay(1000);                     //To avoid false reading
  
  if (LDR_Value < 700){                       //If someone passes through the value will drop, 800 is the threshold, you can chose whatever suits you, the serial monitor will help you
     Serial.println ("Intruder Alert");
     {
     digitalWrite(buzzer, HIGH);
     delay (2000);
     digitalWrite(buzzer, LOW);
     }
     sim800l.println("ATD+ <number>;");  //The number to call don't forget to add the country code
     updateSerial();
   }
}
