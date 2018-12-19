
#include <Canbus.h>
char buffer[456];  //Data will be temporarily stored to this buffer before being written to the file
float noThrottleAccel = -1000;
float currentAccel = 0;

int currentSpeed = 0;
int lastSpeed = 0;

bool isBraking = false;

//********************************Setup Loop*********************************//

void setup(){
Serial.begin(9600);
Serial.println("CAN-Bus Demo");

Serial.print("You're a quivering pussy");
if(Canbus.init(CANSPEED_500))  /* Initialise MCP2515 CAN controller at the specified speed */
  {
    Serial.println("CAN Init ok");
  } else
  {
    Serial.println("Can't init CAN");
  }

  delay(1000); 
}



//********************************Main Loop*********************************//

void loop(){

while(Serial.available()){
   noThrottleAccel = currentAccel;
   Serial.flush();
}

currentAccel = (currentSpeed-lastSpeed)/0.2;

if(currentAccel < noThrottleAccel-5)
  isBraking = true;
else
  isBraking = false;

Canbus.ecu_req(VEHICLE_SPEED, buffer);
Serial.print("Vehicle Speed: ");
Serial.print(buffer);
Serial.print("Is isBraking: ");
Serial.println(isBraking);
delay(20);

}
