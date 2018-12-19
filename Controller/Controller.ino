//Michael Rokas, Aadit Javeri, Karan Bajaj
//Feb. 17th 2018

//Sample code used:
//http://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
//https://github.com/ControlEverythingCommunity/KXTF9-4100/blob/master/Arduino/KXTF9-4100.ino
//http://www.hobbytronics.co.uk/arduino-adxl345-imu3000

#include<Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//KXTF9-4100 I2C address is 0x0F(15)
#define Addr 0x0F
#define GYRO 0x68         // gyro I2C address

RF24 radio(7, 8); // CE, CSN

//
const byte sendAddress[6] = "10011";

//Offsets for accelerometer
int forwardBackwardOffset = 0;
int leftRightOffset = 0;

void setup()
{ 
  pinMode(2, OUTPUT);
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(sendAddress);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  //Set passthrough so gyro can access accelerometer data
  //as accelerometer is not leftRightectly connected to I2C line
  writeTo(GYRO, 0x3D, 0x08);
  
  //Start I2C
  Wire.beginTransmission(Addr);
  // Select Control register1
  Wire.write(0x1B);
  // Operating mode, 12-bit valid
  Wire.write(0xD0);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);

  //Run calibration function to calibrate accelerometer offsets
  calibrate(0,0,0);
}

void calibrate(int count, int forwardBackwardSum, int leftRightSum){
  int forwardBackward;
  int leftRight;

  //Turn on LED to notify calibrating
  digitalWrite(2, HIGH);

  //Get the current angle
  getAccelData(forwardBackward, leftRight);
  
  forwardBackwardSum += forwardBackward;
  leftRightSum += leftRight;

  if(count < 2){
    //Get another test case after 1 sec
    delay(1000);
    calibrate(count+1,forwardBackwardSum,leftRightSum);
  }
  else{
    //Store the average accelerometer readings as the offsets
    forwardBackwardOffset = forwardBackwardSum/3;
    leftRightOffset = leftRightSum/3;
    //Turn the calibration notifier LED off 
    digitalWrite(2,LOW);
  }
}

void loop()
{
  int forwardBackward;
  int leftRight;

  //Get the current angle
  getAccelData(forwardBackward, leftRight);
  
  // Output data to serial monitor
  Serial.print("forwardBackwardnitude:");
  Serial.print(forwardBackward);
  Serial.print(", Rotaton:");
  Serial.println(leftRight);

  //Send the current controller angle to the robot
  const int text[2] = {forwardBackward, leftRight};
  radio.write(&text, sizeof(text));
  
  delay(20);
}

void getAccelData(int& forwardBackward, int& leftRight){
  unsigned int data[6];

  //Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x06);
  // Stop I2C Transmission
  Wire.endTransmission();

  //Request 6 bytes of data
  Wire.requestFrom(Addr, 6);

  //Read 6 bytes of data
  //xAccl lsb, xAccl msb, yAccl lsb, yAccl msb
  if (Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
  }

  //Convert the data to 12-bits
  int xAccl = (data[1] * 256 + (data[0] & 0xF0)) / 16;
  if (xAccl > 2047)
  {
    xAccl -= 4096;
  }
  int yAccl = (data[3] * 256 + (data[2] & 0xF0)) / 16;
  if (yAccl > 2047)
  {
    yAccl -= 4096;
  }

  //Scale angles and apply offset
  forwardBackward = xAccl*0.91 - forwardBackwardOffset;
  leftRight = yAccl*0.91 - leftRightOffset;

  //Make sure values are not over 255as these values will be used
  //for PWM by the robot 
  if(forwardBackward > 255)
    forwardBackward = 255;
  else if(forwardBackward < -255)
    forwardBackward = -255;

  if(leftRight > 255)
    leftRight = 255;
  else if(leftRight < -255)
    leftRight = -255;
}

void writeTo(int device, byte address, byte val) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);            //write register address
  Wire.write(val);                //write value to write
  Wire.endTransmission();         //end transmission
}

