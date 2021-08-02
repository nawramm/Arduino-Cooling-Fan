//including the libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

//Set which pin dts is connected to the Arduino
#define ONE_WIRE_BUS A0

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//set lcd address
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

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

  // Initiate the LCD:
  lcd.init();
  lcd.backlight();
  lcd.print("Mode:");
  lcd.setCursor(0, 1);
  lcd.print("Temp:");

}
void loop() {
  // put your main code here, to run repeatedly:
  sensors.requestTemperatures();      // Send the command to get temperature readings

  temp = sensors.getTempCByIndex(0); //temp is set as dts temperature value in degrees celsius
  Serial.println(temp);              //print temp value

  
  lcd.setCursor(5, 1); //move cursor
  lcd.print(temp);
  lcd.print(char(223));//print degrees sign
  lcd.print("C");//print "C"

  PTMstate = digitalRead(PTM);

  if (PTMstate == HIGH) {
    lcd.setCursor(6, 0);
    lcd.print("Manual");

    digitalWrite(WLED, HIGH);//White LED on
    digitalWrite(FAN1, HIGH);//Fan 1 on
    digitalWrite(FAN2, HIGH);//Fan 2 on

    //temperature indicating LEDs off
    digitalWrite(GLED, LOW);
    digitalWrite(YLED, LOW);
    digitalWrite(RLED, LOW);


  } else {
    lcd.setCursor(6, 0);
    lcd.print("Auto  ");
    digitalWrite(WLED, LOW); //override indicating LED off


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
  }
}
