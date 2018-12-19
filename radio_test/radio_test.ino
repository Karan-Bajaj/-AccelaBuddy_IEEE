#include <Servo.h> 
 #include <SPI.h>
 #include "RF24.h"
 //SCK -> 13//MISO -> 12//MOSI -> 11//CSN -> 7//CE -> 8
RF24 radio(7,8);
 const uint64_t pipe = 0xE8E8F0F0E1LL; 
 int msg[2];
 void setup() 
 { 
 radio.begin();
 radio.openReadingPipe(1,pipe);
 radio.startListening(); 
 Serial.begin(9600);
 } 
 void loop() 
 { 
 while (radio.available()){

 radio.read(msg, 2); 
 Serial.print(msg[0]);
 Serial.print(" ");
 Serial.print(msg[1]);
 Serial.println("");
 }
 }
