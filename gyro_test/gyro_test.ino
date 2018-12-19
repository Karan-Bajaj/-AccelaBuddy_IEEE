#define GYRO 0x68 // gyro I2C address

#include <Wire.h>

void setup()
{
    Wire.begin();
    // Do other gyro stuff here, sample rate, max speed, power management
    // From datasheet, set power management to internal oscillator
    Wire.beginTransmission(GYRO); //Device Address
    Wire.write(0x3E); //Register Address
    Wire.write(0x00); // Value
    Wire.endTransmission();
    Serial.begin(9600);
}

void loop()
{
    byte buff[8];
    // A quick read example, temperature, gyro x,y,z
    Wire.beginTransmission(GYRO);
    Wire.write(0x1B); //Register Address TEMP_H
    Wire.endTransmission();

    Wire.beginTransmission(GYRO);
    Wire.requestFrom(GYRO,6); // Read 8 bytes
    int ii = 0;
    while(Wire.available())
    {
        buff[ii] = Wire.read();
        ii++;
    }
    Wire.endTransmission();

    //Combine bytes into ints
    int temperature = buff[0] << 8 | buff[1];
    int gyro_x = buff[2] << 8 | buff[3];
    int gyro_y = buff[4] << 8 | buff[5];
    int gyro_z = buff[6] << 8 | buff[7];

    Serial.print(gyro_x);
    Serial.print("   ");
    Serial.print(gyro_y);
    Serial.print("   ");
    Serial.print(gyro_z);
    Serial.println("");

    delay(500);
}
