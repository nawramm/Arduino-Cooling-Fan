//including the libraries
#include <OneWire.h>
#include <DallasTemperature.h>

//Set which pin dts is connected to the Arduino
#define ONE_WIRE_BUS A0

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//set a float for the temperature data to be stored on
float temp = 0;

//Green LED connected to D6
int GLED = 6;
//Yellow LED connected to D7
int YLED = 7;
//Red LED connected to D8
int RLED = 8;

//First fan is connected to D9
int FAN1 = 9;
//Second fan is connected to D10
int FAN2 = 10;

//Override PTM is connected to A1
int PTM = A1;
//variable to store PTM state
int PTMstate = 0;
//white override LED is connecte to A3
int WLED = A3;





void setup() {
  // put your setup code here, to run once:

  // start serial monitor to send temp values to
  Serial.begin(9600);

  // Start up the library
  sensors.begin();

  //Set LEDs as output
  //if you don't, LEDs will be dim
  pinMode(GLED, OUTPUT);
  pinMode(YLED, OUTPUT);
  pinMode(RLED, OUTPUT);

  //Set fans as output
  pinMode(FAN1, OUTPUT);
  pinMode(FAN2, OUTPUT);

  //Set Switch and PTM as Input
  pinMode(PTM, INPUT);

}
void loop() {
  // put your main code here, to run repeatedly:

PTMstate = digitalRead(PTM);

  if (PTMstate == HIGH){

    digitalWrite(WLED,HIGH);
    digitalWrite(FAN1,HIGH);
    digitalWrite(FAN2,HIGH);

  } else {


  sensors.requestTemperatures();      // Send the command to get temperature readings

  temp = sensors.getTempCByIndex(0); //temp is set as dts temperature value in degrees celsius
  Serial.println(temp);              //print temp value

  if (temp < 20) {                   //if normal temp
      digitalWrite(GLED, HIGH);        //green LED on
      digitalWrite(FAN1, LOW);         //first fan off

    } else {                           //temp is greater than 20C
      digitalWrite(GLED, LOW);         //green LED off
    }
    if (temp >= 20 && temp < 24) {          //if medium temp
      digitalWrite(YLED, HIGH);        //yellow LED on
      digitalWrite(FAN1, HIGH);        //first fan on

    } else {                           //temp is less than 20/less than medium
      digitalWrite(YLED, LOW);         //yellow LED off
    }
    if (temp >= 24) {                  //if high temp
      digitalWrite(RLED, HIGH);        //red led on
      digitalWrite(FAN2, HIGH);        //second fan on

    } else {                           //temp is less than 24/less than high
      digitalWrite(RLED, LOW);         //red LED off
      digitalWrite(FAN2, LOW);         //second fan off
    }
  delay(1000);                       //delay 1000 ms
}
}

