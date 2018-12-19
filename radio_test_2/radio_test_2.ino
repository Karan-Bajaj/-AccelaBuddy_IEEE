/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte receiveAddress[6] = "10011";
const byte sendAddress[6] = "01100";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, receiveAddress);
  radio.openWritingPipe(sendAddress);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    
    Serial.println(text);

    //Serial.print(" ");
    //Serial.println(text[1]);
  }

  radio.stopListening();
  const char text2[] = "Hello Aadit";
  radio.write(&text2, sizeof(text2));
  radio.startListening();
  delay(10);
}
